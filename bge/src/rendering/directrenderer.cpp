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

#include <QtCore/QSet>

#include "canvas.h"

#include "scene/sceneobject.h"
#include "scene/camera.h"

using namespace BGE;
using namespace BGE::Rendering;

void DirectRenderer::drawQuads(const QList<Vector3f>& vertices)
{
  glBegin(GL_QUADS);
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
      if (!object->meshId())
        bindMesh(object);
      else
        glCallList(object->meshId());
    } else {
      render(object);
    }
  }
}

void DirectRenderer::bindMesh(Scene::SceneObject* object)
{
  quint32 meshId = glGenLists(1);
  glNewList(meshId, GL_COMPILE_AND_EXECUTE);
  Renderer::bindMesh(object, meshId);
  glEndList();
}

void DirectRenderer::unbindMesh(Scene::SceneObject* object)
{
  if (object->meshId())
    glDeleteLists(object->meshId(), 1);
}
