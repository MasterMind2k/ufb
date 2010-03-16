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
#include "canvas.h"

#include "rendering/directrenderer.h"

#include <QtCore/QTimer>
#include <QtCore/QResource>

#include <QtGui/QKeyEvent>
#include <QtGui/QMatrix4x4>
#include <QtGui/QApplication>

#include "scene/sceneobject.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "storage/storage.h"

#include "abstractcontroller.h"

using namespace BGE;

Canvas* Canvas::m_self = 0l;

Canvas::Canvas()
: QGLWidget()
{
  m_frames = 0;
  m_fps = 0;

  // Creating a renderer
  m_renderer = new Rendering::DirectRenderer;

  // Setting up a timer for "game loop"
  QTimer* timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(30);
  timer->start();

  QTimer* fps = new QTimer(this);
  fps->setSingleShot(false);
  fps->setInterval(1000);
  fps->start();

  // Initialize scene graph and set active camera to none
  m_scene = new Scene::SceneObject;
  m_activeCamera = 0l;

  connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
  connect(fps, SIGNAL(timeout()), SLOT(updateFPS()));
  connect(QApplication::instance(), SIGNAL(aboutToQuit()), SLOT(cleanup()));
}

void Canvas::addSceneObject(Scene::SceneObject* object)
{
  m_scene->addChild(object);
}

Canvas* Canvas::canvas()
{
  if (!m_self)
    m_self = new Canvas;

  return m_self;
}

void Canvas::initializeGL()
{
  // This initialization method could/should be moved to Rendering API
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);
}

void Canvas::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  // Default perspective setup
  QMatrix4x4 perspective;
  perspective.perspective(80, (qreal) w / (qreal) h, 0.1, 1000.0);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(perspective.data());
  glMatrixMode(GL_MODELVIEW);
}

void Canvas::paintGL()
{
  // Reset the matrix and other stuff
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Calculate all the transforms (recursive)
  m_scene->prepareTransforms();

  // Enqueue objects for rendering
  /// @TODO Culling comes here somewhere :D (someday)
  QQueue<Scene::SceneObject*> list;
  list.enqueue(m_scene);
  while (!list.isEmpty()) {
    Scene::SceneObject* object = list.dequeue();
    m_renderer->enqueueObject(object);
    list.append(object->children());

    // Mark the object with calculatd transforms
    object->m_transformModified = false;
  }

  // Make the actual rendering
  m_renderer->renderScene();

  m_frames++;
}

void Canvas::setController(AbstractController* controller)
{
  m_controller = controller;
}

Scene::Camera* Canvas::createCamera(const QString &name)
{
  if (m_cameras.contains(name))
    return 0l;

  Scene::Camera* camera = new Scene::Camera(name);
  if (!camera)
    return 0l;

  m_cameras.insert(name, camera);
  return camera;
}

bool Canvas::activateCamera(const QString& name)
{
  if (!m_cameras.contains(name))
    return false;

  m_activeCamera = m_cameras.value(name);
  return true;
}

bool Canvas::removeCamera(const QString& name)
{
  if (!m_cameras.contains(name))
    return false;

  Scene::Camera* camera = m_cameras.value(name);
  if (m_activeCamera == camera)
    return false;

  m_cameras.remove(name);
  delete camera;
}

Scene::Light* Canvas::createLight(const QString& name)
{
  if (m_lights.contains(name))
    return 0l;

  Scene::Light* light = new Scene::Light(name);
  if (!light)
    return 0l;

  m_lights.insert(name, light);
  return light;
}

bool Canvas::removeLight(const QString &name)
{
  Scene::Light* light = m_lights.value(name);
  if (!light)
    return false;

  delete light;
  m_lights.remove(name);

  return true;
}

void Canvas::loadResource(const QString& fileName)
{
  if (!QResource::registerResource(fileName, "/bge_resources"))
    qWarning("BGE::Canvas::loadResource: Cannot register '%s' resource!", fileName.toAscii().data());
  Storage::self()->load();
  if (!QResource::unregisterResource(fileName, "/bge_resources"))
    qWarning("BGE::Canvas::loadResource: Cannot unregister '%s' resource!", fileName.toAscii().data());
}

void Canvas::keyPressEvent(QKeyEvent* event)
{
  if (m_controller)
    m_controller->keyPressed(event);

  QWidget::keyPressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
  if (m_controller)
    m_controller->mouseMoved(event);
  QWidget::mouseMoveEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
  if (m_controller)
    m_controller->mouseButtonPressed(event);
  QWidget::mousePressEvent(event);
}

void Canvas::cleanup()
{
  // Let's unbind our meshes, textures, etc.
  QQueue<Scene::SceneObject*> queue;
  queue.enqueue(m_scene);
  while (!queue.isEmpty()) {
    Scene::SceneObject* object = queue.dequeue();
    m_renderer->unbindObject(object);
    queue.append(object->children());
  }

  // And delete the scene
  delete m_scene;
  delete m_renderer;
}

void Canvas::updateFPS()
{
  m_fps = m_frames;
  m_frames = 0;
  qDebug("FPS: %lld", m_fps);
}
