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

#include "scene/sceneobject.h"
#include "scene/camera.h"

#include "storage/mesh.h"
#include "storage/texture.h"

using namespace BGE;
using namespace BGE::Rendering;

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

  // Let's use the active camera
  Scene::Camera* camera = Canvas::canvas()->activeCamera();
  bool isGlobalCamera = camera && camera->parent() == Canvas::canvas()->scene();

  // Calculate camera transformations
  Transform3f move(Transform3f::Identity()), rotation(Transform3f::Identity());
  if (camera) {
    // Calculate camera translation
    move.translate(-camera->globalPosition());

    // Calculate camera rotation
    if (!isGlobalCamera)
      rotation.translate(-camera->position());
    rotation.rotate(camera->globalOrientation().inverse());
    if (!isGlobalCamera)
      rotation.translate(camera->position());
  }

  while (!m_renderQueue.isEmpty()) {
    Scene::SceneObject* object = m_renderQueue.dequeue();
    // Calculate world transform
    Transform3f worldTransform = rotation * move * object->globalTransform();
    glLoadMatrixf(worldTransform.data());

    // Let's render! :D
    if (object->isBindable()) {
      // Bind the mesh
      if (!object->mesh()->bindId())
        bindMesh(object);
      // Bind the texture
      if (object->texture()) {
        if (!object->texture()->bindId())
          bindTexture(object);

        // Load texture
        glBindTexture(GL_TEXTURE_2D, object->texture()->bindId());
      }

      if (object->texture())
        glEnable(GL_TEXTURE_2D);
      glCallList(object->mesh()->bindId());
      if (object->texture())
        glDisable(GL_TEXTURE_2D);
    }
  }
}

void DirectRenderer::unbindObject(Scene::SceneObject* object)
{
  if (!object->mesh())
    return;

  // Unbind mesh
  if (object->mesh() && object->mesh()->bindId())
    glDeleteLists(object->mesh()->bindId(), 1);
  // Unbind texture
  if (object->texture() && object->texture()->bindId())
    Canvas::canvas()->deleteTexture(object->texture()->bindId());
}

void DirectRenderer::bindTexture(Scene::SceneObject *object)
{
  if (!object->texture())
    return;

  object->texture()->bind(Canvas::canvas()->bindTexture(object->texture()->texture()));
}

void DirectRenderer::bindMesh(Scene::SceneObject* object)
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
