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
#include "rendering/renderer.h"

#include "scene/sceneobject.h"

using namespace BGE;
using namespace BGE::Rendering;

void Renderer::drawRectangle(const Vector3f& bottomLeft, const Vector3f& bottomRight, const Vector3f& upperLeft, const Vector3f& upperRight)
{
  QList<Vector3f> vertices;
  vertices << bottomRight << upperRight << upperLeft << bottomLeft;
  drawQuads(vertices);
}

void Renderer::render(Scene::SceneObject* object)
{
  object->render(this);
}
