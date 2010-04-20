/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 3        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "gl3.h"

#include <QtOpenGL/QGLFramebufferObject>

#include "canvas.h"

#include "scene/object.h"
#include "scene/light.h"

#include "storage/mesh.h"
#include "storage/material.h"
#include "storage/texture.h"
#include "storage/shaderprogram.h"
#include "storage/shader.h"

#include "shader_p.h"
#include "buffer_p.h"
#include "fbo_p.h"

struct BufferElement {
  GLfloat vertex[3];
  GLfloat normal[3];
  GLfloat uvMap[2];
  GLfloat uvMap1[2];
  GLfloat uvMap2[2];
  GLubyte padding[16];
};

#define VERTEX_OFFSET 0
#define NORMAL_OFFSET (3 * sizeof(GLfloat))
#define UV_OFFSET (6 * sizeof(GLfloat))
#define UV1_OFFSET (8 * sizeof(GLfloat))
#define UV2_OFFSET (10 * sizeof(GLfloat))

using namespace BGE;
using namespace BGE::Driver;

class FBO
{
  public:
    inline FBO(const QSize &size)
    {
      glGenFramebuffers(1, &m_frame);
      glBindFramebuffer(GL_FRAMEBUFFER, m_frame);

      glGenRenderbuffers(1, &m_depth);
      glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.width(), size.height());
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

      glGenTextures(7, m_textures);

      for (quint8 i = 0; i < 8; i++) {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, size.width(), size.height(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
      }

      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE)
        qFatal("FBO error!");

      m_size = size;
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline ~FBO()
    {
      glDeleteTextures(7, m_textures);
      glDeleteRenderbuffers(1, &m_depth);
      glDeleteFramebuffers(1, &m_frame);
    }

    inline void bind() const
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_frame);
    }

    inline void unbind() const
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline const QSize &size() const
    {
      return m_size;
    }

    inline void activateTextures()
    {
      for (quint8 i = 0; i < 7; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
      }
    }

    inline void deactivateTextures()
    {
      for (quint8 i = 0; i < 7; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      glActiveTexture(GL_TEXTURE0);
    }

  private:
    GLuint m_textures[7];
    QSize m_size;
    GLuint m_frame;
    GLuint m_depth;
};

GL3::GL3()
{
  getShaderFunctions();
  getBufferFunctions();
  getFBOFunctions();
  m_renderedLights = 0;
  m_boundShader = 0l;
  m_boundMesh = 0l;
  m_quad = 0;
  m_quadIdxs = 0;

  if (m_quad != 0 && m_quadIdxs != 0) {
    GLuint ids[2] = {m_quad, m_quadIdxs};
    glDeleteBuffers(2, ids);
  }
}

GL3::~GL3()
{
  delete m_fbo;
}

void GL3::bindFBO()
{
  GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6};
  m_fbo->bind();
  glDrawBuffers(7, buffers);
}

void GL3::unbindFBO()
{
  m_fbo->unbind();
  glDrawBuffer(GL_BACK);
}

void GL3::bind(Storage::Mesh *mesh)
{
  if (!mesh)
    return;
  if (!mesh->bindId()) {
    load(mesh);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->bindId());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices.value(mesh->bindId()));
  }

  // Let's bind our shader thingies :)
  bindUniformAttribute(m_boundShader, "ProjectionMatrix", m_projectionMatrix);
  bindUniformAttribute(m_boundShader, "ModelViewMatrix", m_transform.matrix());
  bindUniformAttribute(m_boundShader, "NormalMatrix", m_normalMatrix);
  bindAttribute(m_boundShader, "Vertex", 3, GL_FLOAT, sizeof(BufferElement), VERTEX_OFFSET);
  bindAttribute(m_boundShader, "Normal", 3, GL_FLOAT, sizeof(BufferElement), NORMAL_OFFSET);
  bindAttribute(m_boundShader, "TexCoord", 2, GL_FLOAT, sizeof(BufferElement), UV_OFFSET);
  m_boundMesh = mesh;
}

void GL3::bind(Storage::Texture *texture)
{
  if (!texture)
    return;

  if (!texture->bindId())
    texture->setBindId(Canvas::canvas()->bindTexture(texture->texture()));

  if (texture->bindId())
    glBindTexture(GL_TEXTURE_2D, texture->bindId());
}

void GL3::bind(Storage::ShaderProgram *shaderProgram)
{
  if (!shaderProgram)
    return;

  if (!shaderProgram->bindId())
    load(shaderProgram);

  glUseProgram(shaderProgram->bindId());
  m_boundShader = shaderProgram;
}

void GL3::unbind(Storage::Mesh *mesh)
{
  if (!mesh || !mesh->bindId())
    return;

  unbindAttribute(m_boundShader, "Vertex");
  unbindAttribute(m_boundShader, "Normal");
  unbindAttribute(m_boundShader, "TexCoord");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  m_boundMesh = 0l;
}

void GL3::unbind(Storage::Texture *texture)
{
  Q_UNUSED(texture);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GL3::unbind(Storage::ShaderProgram *shaderProgram)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  glUseProgram(0);
  m_boundShader = 0l;
}

void GL3::unload(Storage::Mesh *mesh)
{
  if (!mesh || !mesh->bindId())
    return;

  GLuint ids[2] = {mesh->bindId(), m_indices.value(mesh->bindId())};
  glDeleteBuffers(2, ids);
  mesh->setBindId(0);
}

void GL3::unload(Storage::Texture *texture)
{
  if (!texture || !texture->bindId())
    return;

  Canvas::canvas()->deleteTexture(texture->bindId());
  texture->setBindId(0);
}

void GL3::unload(Storage::ShaderProgram *shaderProgram)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  glDeleteProgram(shaderProgram->bindId());
  shaderProgram->setBindId(0);
}

void GL3::setLight(Scene::Light *light)
{
  Light temp;
  Vector4f tempPos(0, 0, 0, 1);
  if (!light->isPositional())
    tempPos = Vector4f(light->globalPosition().x(), light->globalPosition().y(), light->globalPosition().z(), 0);
  temp.position = m_transform * tempPos;
  temp.position.w() = light->isPositional() ? 1 : 0;

  temp.ambient = Vector4f(light->ambientColor().redF(), light->ambientColor().greenF(), light->ambientColor().blueF(), light->ambientColor().alphaF());
  temp.diffuse = Vector4f(light->diffuseColor().redF(), light->diffuseColor().greenF(), light->diffuseColor().blueF(), light->diffuseColor().alphaF());
  temp.specular = Vector4f(light->specularColor().redF(), light->specularColor().greenF(), light->specularColor().blueF(), light->specularColor().alphaF());
  temp.constant = light->constantAttenuation();
  temp.linear = light->linearAttenuation();
  temp.quadratic = light->quadraticAttenuation();
  if (light->isPositional() && light->isSpot())
    temp.spot_cutoff = light->spotCutOff();
  else
    temp.spot_cutoff = 180;
  temp.spot_exponent = light->spotExponent();
  temp.spot_direction = m_transform.rotation() * Vector3f(0, 0, -1);
  m_lights << temp;
}

void GL3::resetLighting()
{
  m_lights.clear();
}

void GL3::setTransformMatrix(const Transform3f& transform)
{
  m_transform = transform;
  Matrix4f inverse;
  transform.matrix().computeInverse(&inverse);
  inverse.transposeInPlace();
  m_normalMatrix = inverse.block<3, 3>(0, 0);
}

void GL3::draw(Scene::Object *object)
{
  if (!m_boundShader)
    qFatal("BGE::Driver::GL3::draw(): Shader not bound!");

  Storage::Material* currentMaterial = 0l;
  setMaterial(currentMaterial, m_boundShader);
  foreach (Plan plan, m_plans.value(m_boundMesh->bindId())) {
    if (currentMaterial != m_materials.value(plan.materialName)) {
      currentMaterial = m_materials.value(plan.materialName);
      setMaterial(currentMaterial, m_boundShader);
    }

    glDrawElements(plan.primitive, plan.count, GL_UNSIGNED_SHORT, (GLushort*)0 + plan.offset);
  }
}

void GL3::init()
{
  glClearColor(0, 0, 0, 0);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);

  m_fbo = new FBO(Canvas::canvas()->size());
  m_fbo->bind();
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  m_fbo->unbind();
}

void GL3::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_fbo->size() != Canvas::canvas()->size()) {
    delete m_fbo;
    m_fbo = new FBO(Canvas::canvas()->size());
    m_fbo->bind();
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    m_fbo->unbind();
  } else {
    m_fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_fbo->unbind();
  }
}

void GL3::setProjection(const Transform3f &projection)
{
  m_projectionMatrix = projection.matrix();
}

void GL3::shading()
{
  if (!m_quad) {
    // Vertices
    BufferElement *buffer = (BufferElement*) malloc(4 * sizeof(BufferElement));
    BufferElement *bufPtr = buffer;
    BufferElement temp;
    Vector3f vertex;
    Vector2f uvMap;

    // First
    vertex = Vector3f(-1, -1, 0);
    uvMap = Vector2f(0, 0);
    memcpy(temp.vertex, vertex.data(), 3 * sizeof(GLfloat));
    memcpy(temp.uvMap, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap1, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap2, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(bufPtr++, &temp, sizeof(BufferElement));

    // Second
    vertex = Vector3f(1, -1, 0);
    uvMap = Vector2f(1, 0);
    memcpy(temp.vertex, vertex.data(), 3 * sizeof(GLfloat));
    memcpy(temp.uvMap, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap1, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap2, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(bufPtr++, &temp, sizeof(BufferElement));

    // Third
    vertex = Vector3f(1, 1, 0);
    uvMap = Vector2f(1, 1);
    memcpy(temp.vertex, vertex.data(), 3 * sizeof(GLfloat));
    memcpy(temp.uvMap, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap1, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap2, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(bufPtr++, &temp, sizeof(BufferElement));

    // Fourth
    vertex = Vector3f(-1, 1, 0);
    uvMap = Vector2f(0, 1);
    memcpy(temp.vertex, vertex.data(), 3 * sizeof(GLfloat));
    memcpy(temp.uvMap, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap1, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(temp.uvMap2, uvMap.data(), 2 * sizeof(GLfloat));
    memcpy(bufPtr++, &temp, sizeof(BufferElement));

    glGenBuffers(1, &m_quad);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(BufferElement), buffer, GL_STATIC_DRAW);
    free(buffer);

    // Indices
    quint16 indices[] = {0, 1, 2, 3};
    glGenBuffers(1, &m_quadIdxs);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIdxs);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(quint16), indices, GL_STATIC_DRAW);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, m_quad);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIdxs);
  }

  bindUniformAttribute(m_boundShader, "ProjectionMatrix", m_projectionMatrix);
  bindUniformAttribute(m_boundShader, "ModelViewMatrix", m_transform.matrix());
  bindAttribute(m_boundShader, "Vertex", 3, GL_FLOAT, sizeof(BufferElement), VERTEX_OFFSET);
  bindAttribute(m_boundShader, "TexCoord0", 2, GL_FLOAT, sizeof(BufferElement), UV_OFFSET);
  bindAttribute(m_boundShader, "TexCoord1", 2, GL_FLOAT, sizeof(BufferElement), UV1_OFFSET);
  bindAttribute(m_boundShader, "TexCoord2", 2, GL_FLOAT, sizeof(BufferElement), UV2_OFFSET);
  bindUniformAttribute(m_boundShader, "Tex0", 0);
  bindUniformAttribute(m_boundShader, "Tex1", 1);
  bindUniformAttribute(m_boundShader, "Tex2", 2);
  bindUniformAttribute(m_boundShader, "Tex3", 3);
  bindUniformAttribute(m_boundShader, "Tex4", 4);
  bindUniformAttribute(m_boundShader, "Tex5", 5);
  bindUniformAttribute(m_boundShader, "Tex6", 6);
  bindUniformAttribute(m_boundShader, "GlobalAmbient", Vector4f(Scene::Light::globalAmbient().redF(), Scene::Light::globalAmbient().greenF(), Scene::Light::globalAmbient().blueF(), Scene::Light::globalAmbient().alphaF()));
  m_fbo->activateTextures();

  bool isFirstPass = true;

  while (m_renderedLights < m_lights.size()) {
    loadLights(m_boundShader);

    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLushort*)0);

    if (isFirstPass) {
      isFirstPass = false;
      glEnable(GL_BLEND);
      glDisable(GL_DEPTH_TEST);
      bindUniformAttribute(m_boundShader, "GlobalAmbient", Vector4f(0, 0, 0, 1));
    }
  }

  unbindAttribute(m_boundShader, "Vertex");
  unbindAttribute(m_boundShader, "TexCoord0");
  unbindAttribute(m_boundShader, "TexCoord1");
  unbindAttribute(m_boundShader, "TexCoord2");
  m_fbo->deactivateTextures();

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  m_renderedLights = 0;
}

void GL3::load(Storage::Mesh *mesh)
{
  quint32 bindId;
  glGenBuffers(1, &bindId);
  glBindBuffer(GL_ARRAY_BUFFER, bindId);
  mesh->setBindId(bindId);

  // Make vertices, normals, faces, etc. lists
  QList<Vector3f> vertices;
  QList<Vector3f> normals;
  QList<Vector2f> textureMaps;
  QHash<QString, quint16> verticesNum;
  foreach (QString objectName, mesh->objects()) {
    vertices += mesh->vertices(objectName).toList();
    normals += mesh->normals(objectName).toList();
    textureMaps += mesh->textureMaps(objectName).toList();
    verticesNum.insert(objectName, mesh->vertices(objectName).size());
  }

  // Process vertices and normals
  {
    quint64 size = vertices.size();

    BufferElement* buffer = (BufferElement*) malloc(size * sizeof(BufferElement));
    BufferElement* bufferPtr = buffer;
    QList<Vector3f>::const_iterator i = vertices.constBegin();
    QList<Vector3f>::const_iterator j = normals.constBegin();
    QList<Vector2f>::const_iterator k = textureMaps.constBegin();
    QList<Vector3f>::const_iterator end = vertices.constEnd();
    while (i != end) {
      Vector3f vertex = *i++;
      Vector3f normal = *j++;
      Vector2f textureMap = Vector2f::Zero();
      if (!textureMaps.isEmpty())
        textureMap = *k++;

      // Prepare element
      BufferElement element;
      memcpy(element.vertex, vertex.data(), 3 * sizeof(GLfloat));
      memcpy(element.normal, normal.data(), 3 * sizeof(GLfloat));
      memcpy(element.uvMap, textureMap.data(), 2 * sizeof(GLfloat));

      // Copy element
      memcpy(bufferPtr, &element, sizeof(BufferElement));
      // Iterate
      bufferPtr++;
    }

    // Copy it to GPU
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(BufferElement), buffer, GL_STATIC_DRAW);
    free(buffer);
  }

  // Bind VBO for faces
  glGenBuffers(1, &bindId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bindId);
  m_indices.insert(mesh->bindId(), bindId);

  // Process faces
  {
    // First we sort faces per materials and add objects offset
    QMap<QString, Face> facesPerMaterial;
    quint32 offset = 0;
    foreach (QString objectName, mesh->objects()) {
      QList<Face> faces = mesh->faces(objectName);
      QStringList materials = mesh->faceMaterials(objectName).values();
      QList<Face>::const_iterator i = faces.constBegin();
      QStringList::const_iterator j = materials.constBegin();
      QList<Face>::const_iterator end = faces.constEnd();
      while (i != end) {
        Face face = *i++;
        // Move vertex index by object offset
        if (offset) {
          for (quint16 k = 0; k < face.second.size(); k++)
            face.second[k] += offset;
        }

        QString material = "None";
        if (!materials.isEmpty())
          material = *j++;
        facesPerMaterial[material].first = face.first;
        facesPerMaterial[material].second += face.second;
      }

      offset += verticesNum.value(objectName);
    }

    // Now we make a plan to draw the mesh
    QList<Plan> plans;
    QVector<quint16> idxs;
    quint32 size = 0;
    for (QMap<QString, Face>::const_iterator i = facesPerMaterial.constBegin(); i != facesPerMaterial.constEnd(); i++) {
      Face face = i.value();
      quint32 primitive;
      switch (face.first) {
        case Storage::Mesh::Quads:
          primitive = GL_QUADS;
          break;
        case Storage::Mesh::Triangles:
          primitive = GL_TRIANGLES;
          break;
      }

      idxs += face.second;
      size += face.second.size();

      Plan plan;
      plan.count = face.second.size();
      plan.primitive = primitive;
      plan.offset = size - plan.count;
      plan.materialName = i.key();

      plans << plan;
    }
    m_plans.insert(mesh->bindId(), plans);
    // Pad the array (blocks of 64 bytes)
    quint32 padding = 0;
    if (size % 32)
      padding = 32 - size % 32;

    quint16* indices = (quint16*) malloc((size + padding) * sizeof(quint16));
    memcpy(indices, idxs.constData(), size * sizeof(quint16));
    // Copy the array to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size + padding) * sizeof(quint16), indices, GL_STATIC_DRAW);
    free(indices);
  }
}

void GL3::load(Storage::ShaderProgram *shaderProgram)
{
  if (!shaderProgram || shaderProgram->bindId())
    return;

  shaderProgram->setBindId(glCreateProgram());

  foreach (Storage::Shader* shader, shaderProgram->shaders()) {
    if (!shader->bindId()) {
      // Bind and compile shader
      switch (shader->type()) {
        case Storage::Shader::VertexShader:
          shader->setBindId(glCreateShader(VERTEX_SHADER));
          break;
        case Storage::Shader::FragmentShader:
          shader->setBindId(glCreateShader(FRAGMENT_SHADER));
          break;
        default:
          qWarning("BGE::Rendering::ShaderManager::bindObject(): Shader type not known!");
          break;
      }
      // Prepare input data
      qint32 count;
      qint32* length;
      char** lines = prepareShaderSource(shader->shaderSource(), count, &length);

      // Load source
      glShaderSource(shader->bindId(), count, (const GLchar**) lines, length);
      // Compile shader
      glCompileShader(shader->bindId());

      // Check for errors
      int output;
      glGetShaderiv(shader->bindId(), GL_COMPILE_STATUS, &output);
      if (output == GL_FALSE) {
        glGetShaderiv(shader->bindId(), GL_INFO_LOG_LENGTH, &output);
        char *log = (char*) malloc(output * sizeof(char));
        glGetShaderInfoLog(shader->bindId(), output, 0l, log);
        qDebug("BGE::Rendering::ShaderManager::bindProgram(): Compile failed for shader '%s' with info log:\n%s", shader->path().toUtf8().data(), log);
        free(log);

        shaderProgram->setFailed(true);
        unbind(shaderProgram);
        return;
      }
    }

    // Attach shader
    glAttachShader(shaderProgram->bindId(), shader->bindId());
  }

  // Link program
  glLinkProgram(shaderProgram->bindId());

  // Check for errors.
  int output;
  glGetProgramiv(shaderProgram->bindId(), GL_LINK_STATUS, &output);
  if (output == GL_FALSE) {
    glGetProgramiv(shaderProgram->bindId(), GL_INFO_LOG_LENGTH, &output);
    char *log = (char*) malloc(output * sizeof(char));
    glGetProgramInfoLog(shaderProgram->bindId(), output, 0l, log);
    qDebug("BGE::Rendering::ShaderManager::bindProgram(): Link failed for shader program '%s' with info log:\n%s", shaderProgram->path().toUtf8().data(), log);
    free(log);

    // Cleanup
    shaderProgram->setFailed(true);
    unbind(shaderProgram);
  }
}

void GL3::bindAttribute(Storage::ShaderProgram *shaderProgram, QString name, qint32 size, quint32 type, quint32 stride, quint32 offset)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  name.prepend("in_");
  GLint loc = glGetAttribLocation(shaderProgram->bindId(), name.toAscii().data());
  if (loc != -1) {
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, size, type, GL_FALSE, stride, (void*) offset);
  }
}

void GL3::unbindAttribute(Storage::ShaderProgram *shaderProgram, QString name)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  name.prepend("in_");
  GLint loc = glGetAttribLocation(shaderProgram->bindId(), name.toAscii().data());
  if (loc != -1)
    glDisableVertexAttribArray(loc);
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QVector<float>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1)
    glUniform1fv(loc, values.size(), (GLfloat*) values.data());
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Vector2f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 2 * sizeof(GLfloat));
    foreach (Vector2f value, values)
      memcpy(data, value.data(), 2 * sizeof(float));

    glUniform2fv(loc, values.size(), data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Vector3f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 3 * sizeof(GLfloat));
    foreach (Vector3f value, values)
      memcpy(data, value.data(), 3 * sizeof(float));

    glUniform3fv(loc, values.size(), data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Vector4f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 4 * sizeof(GLfloat));
    foreach (Vector4f value, values)
      memcpy(data, value.data(), 4 * sizeof(float));

    glUniform4fv(loc, values.size(), data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Matrix2f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 4 * sizeof(GLfloat));
    foreach (Matrix2f value, values)
      memcpy(data, value.data(), 4 * sizeof(float));

    glUniformMatrix2fv(loc, values.size(), 0, data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Matrix3f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 9 * sizeof(GLfloat));
    foreach (Matrix3f value, values)
      memcpy(data, value.data(), 9 * sizeof(float));

    glUniformMatrix3fv(loc, values.size(), 0, data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, const QList<Matrix4f>& values)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1) {
    GLfloat *data = (GLfloat*) malloc(values.size() * 16 * sizeof(GLfloat));
    foreach (Matrix4f value, values)
      memcpy(data, value.data(), 16 * sizeof(float));

    glUniformMatrix4fv(loc, values.size(), 0, data);
    free(data);
  }
}

void GL3::bindUniformAttribute(Storage::ShaderProgram *shaderProgram, const QString &name, int value)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  GLint loc = glGetUniformLocation(shaderProgram->bindId(), name.toAscii());
  if (loc != -1)
    glUniform1i(loc, value);
}

void GL3::setMaterial(Storage::Material *material, Storage::ShaderProgram *shaderProgram)
{
  bool deleteAfter = false;
  if (!material) {
    material = new Storage::Material("default");
    deleteAfter = true;
  }

  Vector4f color(material->ambient().redF(), material->ambient().greenF(), material->ambient().blueF(), material->ambient().alphaF());
  bindUniformAttribute(shaderProgram, "Material.ambient", color);

  color = Vector4f(material->diffuse().redF(), material->diffuse().greenF(), material->diffuse().blueF(), material->diffuse().alphaF());
  bindUniformAttribute(shaderProgram, "Material.diffuse", color);

  color = Vector4f(material->specular().redF(), material->specular().greenF(), material->specular().blueF(), material->specular().alphaF());
  bindUniformAttribute(shaderProgram, "Material.specular", color);

  color = Vector4f(material->emission().redF(), material->emission().greenF(), material->emission().blueF(), material->emission().alphaF());
  bindUniformAttribute(shaderProgram, "Material.emission", color);

  bindUniformAttribute(shaderProgram, "Material.shininess", (float) material->shininess());

  if (deleteAfter)
    delete material;
}

char** GL3::prepareShaderSource(const QString &source, qint32 &count, qint32 **length)
{
  QStringList lines = source.split('\n');
  count = lines.count();
  char** output = (char**) malloc(count * sizeof(char*));
  length[0] = (qint32*) malloc(count * sizeof(qint32));

  quint32 i = 0;
  foreach (QString line, lines) {
    length[0][i] = line.toUtf8().size();
    output[i] = (char*) malloc((length[0][i] + 1) * sizeof(char));
    memcpy(output[i], line.toUtf8().data(), length[0][i] + 1);
    i++;
  }

  return output;
}

void GL3::loadLights(Storage::ShaderProgram *shaderProgram)
{
  //quint32 i = 0;
  //foreach (Light light, m_lights) {
  quint8 offset = m_renderedLights;
  if (offset)
    offset--;
  qint32 size = m_lights.size();
  quint8 usedLights = 0;
  for (quint32 i = 0; i < m_maxLights; i++) {
    if (i + offset >= size)
      break;
    Light light = m_lights.at(i + offset);
    bindUniformAttribute(shaderProgram, QString("Lights%0.position").arg(i), light.position);
    bindUniformAttribute(shaderProgram, QString("Lights%0.ambient").arg(i), light.ambient);
    bindUniformAttribute(shaderProgram, QString("Lights%0.diffuse").arg(i), light.diffuse);
    bindUniformAttribute(shaderProgram, QString("Lights%0.specular").arg(i), light.specular);
    bindUniformAttribute(shaderProgram, QString("Lights%0.constant").arg(i), light.constant);
    bindUniformAttribute(shaderProgram, QString("Lights%0.linear").arg(i), light.linear);
    bindUniformAttribute(shaderProgram, QString("Lights%0.quadratic").arg(i), light.quadratic);
    bindUniformAttribute(shaderProgram, QString("Lights%0.spot_cutoff").arg(i), light.spot_cutoff);
    bindUniformAttribute(shaderProgram, QString("Lights%0.spot_exponent").arg(i), light.spot_exponent);
    bindUniformAttribute(shaderProgram, QString("Lights%0.spot_direction").arg(i), light.spot_direction);
    m_renderedLights++;
    usedLights++;
  }
  bindUniformAttribute(shaderProgram, "UsedLights", usedLights);
}
