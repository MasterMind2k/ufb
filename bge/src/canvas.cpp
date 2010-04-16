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

#include "rendering/renderer.h"

#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QResource>

#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtGui/QMatrix4x4>

#include "driver/abstractdriver.h"

#include "scene/object.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "scene/partition.h"

#include "storage/storagemanager.h"

#include "abstractcontroller.h"
#include "abstractoverlay.h"
#include "recorder.h"

using namespace BGE;

Canvas* Canvas::m_self = 0l;

Canvas::Canvas()
: QGLWidget()
{
  m_frames = 0;
  m_fps = 0;
  m_isFPSShown = false;
  m_totalElapsed = 0;
  m_timeSinceSnap = 0;

  // Setup recording if necessary
  QStringList args = QApplication::instance()->arguments();
  QString recordString = args.value(args.indexOf(QRegExp("record=*", Qt::CaseInsensitive, QRegExp::Wildcard)));
  m_recorder = 0l;
  if (!recordString.isEmpty()) {
    m_recorder = new Recorder(this);
    QDir recordOutputDir;
    recordOutputDir.setPath(recordString.split("=").last());
    m_recorder->setOutput(recordOutputDir);
    m_recorder->start();
    qDebug("BGE::Canvas(): Recording...");
  }
  if (args.contains("drawBV")) {
    m_drawBoundingVolumes = true;
    qDebug("BGE::Canvas(): Bounding volumes are drawn.");
  } else {
    m_drawBoundingVolumes = false;
  }

  m_timer = new QTimer(this);
  m_timer->setSingleShot(false);
  m_timer->setInterval(0);

  m_time = new QTime;
  m_time->start();

  // Initialize scene graph and set active camera to none
  m_scene = new Scene::Object;
  m_activeCamera = 0l;

  m_controller = 0l;
  m_overlay = 0l;
  m_partition = new Scene::Partition(1000, 1000, 1000);

  setAutoFillBackground(false);

  // Needed to "block" any idiotic loading of a idiotic shaders :D
  QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);

  connect(m_timer, SIGNAL(timeout()), SLOT(updateGL()));
  connect(QApplication::instance(), SIGNAL(aboutToQuit()), SLOT(cleanup()));
}

void Canvas::addSceneObject(Scene::Object* object)
{
  m_scene->addChild(object);
  m_partition->addObject(object);
}

Canvas* Canvas::canvas()
{
  if (!m_self)
    m_self = new Canvas;

  return m_self;
}

void Canvas::initializeGL()
{
  // Creating a renderer
  m_renderer = new Rendering::Renderer;

  Driver::AbstractDriver::self()->init();
}

void Canvas::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  // Default perspective setup
  QMatrix4x4 projection;
  // Careful! Values are also used by v-f culling
  projection.perspective(80, (qreal) w / (qreal) h, 0.1, 1000.0);
  Matrix4d temp;
  memcpy(temp.data(), projection.data(), 16 * sizeof(qreal));
  Scene::Camera::m_projection.matrix() = temp.cast<float>();
}

void Canvas::paintGL()
{
  m_timer->stop();

  // Reset the scene
  Driver::AbstractDriver::self()->clear();

  qint32 elapsed = m_time->restart();
  m_totalElapsed += elapsed;
  m_timeSinceSnap += elapsed;

  // Calculate all the transforms (recursive)
  if (elapsed > 0) {
    m_scene->prepareTransforms(elapsed);

    // Calculate list of visible objects
    // Using hard-coded values!
    activeCamera()->calculateFrustrum();
    QQueue<Scene::Partition*> partitionQueue;
    partitionQueue.enqueue(m_partition);
    while (!partitionQueue.isEmpty()) {
      Scene::Partition *partition = partitionQueue.dequeue();

      switch (activeCamera()->isSphereInFrustrum(partition->boundingVolume())) {
      case PartialyInside: {
          Containment cont = activeCamera()->isBoxInFrustrum(partition->boundingVolume());
          if (cont == FullyInside || cont == PartialyInside) {
            partitionQueue.append(partition->partitions().toList());
            QQueue<Scene::Object*> objectQueue;
            objectQueue.append(partition->objects());
            while (!objectQueue.isEmpty()) {
              Scene::Object *object = objectQueue.dequeue();
              if (cont == PartialyInside) {
                if (activeCamera()->isBoxInFrustrum(object->boundingVolume()) == Outside)
                  continue;
              }
              objectQueue.append(object->children());

              // Put object to the rendering queue
              m_renderer->enqueueObject(object);
            }
          }
          break;
        }

      case FullyInside: {
          QQueue<Scene::Partition*> queue;
          queue.enqueue(partition);;
          while (!queue.isEmpty()) {
            Scene::Partition *partition = queue.dequeue();
            QQueue<Scene::Object*> objectQueue;
            objectQueue.append(partition->objects());
            while (!objectQueue.isEmpty()) {
              Scene::Object *object = objectQueue.dequeue();
              objectQueue.append(object->children());

              // Put object to the rendering queue
              m_renderer->enqueueObject(object);
            }
            queue.append(partition->partitions().toList());
          }
          break;
        }
      }
    }

    // Mark the objects as calculated
    QQueue<Scene::Object*> objectQueue;
    objectQueue.enqueue(m_scene);
    while (!objectQueue.isEmpty()) {
      Scene::Object *object = objectQueue.dequeue();
      object->m_transformModified = false;
    }
  }

  // Make the actual rendering
  m_renderer->renderScene();

  // 2D painting
  QPainter painter(this);
  if (m_overlay) {
    painter.save();
    m_overlay->paint(&painter, elapsed);
    painter.restore();
  }

  if (m_isFPSShown) {
    m_frames++;
    if (m_totalElapsed >= 1000)
      updateFPS();

    painter.setPen(Qt::white);
    painter.drawText(width() / 2, height() - 3, "FPS: " + QString::number(m_fps));
  }

  painter.end();

  if (m_recorder && m_timeSinceSnap >= 40) {
    m_timeSinceSnap = 0;
    QImage img = grabFrameBuffer();
    m_recorder->enqueueImage(img);
  }

  // Set next rendering
  m_timer->start();
}

void Canvas::setOverlay(AbstractOverlay *overlay)
{
  overlay->m_canvas = this;
  m_overlay = overlay;
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
  if (fileName.isEmpty()) {
    Storage::StorageManager::self()->load();
    return;
  }

  if (!QResource::registerResource(fileName, "/bge_resources"))
    qWarning("BGE::Canvas::loadResource: Cannot register '%s' resource!", fileName.toAscii().data());
  Storage::StorageManager::self()->load();
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
  m_timer->setSingleShot(true);

  if (m_recorder)
    m_recorder->stop();

  // Delete the scene
  delete m_scene;
  delete m_renderer;

  // Delete the storage
  delete Storage::StorageManager::self();
  delete m_time;

  if (m_recorder)
    m_recorder->wait();
}

void Canvas::updateFPS()
{
  m_fps = ((qreal) m_frames / m_totalElapsed) * 1000.0;
  m_frames = 0;
  m_totalElapsed = 0;
}
