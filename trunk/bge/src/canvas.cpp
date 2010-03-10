/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
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

#include <QtGui/QKeyEvent>
#include <QtGui/QMatrix4x4>

#include "scene/sceneobject.h"
#include "scene/camera.h"

#include "abstractcontroller.h"

using namespace BGE;

Canvas* Canvas::m_self = 0l;

Canvas::Canvas()
: QGLWidget()
{
  // Creating a renderer
  m_renderer = new Rendering::DirectRenderer;

  // Setting up a timer for "game loop"
  QTimer* timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(30);
  timer->start();

  // Initialize scene graph and set active camera to none
  m_scene = new Scene::SceneObject;
  m_activeCamera = 0l;

  connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
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
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
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
    list.append(object->childs());
  }

  // Make the actual rendering
  m_renderer->renderScene();
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
