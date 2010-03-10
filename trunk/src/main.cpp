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
#include "scene/camera.h"

// Our little testing cube :D
class Cube : public BGE::Scene::SceneObject
{
  public:
    Cube(float a)
    {
      m_a = a;
    }

  protected:
    void render(BGE::Rendering::Renderer* renderer)
    {
      float half = m_a /2;
      Vector3f bottomFrontLeft(-half, -half, half);
      Vector3f bottomFrontRight(half, -half, half);
      Vector3f topFrontLeft(-half, half, half);
      Vector3f topFrontRight(half, half, half);

      Vector3f bottomBehindLeft(-half, -half, -half);
      Vector3f bottomBehindRight(half, -half, -half);
      Vector3f topBehindLeft(-half, half, -half);
      Vector3f topBehindRight(half, half, -half);

      // Front
      renderer->drawRectangle(bottomFrontLeft, bottomFrontRight, topFrontLeft, topFrontRight);
      // Right side
      renderer->drawRectangle(bottomFrontRight, bottomBehindRight, topFrontRight, topBehindRight);
      // Left side
      renderer->drawRectangle(bottomBehindLeft, bottomFrontLeft, topBehindLeft, topFrontLeft);
      // Bottom
      renderer->drawRectangle(bottomBehindLeft, bottomBehindRight, bottomFrontLeft, bottomFrontRight);
      // Behind
      renderer->drawRectangle(topBehindLeft, topBehindRight, bottomBehindLeft, bottomBehindRight);
      // Top
      renderer->drawRectangle(topFrontLeft, topFrontRight, topBehindLeft, topBehindRight);
    }

  private:
    float m_a;
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
          m_controlled->rotateX(M_PI/150);
          break;
        case Qt::Key_S:
          m_controlled->rotateX(-M_PI/150);
          break;
        case Qt::Key_W:
          m_controlled->rotateY(M_PI/150);
          break;
        case Qt::Key_E:
          m_controlled->rotateY(-M_PI/150);
          break;
        case Qt::Key_Space:
          if (BGE::Canvas::canvas()->activeCamera() != BGE::Canvas::canvas()->camera("First camera"))
            BGE::Canvas::canvas()->activateCamera("First camera");
          else
            BGE::Canvas::canvas()->activateCamera("Second camera");
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
  object->addChild(new Cube(0.1));
  object->child(0)->move(0.5, 0.5, 0);
  object->child(1)->move(0, 0, 3);

  BGE::Scene::Camera* camera = canvas->createCamera("First camera");
  object->addChild(camera);
  camera->move(0, 0, 3);

  canvas->addSceneObject(canvas->createCamera("Second camera"));
  canvas->camera("Second camera")->move(0, 20, 5);

  canvas->camera("Second camera")->rotateX(-M_PI / 2);

  canvas->activateCamera("First camera");
  Controller controller(object);
  canvas->setController(&controller);

  canvas->addSceneObject(object);

  object = new Cube(2);
  object->move(0, 0, 10);
  canvas->addSceneObject(object);

  return app.exec();
}
