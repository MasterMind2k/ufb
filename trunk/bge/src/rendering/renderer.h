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
#ifndef __BGE_RENDERING_RENDERER_H
#define __BGE_RENDERING_RENDERER_H

class QGLWidget;

#include <QtCore/QQueue>

#include "global.h"

namespace BGE {
class Mesh;
namespace Scene {
class Object;
class Light;
}
namespace Rendering {

/**
 * @short Abstract renderer
 *
 * It's an abstract interface to the OpenGL rendering.
 */
class Renderer
{
  public:
    /**
     * Enqueues an object to the rendering queue.
     */
    void enqueueObject(Scene::Object* object);

    /**
     * Method for making the whole scene rendering.
     */
    void renderScene();

  private:
    QQueue<Scene::Object*> m_renderQueue;

    void drawScene();
};

}
}

#endif
