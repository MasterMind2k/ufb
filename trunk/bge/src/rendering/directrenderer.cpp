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

using namespace BGE;
using namespace BGE::Rendering;

void DirectRenderer::drawQuads(const VectorList& vertices, const Vector3f& normal)
{
  glBegin(GL_QUADS);

  if (!normal.isZero())
    glNormal3fv(normal.data());

  foreach(Vector3f vertex, vertices)
    glVertex3fv(vertex.data());

  glEnd();
}

void DirectRenderer::drawTriangles(const VectorList& vertices, const Vector3f& normal)
{
  glBegin(GL_TRIANGLES);

  if (!normal.isZero())
    glNormal3fv(normal.data());

  foreach(Vector3f vertex, vertices)
    glVertex3fv(vertex.data());

  glEnd();
}

void DirectRenderer::renderScene()
{
  // Needed for gl commands
  Canvas::canvas()->makeCurrent();

  // Let's use the active camera
  Scene::Camera* camera = Canvas::canvas()->activeCamera();
  bool isGlobalCamera = camera && camera->parent() == Canvas::canvas()->scene();

  while (!m_renderQueue.isEmpty()) {
    Scene::SceneObject* object = m_renderQueue.dequeue();
    Transform3f worldTransform = object->globalTransform();
    if (camera) {
      // The translation matrix
      Transform3f move(Transform3f::Identity());
      move.translate(-camera->globalPosition());

      // Let's rotate our camera
      Transform3f rotation(Transform3f::Identity());
      if (!isGlobalCamera)
        rotation.translate(-camera->position());
      rotation.rotate(camera->globalOrientation().inverse());
      if (!isGlobalCamera)
        rotation.translate(camera->position());

      // Calculated world transform
      worldTransform = rotation * move * worldTransform;
    }
    glLoadMatrixf(worldTransform.data());

    // Let's render! :D
    if (object->isBindable()) {
      if (!object->mesh()->bindId())
        bindObject(object);
      else
        glCallList(object->mesh()->bindId());
    } else {
      render(object);
    }
  }
}

void DirectRenderer::unbindMesh(Scene::SceneObject* object)
{
  if (!object->mesh())
    return;

  if (object->mesh()->bindId())
    glDeleteLists(object->mesh()->bindId(), 1);
}

void DirectRenderer::bindObject(Scene::SceneObject* object)
{
  // Bind the mesh
  quint32 meshId = glGenLists(1);
  glNewList(meshId, GL_COMPILE_AND_EXECUTE);
  object->mesh()->bind(meshId);

  foreach (QString meshObject, object->mesh()->objects()) {
    QVector<Vector3f> vertices = object->mesh()->vertices(meshObject);
    QVector<Vector3f> normals = object->mesh()->normals(meshObject);

    quint16 i = 0;
    foreach (Face face, object->mesh()->faces(meshObject)) {
      VectorList temp;
      QVector<quint16> idxs = face.second;
      switch (face.first) {
        case Mesh::Quads:
          foreach (quint16 idx, idxs)
            temp << vertices.at(idx);
          drawQuads(temp, normals.at(i));
          break;

        case Mesh::Triangles:
          temp << vertices.at(idxs.at(0)) << vertices.at(idxs.at(1)) << vertices.at(idxs.at(2));
          drawTriangles(temp, normals.at(i));
          break;

        default:
          break;
      }

      i++;
    }
  }

  glEndList();
}
