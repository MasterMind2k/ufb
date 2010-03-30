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

#include "canvas.h"

#include "driver/abstractdriver.h"

#include "scene/object.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/shaderprogram.h"

using namespace BGE;
using namespace BGE::Rendering;

void Renderer::enqueueObject(Scene::Object *object)
{
  if (object->isBindable())
    m_renderQueue.enqueue(object);
}

void Renderer::renderScene()
{
  // Let's use the active camera
  Scene::Camera* camera = Canvas::canvas()->activeCamera();

  // Calculate camera transformations
  Transform3f move(Transform3f::Identity()), rotation(Transform3f::Identity());
  if (camera) {
    // Calculate camera translation
    move.translate(-camera->globalPosition());

    // Calculate camera rotation
    rotation *= camera->orientation().inverse();

    // And do some fixing for local camera
    if (camera->parent() != Canvas::canvas()->scene()) {
      rotation.translate(-camera->position());
      rotation.rotate(camera->orientation() * camera->globalOrientation().inverse());
      rotation.translate(camera->position());
    }
  }

  // Prepare lighting
  foreach (Scene::Light* light, Canvas::canvas()->lights()) {
    Transform3f transform = rotation * move * light->globalTransform();
    Driver::AbstractDriver::self()->setTransformMatrix(transform);

    Driver::AbstractDriver::self()->setLight(light);
  }

  while (!m_renderQueue.isEmpty()) {
    Scene::Object* object = m_renderQueue.dequeue();

    // Calculate world transform
    Transform3f worldTransform = rotation * move * object->globalTransform();
    Driver::AbstractDriver::self()->setTransformMatrix(worldTransform);

    if (object->shaderProgram())
      object->shaderProgram()->bind();

    Driver::AbstractDriver::self()->bind(object->materials());

    object->mesh()->bind();
    if (object->texture())
      object->texture()->bind();
    Driver::AbstractDriver::self()->draw(object);
    if (object->texture())
      object->texture()->unbind();
    object->mesh()->unbind();

    if (object->shaderProgram())
      object->shaderProgram()->unbind();
  }

  Driver::AbstractDriver::self()->resetLighting();
}
