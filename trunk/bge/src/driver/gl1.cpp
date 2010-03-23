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

#include "gl1.h"

#include "canvas.h"

#include "scene/object.h"
#include "scene/light.h"

#include "storage/mesh.h"
#include "storage/texture.h"

using namespace BGE;
using namespace BGE::Driver;

void drawQuads(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps)
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

void drawTriangles(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps)
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

GL1::GL1()
{
  m_usedLights = 0;
}

void GL1::bind(Storage::Mesh *mesh)
{
  // Return if mesh is already created
  if (!mesh || mesh->bindId())
    return;

  // Create the call list
  quint32 meshId = glGenLists(1);
  glNewList(meshId, GL_COMPILE);
  // Save the id :)
  mesh->setBindId(meshId);

  // Rendering stuff
  foreach (QString meshObject, mesh->objects()) {
    // Prepare data
    QVector<Vector3f> vertices = mesh->vertices(meshObject);
    QVector<Vector3f> normals = mesh->normals(meshObject);
    QVector<Vector2f> textureMaps = mesh->textureMaps(meshObject);
    bool hasTexturesMapping = !textureMaps.isEmpty();

    foreach (Face face, mesh->faces(meshObject)) {
      // Temporary lists and vectors
      VectorList verticesTemp;
      QVector<Vector2f> textureMapTemp;
      VectorList normalsTemp;

      QVector<quint16> idxs = face.second;
      switch (face.first) {
        // Quads rendering
        case Storage::Mesh::Quads:
          foreach (quint16 idx, idxs) {
            // Prepare vertices
            verticesTemp << vertices.at(idx);
            // Prepare normal vectors
            normalsTemp << normals.at(idx);
            // Prepare texture mappings
            if (hasTexturesMapping)
              textureMapTemp << textureMaps.at(idx);
          }
          // Render
          drawQuads(verticesTemp, normalsTemp, textureMapTemp);
          break;

        // Triangles rendering
        case Storage::Mesh::Triangles:
          foreach (quint16 idx, idxs) {
            // Prepare vertices
            verticesTemp << vertices.at(idx);
            // Prepare normal vectors
            normalsTemp << normals.at(idx);
            // Prepare texture mappings
            if (hasTexturesMapping)
              textureMapTemp << textureMaps.at(idx);
          }
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

void GL1::bind(Storage::Texture* texture)
{
  if (!texture)
    return;

  if (!texture->bindId())
    texture->setBindId(Canvas::canvas()->bindTexture(texture->texture()));

  if (texture->bindId())
    glBindTexture(GL_TEXTURE_2D, texture->bindId());
}

void GL1::unload(Storage::Mesh* mesh)
{
  if (!mesh || !mesh->bindId())
    return;

  glDeleteLists(mesh->bindId(), 1);
  mesh->setBindId(0);
}

void GL1::setLight(Scene::Light *light)
{
  if (m_usedLights >= GL_MAX_LIGHTS) {
    qWarning("BGE::Driver::GL1::setLight(): Number of lights exeeds max value (%d)!", GL_MAX_LIGHTS);
    return;
  }
  GLenum lightId = GL_LIGHT0 + m_usedLights++;

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

void GL1::resetLighting()
{
  for (quint8 i = 0; i < m_usedLights; i++)
    glDisable(GL_LIGHT0 + i);

  m_usedLights = 0;
}

void GL1::unload(Storage::Texture* texture)
{
  if (!texture || !texture->bindId())
    return;

  Canvas::canvas()->deleteTexture(texture->bindId());
  texture->setBindId(0);
}

void GL1::setTransformMatrix(const Transform3f& transform)
{
  glLoadMatrixf(transform.data());
}

void GL1::draw(Scene::Object* object)
{
  bool hasTexture = object->texture() && object->texture()->bindId();
  if (hasTexture)
    glEnable(GL_TEXTURE_2D);

  glCallList(object->mesh()->bindId());

  if (hasTexture)
    glDisable(GL_TEXTURE_2D);
}
