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
  /*Scene::Camera* camera = Canvas::canvas()->activeCamera();
  if (!camera)
    qFatal("BGE::Rendering::Renderer::renderScene(): No active camera defined!");*/

  Transform3f cameraTransform(Transform3f::Identity());
  cameraTransform.rotate(AngleAxisf(M_PI/2, Vector3f::UnitX()));
  cameraTransform.translate(Vector3f(0, -500, 0));
  //cameraTransform = Canvas::canvas()->activeCamera()->cameraTransform();

  // Set projection matrix
  Driver::AbstractDriver::self()->setProjection(Scene::Camera::projection());

  // Prepare lighting
  foreach (Scene::Light* light, Canvas::canvas()->lights()) {
    Transform3f transform = cameraTransform * light->globalTransform();
    Driver::AbstractDriver::self()->setTransformMatrix(transform);

    Driver::AbstractDriver::self()->setLight(light);
  }

  while (!m_renderQueue.isEmpty()) {
    Scene::Object* object = m_renderQueue.dequeue();

    // Calculate world transform
    Transform3f worldTransform = cameraTransform * object->globalTransform();
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
