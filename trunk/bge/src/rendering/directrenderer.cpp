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
#include "rendering/directrenderer.h"

#include <QtCore/QVector>

#include "canvas.h"

#include "scene/object.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/shader.h"
#include "storage/shaderprogram.h"

#include "rendering/shadermanager.h"
#include "rendering/buffermanager.h"

using namespace BGE;
using namespace BGE::Rendering;

DirectRenderer::DirectRenderer()
  : Renderer()
{
  m_shaderManager = new ShaderManager;
  m_bufferManager = BufferManager::init();
}

void DirectRenderer::drawQuads(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps)
{
  bool addTextureMapping = !textureMaps.isEmpty();

  glBegin(GL_QUADS);

  for (quint16 i = 0; i < vertices.size(); i++) {
    Vector3f vertex = vertices.at(i);
    // Set normal vector
    if (!normals.at(i).isZero())
      glNormal3fv(normals.at(i).data());
    // Set texture mapping
    if (addTextureMapping)
      glTexCoord2fv(textureMaps.at(i).data());
    // Set vertex
    glVertex3fv(vertex.data());
  }

  glEnd();
}

void DirectRenderer::drawTriangles(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps)
{
  bool addTextureMapping = !textureMaps.isEmpty();

  glBegin(GL_TRIANGLES);

  for (quint16 i = 0; i < vertices.size(); i++) {
    Vector3f vertex = vertices.at(i);
    // Set normal vector
    if (!normals.at(i).isZero())
      glNormal3fv(normals.at(i).data());
    // Set texture mapping
    if (addTextureMapping)
      glTexCoord2fv(textureMaps.at(i).data());
    // Set vertex
    glVertex3fv(vertex.data());
  }

  glEnd();
}

void DirectRenderer::renderScene()
{
  // Needed for gl commands
  Canvas::canvas()->makeCurrent();

  // Make a temporary copy of lights
  QList<Scene::Light*> lights = Canvas::canvas()->lights();

  // Let's use the active camera
  Scene::Camera* camera = Canvas::canvas()->activeCamera();

  // Calculate camera transformations
  Transform3f move(Transform3f::Identity()), rotation(Transform3f::Identity());
  if (camera) {
    // Calculate camera translation
    move.translate(-camera->globalPosition());

    // Calculate camera rotation
    rotation.rotate(camera->orientation().inverse());

    // And do some fixing for local camera
    if (camera->parent() != Canvas::canvas()->scene()) {
      rotation.translate(-camera->position());
      rotation.rotate(camera->orientation() * camera->globalOrientation().inverse());
      rotation.translate(camera->position());
    }
  }

  while (!m_renderQueue.isEmpty()) {
    Scene::Object* object = m_renderQueue.dequeue();

    // Prepare lighting
    foreach (Scene::Light* light, lights) {
      GLenum lightId = GL_LIGHT0 + assignLight(light);
      if (light->isPositional()) {
       Transform3f transform = rotation * move * light->globalTransform();
       glLoadMatrixf(transform.data());
      }

      // Setup light properties
      // Position
      Vector4f temp(0, 0, 0, 1);
      if (!light->isPositional())
        temp = Vector4f(light->globalPosition().x(), light->globalPosition().y(), light->globalPosition().z(), 0);
      glLightfv(lightId, GL_POSITION, temp.data());
      // Colors
      temp = Vector4f(light->ambientColor().redF(), light->ambientColor().greenF(), light->ambientColor().blueF(), light->ambientColor().alphaF());
      glLightfv(lightId, GL_AMBIENT, temp.data());
      temp = Vector4f(light->diffuseColor().redF(), light->diffuseColor().greenF(), light->diffuseColor().blueF(), light->diffuseColor().alphaF());
      glLightfv(lightId, GL_DIFFUSE, temp.data());
      temp = Vector4f(light->specularColor().redF(), light->specularColor().greenF(), light->specularColor().blueF(), light->specularColor().alphaF());
      glLightfv(lightId, GL_SPECULAR, temp.data());
      // Attenuation
      glLightf(lightId, GL_CONSTANT_ATTENUATION, light->constantAttenuation());
      glLightf(lightId, GL_LINEAR_ATTENUATION, light->linearAttenuation());
      glLightf(lightId, GL_QUADRATIC_ATTENUATION, light->quadraticAttenuation());
      // Spot properties
      if (light->isPositional() && light->isSpot()) {
        glLightf(lightId, GL_SPOT_CUTOFF, light->spotCutOff());
        glLightf(lightId, GL_SPOT_EXPONENT, light->spotExponent());
      } else {
        glLightf(lightId, GL_SPOT_CUTOFF, 180);
      }

      // Enable light
      glEnable(lightId);
    }

    // Calculate world transform
    Transform3f worldTransform = rotation * move * object->globalTransform();
    glLoadMatrixf(worldTransform.data());

    // Bind and use shader
    if (object->shaderProgram()) {
      if (!object->shaderProgram()->bindId())
        m_shaderManager->bindProgram(object->shaderProgram());
      m_shaderManager->useProgram(object->shaderProgram());
    }

    // Bind the mesh
    m_bufferManager->bindObject(object);

    // Bind the texture
    if (object->texture()) {
      if (!object->texture()->bindId())
        bindTexture(object);

      // Load texture
      glBindTexture(GL_TEXTURE_2D, object->texture()->bindId());
    }

    if (object->texture())
      glEnable(GL_TEXTURE_2D);

    m_bufferManager->drawObject(object);

    if (object->texture())
      glDisable(GL_TEXTURE_2D);

    m_bufferManager->unbindObject(object);

    // Disable and clear lights
    quint8 size = assignedLights().size();
    for (quint8 i = 0; i < size; i++)
      glDisable(GL_LIGHT0 + i);
    clearAssignedLights();

    // Unload shader
    if (object->shaderProgram())
      m_shaderManager->unload();
  }
}

void DirectRenderer::unbindObject(Scene::Object* object)
{
  if (!object->mesh())
    return;

  // Unbind mesh
  if (object->mesh() && object->mesh()->bindId())
    m_bufferManager->removeObject(object);
  // Unbind texture
  if (object->texture() && object->texture()->bindId())
    Canvas::canvas()->deleteTexture(object->texture()->bindId());
  // Unbind shader
  if (object->shaderProgram())
    m_shaderManager->unbindProgram(object->shaderProgram());
}

void DirectRenderer::bindTexture(Scene::Object *object)
{
  if (!object->texture())
    return;

  object->texture()->bind(Canvas::canvas()->bindTexture(object->texture()->texture()));
}

void DirectRenderer::bindMesh(Scene::Object* object)
{
  if (!object->mesh())
    return;

  // Create the call list
  quint32 meshId = glGenLists(1);
  glNewList(meshId, GL_COMPILE);
  // Save the id :)
  object->mesh()->bind(meshId);

  // Let's do some deprecated rendring :P
  foreach (QString meshObject, object->mesh()->objects()) {
    // Prepare "the" data
    QVector<Vector3f> vertices = object->mesh()->vertices(meshObject);
    QVector<Vector3f> normals = object->mesh()->normals(meshObject);
    bool hasNormals = !normals.isEmpty();
    QVector<Vector2f> textureMaps = object->mesh()->textureMaps(meshObject);
    bool hasTexturesMapping = !textureMaps.isEmpty();

    foreach (Face face, object->mesh()->faces(meshObject)) {
      // Temporary lists and vectors
      VectorList verticesTemp;
      QVector<Vector2f> textureMapTemp;
      VectorList normalsTemp;

      QVector<quint16> idxs = face.second;
      switch (face.first) {
        // Quads rendering
        case Mesh::Quads:
          foreach (quint16 idx, idxs) {
            // Prepare normal vectors
            if (hasNormals)
              normalsTemp << normals.at(idx);
            // Prepare texture mappings
            if (hasTexturesMapping)
              textureMapTemp << textureMaps.at(idx);
            // Prepare vertices
            verticesTemp << vertices.at(idx);
          }
          // Render
          drawQuads(verticesTemp, normalsTemp, textureMapTemp);
          break;

        // Triangles rendering
        case Mesh::Triangles:
          // Prepare vertices
          verticesTemp << vertices.at(idxs.at(0)) << vertices.at(idxs.at(1)) << vertices.at(idxs.at(2));
          // Prepare texture mappings
          if (hasTexturesMapping)
            textureMapTemp << textureMaps.at(idxs.at(0)) << textureMaps.at(idxs.at(1)) << textureMaps.at(idxs.at(2));
          // Prepare normal vectors
          if (hasNormals)
            normalsTemp << normals.at(idxs.at(0)) << normals.at(idxs.at(1)) << normals.at(idxs.at(2));
          // Render
          drawTriangles(verticesTemp, normalsTemp, textureMapTemp);
          break;

        default:
          break;
      }
    }
  }

  glEndList();
}
