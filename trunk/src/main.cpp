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
#include <QtGui/QApplication>

#include "canvas.h"
#include "abstractcontroller.h"

#include "rendering/renderer.h"

#include "scene/sceneobject.h"

// Our little testing cube :D
class Cube : public BGE::Scene::SceneObject
{
  public:
    Cube(float a)
    {
      float half = a/2;
      m_vertices << Vector3f(-half, -half, -half);
      m_vertices << Vector3f(-half, half, -half);
      m_vertices << Vector3f(half, half, -half);
      m_vertices << Vector3f(half, -half, -half);

      m_vertices << Vector3f(-half, -half, half);
      m_vertices << Vector3f(-half, half, half);
      m_vertices << Vector3f(half, half, half);
      m_vertices << Vector3f(half, -half, half);

      m_vertices << Vector3f(-half, half, -half);
      m_vertices << Vector3f(-half, half, half);
      m_vertices << Vector3f(half, half, half);
      m_vertices << Vector3f(half, half, -half);

      m_vertices << Vector3f(half, -half, -half);
      m_vertices << Vector3f(half, -half, half);
      m_vertices << Vector3f(-half, -half, half);
      m_vertices << Vector3f(-half, -half, -half);

      m_vertices << Vector3f(half, -half, -half);
      m_vertices << Vector3f(half, -half, half);
      m_vertices << Vector3f(half, half, half);
      m_vertices << Vector3f(half, half, -half);

      m_vertices << Vector3f(-half, half, -half);
      m_vertices << Vector3f(-half, half, half);
      m_vertices << Vector3f(-half, -half, half);
      m_vertices << Vector3f(-half, -half, -half);
    }

  protected:
    void render(BGE::Rendering::Renderer* renderer)
    {
      renderer->drawRectangle(m_vertices.mid(0, 4));
      renderer->drawRectangle(m_vertices.mid(4, 4));
      renderer->drawRectangle(m_vertices.mid(8, 4));
      renderer->drawRectangle(m_vertices.mid(12, 4));
      renderer->drawRectangle(m_vertices.mid(16, 4));
      renderer->drawRectangle(m_vertices.mid(20, 4));
    }

  private:
    QList<Vector3f> m_vertices;
};

// Our little testing controller :)
class Controller : public BGE::AbstractController
{
  public:
    Controller(BGE::Scene::SceneObject* controlled)
    {
      m_controlled = controlled;
    }

    void keyPressed(QKeyEvent* event)
    {
      switch (event->key()) {
        case Qt::Key_Right:
          m_controlled->move(Vector3f(0.1, 0, 0));
          break;
        case Qt::Key_Left:
          m_controlled->move(Vector3f(-0.1, 0, 0));
          break;
        case Qt::Key_Up:
          m_controlled->move(Vector3f(0, 0.1, 0));
          break;
        case Qt::Key_Down:
          m_controlled->move(Vector3f(0, -0.1, 0));
          break;
        case Qt::Key_Plus:
          m_controlled->move(Vector3f(0, 0, 0.1));
          break;
        case Qt::Key_Minus:
          m_controlled->move(Vector3f(0, 0, -0.1));
          break;
        case Qt::Key_A:
          m_controlled->rotateX(50);
          break;
        case Qt::Key_S:
          m_controlled->rotateX(-50);
          break;
        case Qt::Key_W:
          m_controlled->rotateY(50);
          break;
        case Qt::Key_E:
          m_controlled->rotateY(-50);
          break;
      }
    }

  private:
    BGE::Scene::SceneObject* m_controlled;
};

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  BGE::Canvas *canvas = BGE::Canvas::canvas();
  canvas->show();

  Cube* object = new Cube(1);
  object->addChild(new Cube(0.5));
  object->child(0)->move(Vector3f(0.5, 0.5, 0));

  Controller controller(object);
  canvas->setController(&controller);

  canvas->addSceneObject(object);

  return app.exec();
}
