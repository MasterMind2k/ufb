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
#ifndef __BGE_RENDERING_RENDERER_H
#define __BGE_RENDERING_RENDERER_H

class QGLWidget;

#include <QtCore/QQueue>

#include "global.h"

namespace BGE
{
namespace Scene
{
class SceneObject;
}
namespace Rendering
{

class Renderer
{
  public:
    void enqueueObject(Scene::SceneObject* object);

    virtual void drawRectangle(const QList<Vector3f>& vertexes) = 0;

    virtual void renderScene() = 0;

  protected:
    QQueue<Scene::SceneObject*> m_renderQueue;
};

}
}

#endif
