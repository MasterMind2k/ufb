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
#ifndef __BGE_CANVAS_H
#define __BGE_CANVAS_H

#include <QtCore/QHash>

#include <QtOpenGL/QGLWidget>

namespace BGE
{

class AbstractController;
namespace Scene
{
class SceneObject;
class Camera;
}
namespace Rendering
{
class Renderer;
}

class Canvas : public QGLWidget
{
  Q_OBJECT
  public:
    void addSceneObject(Scene::SceneObject* object);
    inline Scene::SceneObject* scene() const
    {
      return m_scene;
    }

    static Canvas* canvas();

    void setController(AbstractController* controller);

    Scene::Camera* createCamera(const QString& name);
    inline Scene::Camera* camera(const QString& name) const
    {
      return m_cameras.value(name);
    }
    bool activateCamera(const QString& name);
    bool removeCamera(const QString& name);
    inline Scene::Camera* activeCamera()
    {
      return m_activeCamera;
    }

  protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

  private:
    Canvas();

    Scene::SceneObject* m_scene;
    Rendering::Renderer* m_renderer;
    AbstractController* m_controller;

    QHash<QString, Scene::Camera*> m_cameras;
    Scene::Camera* m_activeCamera;

    static Canvas* m_self;
};

}

#endif
