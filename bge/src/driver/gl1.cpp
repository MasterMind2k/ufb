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

#include <QtGui/QMatrix4x4>

#include "canvas.h"

#include "scene/object.h"
#include "scene/boundingvolume.h"
#include "scene/light.h"

#include "storage/mesh.h"
#include "storage/material.h"
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

void drawPolygon(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps)
{
  bool addTextureMapping = !textureMaps.isEmpty();

  glBegin(GL_POLYGON);

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

void setMaterial(Storage::Material* material)
{
  if (!material)
    return;

  Vector4f color(material->ambient().redF(), material->ambient().greenF(), material->ambient().blueF(), material->ambient().alphaF());
  glMaterialfv(GL_FRONT, GL_AMBIENT, color.data());

  color = Vector4f(material->diffuse().redF(), material->diffuse().greenF(), material->diffuse().blueF(), material->diffuse().alphaF());
  glMaterialfv(GL_FRONT, GL_DIFFUSE, color.data());

  color = Vector4f(material->specular().redF(), material->specular().greenF(), material->specular().blueF(), material->specular().alphaF());
  glMaterialfv(GL_FRONT, GL_SPECULAR, color.data());

  color = Vector4f(material->emission().redF(), material->emission().greenF(), material->emission().blueF(), material->emission().alphaF());
  glMaterialfv(GL_FRONT, GL_EMISSION, color.data());

  glMateriali(GL_FRONT, GL_SHININESS, material->shininess());
}

GL1::GL1()
{
  m_usedLights = 0;
  m_renderedLights = 0;
  m_boundMesh = 0l;
  m_hasTexture = false;
}

void GL1::bind(Storage::Mesh *mesh)
{
  // Return if mesh is already created
  if (!mesh)
    return;

  m_boundMesh = mesh;

  Storage::Material* firstMaterial = 0l;
  if (!m_materials.isEmpty())
    firstMaterial = m_materials.values().first();
  quint32 meshId;
  if (firstMaterial)
    meshId = m_meshBindings.value(QPair<quint32, QString> (mesh->bindId(), firstMaterial->name()), 0);
  else
    meshId = mesh->bindId();
  if (meshId)
    return;

  // Create the call list
  meshId = glGenLists(1);
  glNewList(meshId, GL_COMPILE);
  // Save the id :)
  if (!mesh->bindId())
    mesh->setBindId(meshId);
  if (m_materials.isEmpty())
    m_meshBindings.insert(QPair<quint32, QString> (mesh->bindId(), "none"), meshId);
  else
    m_meshBindings.insert(QPair<quint32, QString> (mesh->bindId(), firstMaterial->name()), meshId);

  Storage::Material *defaultMaterial = new Storage::Material("default");

  // Rendering stuff
  foreach (QString meshObject, mesh->objects()) {
    // Prepare data
    QVector<Vector3f> vertices = mesh->vertices(meshObject);
    QVector<Vector3f> normals = mesh->normals(meshObject);
    QVector<Vector2f> textureMaps = mesh->textureMaps(meshObject);
    FaceMaterial materials = mesh->faceMaterials(meshObject);
    bool hasTexturesMapping = !textureMaps.isEmpty();

    quint16 i = 0;
    foreach (Face face, mesh->faces(meshObject)) {
      // Temporary lists and vectors
      VectorList verticesTemp;
      QVector<Vector2f> textureMapTemp;
      VectorList normalsTemp;

      // Setup material
      setMaterial(m_materials.value(materials.value(i++), defaultMaterial));

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

  delete defaultMaterial;

  if (Canvas::canvas()->drawBoundingVolumes())
    draw(mesh->boundingVolume());

  glEndList();
}

void GL1::bind(Storage::Texture* texture)
{
  if (!texture)
    return;

  if (!texture->bindId())
    texture->setBindId(Canvas::canvas()->bindTexture(texture->texture()));

  if (texture->bindId()) {
    glBindTexture(GL_TEXTURE_2D, texture->bindId());
    m_hasTexture = true;
  }
}

void GL1::unbind(Storage::Texture *texture)
{
  Q_UNUSED(texture);
  glBindTexture(GL_TEXTURE2, 0);
  m_hasTexture = false;
}

void GL1::unload(Storage::Mesh* mesh)
{
  if (!mesh || !mesh->bindId())
    return;

  glDeleteLists(mesh->bindId(), 1);
  mesh->setBindId(0);
}

void GL1::unload(Storage::Texture* texture)
{
  if (!texture || !texture->bindId())
    return;

  Canvas::canvas()->deleteTexture(texture->bindId());
  texture->setBindId(0);
}

void GL1::setLight(Scene::Light *light)
{
  m_lights << light;
  m_lightTransforms << m_transform;
}

void GL1::resetLighting()
{
  m_lights.clear();
  m_lightTransforms.clear();
}

void GL1::setTransformMatrix(const Transform3f& transform)
{
  glLoadMatrixf(transform.data());
  m_transform = transform;
}

void GL1::draw()
{
  if (m_hasTexture)
    glEnable(GL_TEXTURE_2D);

  Storage::Material* firstMaterial = 0l;
  if (!m_materials.isEmpty())
    firstMaterial = m_materials.values().first();

  quint32 meshId;
  if (firstMaterial)
    meshId = m_meshBindings.value(QPair<quint32, QString> (m_boundMesh->bindId(), firstMaterial->name()));
  else
    meshId = m_boundMesh->bindId();

  bool isFirstPass = true;
  bool isBlending = false;

  while (m_renderedLights < m_lights.size()) {
    if (!isFirstPass)
      loadLights();

    glCallList(meshId);

    if (!isFirstPass)
      unloadLights();

    if (isFirstPass && !isBlending) {
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
      isFirstPass = false;
      isBlending = true;
    }
  }

  if (m_hasTexture)
    glDisable(GL_TEXTURE_2D);

  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);

  m_renderedLights = 0;
}

void GL1::init()
{
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);

  Vector4f black(0, 0, 0, 1);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black.data());
}

void GL1::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GL1::setProjection(const Transform3f &projection)
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection.data());
  glMatrixMode(GL_MODELVIEW);
}

void GL1::loadLights()
{
  quint32 offset = 0;
  if (m_renderedLights)
    offset = m_renderedLights;
  for (quint32 i = 0; i < 8; i++) {
    if (i + offset >= m_lights.size())
      break;

    Scene::Light *light = m_lights.at(i + offset);

    glPushMatrix();
    glLoadMatrixf(m_lightTransforms.at(i + offset).data());

    GLenum lightId = GL_LIGHT0 + i;

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
      glLightfv(lightId, GL_SPOT_DIRECTION, Vector3f(0, 0, -1).data());
      glLightf(lightId, GL_SPOT_CUTOFF, light->spotCutOff());
      glLightf(lightId, GL_SPOT_EXPONENT, light->spotExponent());
    } else {
      glLightf(lightId, GL_SPOT_CUTOFF, 180);
    }

    // Enable light
    glEnable(lightId);

    m_renderedLights++;
    m_usedLights++;

    glPopMatrix();
  }
}

void GL1::unloadLights()
{
  for (quint8 i = 0; i < m_usedLights; i++)
    glDisable(GL_LIGHT0 + i);

  m_usedLights = 0;
}

void GL1::draw(const Scene::BoundingVolume *boundingVolume)
{
  // Set material
  Storage::Material* white = new Storage::Material("white");
  white->setEmission(QColor(255, 255, 255));
  setMaterial(white);
  delete white;

  glDisable(GL_LIGHTING);
  // Draw front
  glBegin(GL_LINE_STRIP);
  glVertex3fv(boundingVolume->corners().at(0b000).data());
  glVertex3fv(boundingVolume->corners().at(0b001).data());
  glVertex3fv(boundingVolume->corners().at(0b011).data());
  glVertex3fv(boundingVolume->corners().at(0b010).data());
  glVertex3fv(boundingVolume->corners().at(0b000).data());
  glEnd();

  // Draw behind
  glBegin(GL_LINE_STRIP);
  glVertex3fv(boundingVolume->corners().at(0b100).data());
  glVertex3fv(boundingVolume->corners().at(0b101).data());
  glVertex3fv(boundingVolume->corners().at(0b111).data());
  glVertex3fv(boundingVolume->corners().at(0b110).data());
  glVertex3fv(boundingVolume->corners().at(0b100).data());
  glEnd();

  // Draw side lines
  glBegin(GL_LINES);
  glVertex3fv(boundingVolume->corners().at(0b000).data());
  glVertex3fv(boundingVolume->corners().at(0b100).data());
  glVertex3fv(boundingVolume->corners().at(0b001).data());
  glVertex3fv(boundingVolume->corners().at(0b101).data());
  glVertex3fv(boundingVolume->corners().at(0b011).data());
  glVertex3fv(boundingVolume->corners().at(0b111).data());
  glVertex3fv(boundingVolume->corners().at(0b010).data());
  glVertex3fv(boundingVolume->corners().at(0b110).data());
  glEnd();

  glEnable(GL_LIGHTING);
}
