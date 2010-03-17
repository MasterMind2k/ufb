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
#ifndef __BGE_RENDERING_DIRECTRENDERER_H
#define __BGE_RENDERING_DIRECTRENDERER_H

#include "global.h"

#include "rendering/renderer.h"

#include <QtCore/QVector>

namespace BGE
{
namespace Rendering
{

/**
 * @short Renderer for old OpenGL API
 *
 * This renderer should support only commands prior to OpenGL 2.1.
 */
class DirectRenderer : public Renderer
{
  public:
    void drawQuads(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps = QVector<Vector2f>());
    void drawTriangles(const VectorList& vertices, const VectorList& normals, const QVector<Vector2f>& textureMaps = QVector<Vector2f>());

    void renderScene();

    void unbindObject(Scene::Object* object);

  private:
    void bindTexture(Scene::Object* object);
    void bindMesh(Scene::Object* object);
};

}
}

#endif
