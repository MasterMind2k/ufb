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
class Object;
class Light;
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
    void enqueueObject(Scene::Object* object);

    /**
     * Method for making the whole scene rendering.
     */
    virtual void renderScene() = 0;

    /**
     * Unbinds the object.
     */
    virtual void unbindObject(Scene::Object* object) = 0;

    virtual quint8 assignLight(Scene::Light* light)
    {
      m_assignedLights.append(light);
      return m_assignedLights.size() - 1;
    }
    virtual const QList<Scene::Light*>& assignedLights() const
    {
      return m_assignedLights;
    }
    virtual void clearAssignedLights()
    {
      m_assignedLights.clear();
    }

  protected:
    /**
     * Binds the texture that is associated with the object.
     */
    virtual void bindTexture(Scene::Object* object) = 0;
    /**
     * Binds the mesh that is associated with the object.
     */
    virtual void bindMesh(Scene::Object* object) = 0;

    /**
     * Queue to be used by implementations.
     */
    QQueue<Scene::Object*> m_renderQueue;

  private:
    QList<Scene::Light*> m_assignedLights;
};

}
}

#endif
