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
#include "scene/particleemitter.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/shaderprogram.h"

using namespace BGE;
using namespace BGE::Rendering;

bool objectComperator(Scene::Object *object1, Scene::Object *object2)
{
  return object2->mesh() < object1->mesh();
}

void Renderer::enqueueObject(Scene::Object *object)
{
  m_renderQueue.enqueue(object);
}

void Renderer::renderScene()
{
  // Sort the list
  qSort(m_renderQueue.begin(), m_renderQueue.end(), objectComperator);

  // Let's use the active camera
  if (!Canvas::canvas()->activeCamera())
    qFatal("BGE::Rendering::Renderer::renderScene(): No active camera defined!");

  // Set projection matrix
  Driver::AbstractDriver::self()->setProjection(Scene::Camera::projection());

  // Prepare lighting
  foreach (Scene::Light* light, Canvas::canvas()->lights()) {
    Transform3f transform = Canvas::canvas()->activeCamera()->cameraTransform() * light->globalTransform();
    Driver::AbstractDriver::self()->setTransformMatrix(transform);

    Driver::AbstractDriver::self()->setLight(light);
  }

  // Geometry render
  Driver::AbstractDriver::self()->bindFBO();
  Driver::AbstractDriver::self()->bind(Storage::Manager::self()->get<Storage::ShaderProgram*>("/shaders/FirstStage")); // FirstStage
  drawScene();
  Driver::AbstractDriver::self()->unbind(Storage::Manager::self()->get<Storage::ShaderProgram*>("/shaders/FirstStage"));
  Driver::AbstractDriver::self()->unbindFBO();

  // Shading stage(s)
  Driver::AbstractDriver::self()->shading();

  Driver::AbstractDriver::self()->resetLighting();
}

void Renderer::drawScene()
{
  Storage::Mesh *currentMesh = 0l;
  Storage::Texture *currentTexture = 0l;

  while (!m_renderQueue.isEmpty()) {
    Scene::Object* object = m_renderQueue.dequeue();

    // Calculate world transform
    Transform3f worldTransform = Canvas::canvas()->activeCamera()->cameraTransform();

    // Toggle lighting
    if (object->isCulled())
      Driver::AbstractDriver::self()->enableLighting();
    else
      Driver::AbstractDriver::self()->disableLighting();

    Driver::AbstractDriver::self()->bind(object->materials());

    if (currentMesh != object->mesh()) {
      if (currentMesh)
        currentMesh->unbind();

      if (object->mesh())
        object->mesh()->bind();
      currentMesh = object->mesh();
    }

    if (currentTexture != object->texture()) {
      if (currentTexture)
        currentTexture->unbind();

      if (object->texture())
        object->texture()->bind();
      currentTexture = object->texture();
    }

    if (object->mesh()) {
      Driver::AbstractDriver::self()->setTransformMatrix(worldTransform * object->globalTransform());

      Driver::AbstractDriver::self()->draw();
    } else {
      Driver::AbstractDriver::self()->setTransformMatrix(worldTransform);

      Driver::AbstractDriver::self()->draw(static_cast<Scene::ParticleEmitter*> (object));
    }
  }

  if (currentMesh)
    currentMesh->unbind();
  if (currentTexture)
    currentTexture->unbind();
}
