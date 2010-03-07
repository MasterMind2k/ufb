/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
 *   Copyright (C) 2010 by Matevž Pesek    <be inserted>                   *
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

using namespace BGE;
using namespace BGE::Rendering;

void DirectRenderer::drawRectangle(const QList< Vector3f >& vertices)
{
  glBegin(GL_QUADS);
  foreach(Vector3f vertex, vertices)
    glVertex3f(vertex[0], vertex[1], vertex[2]);
  glEnd();
}

void DirectRenderer::renderScene()
{
  Canvas::canvas()->makeCurrent();

  while (!m_renderQueue.isEmpty()) {
    Scene::SceneObject* object = m_renderQueue.dequeue();
    glLoadMatrixf(object->globalTransform().data());

    object->render(this);
  }
}
