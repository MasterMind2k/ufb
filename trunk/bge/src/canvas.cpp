/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
 *   Copyright (C) 2010 by Matevž Pesek    <be inserted>                   *
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

#include "abstractcontroller.h"

using namespace BGE;

Canvas* Canvas::m_self = 0l;

Canvas::Canvas()
: QGLWidget()
{
  m_renderer = new Rendering::DirectRenderer;

  QTimer* timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(30);
  timer->start();

  m_scene = new Scene::SceneObject;

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
  perspective.perspective(45, (qreal) w / (qreal) h, 0.1, 1000.0);
  perspective.translate(0, 0, -5);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(perspective.data());
  glMatrixMode(GL_MODELVIEW);
}

void Canvas::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  m_scene->prepareTransforms();

  /* Culling comes here :D */
  QQueue<Scene::SceneObject*> list;
  list.enqueue(m_scene);
  while (!list.isEmpty()) {
    Scene::SceneObject* object = list.dequeue();
    m_renderer->enqueueObject(object);
    list.append(object->childs());
  }

  m_renderer->renderScene();
}

void Canvas::setController(AbstractController* controller)
{
  m_controller = controller;
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
