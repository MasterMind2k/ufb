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
     * A convenient method for drawing rectangles.
     */
    void drawRectangle(const Vector3f& bottomLeft, const Vector3f& bottomRight, const Vector3f& upperLeft, const Vector3f& upperRight);

    /**
     * Method for drawing quads.
     */
    virtual void drawQuads(const VectorList& vertexes) = 0;
    /**
     * Method for drawing triangles.
     */
    virtual void drawTriangles(const VectorList& vertexes) = 0;

    /**
     * Method for making the whole scene rendering.
     */
    virtual void renderScene() = 0;

    /**
     * Unbinds the mesh.
     */
    virtual void unbindMesh(Scene::SceneObject* object) = 0;

  protected:
    /**
     * A "redirect" method for accessing the render method of the Scene::SceneObject.
     */
    void render(Scene::SceneObject* object);
    /**
     * High-level binding of mesh.
     */
    void bindMesh(Mesh* mesh);

    /**
     * Queue to be used by implementations.
     */
    QQueue<Scene::SceneObject*> m_renderQueue;
};

}
}

#endif
