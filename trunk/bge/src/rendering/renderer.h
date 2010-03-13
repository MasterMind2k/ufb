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

namespace BGE
{
class Mesh;
namespace Scene
{
class SceneObject;
}
namespace Rendering
{

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
    inline void enqueueObject(Scene::SceneObject* object)
    {
      m_renderQueue.enqueue(object);
    }

    /**
     * Method for making the whole scene rendering.
     */
    virtual void renderScene() = 0;

    /**
     * Unbinds the object.
     */
    virtual void unbindObject(Scene::SceneObject* object) = 0;

  protected:
    /**
     * Binds the texture that is associated with the object.
     */
    virtual void bindTexture(Scene::SceneObject* object) = 0;
    /**
     * Binds the mesh that is associated with the object.
     */
    virtual void bindMesh(Scene::SceneObject* object) = 0;

    /**
     * Queue to be used by implementations.
     */
    QQueue<Scene::SceneObject*> m_renderQueue;
};

}
}

#endif
