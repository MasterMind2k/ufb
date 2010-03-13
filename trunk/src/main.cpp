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
#include <QtGui/QApplication>

#include "canvas.h"
#include "abstractcontroller.h"

#include "rendering/renderer.h"

#include "scene/sceneobject.h"
#include "scene/camera.h"

#include "storage/storage.h"
#include "storage/mesh.h"
#include "storage/texture.h"

// Our little testing cube :D
class Cube : public BGE::Scene::SceneObject
{
  public:
    Cube(BGE::Mesh* mesh, float a)
    {
      setMesh(mesh);
      m_a = a;
      scale(m_a);
    }

    Cube(float a)
    {
      m_a = a;
      prepareMesh();
      scale(m_a);
    }

  protected:
    void prepareMesh()
    {
      QString name = "cube";
      VectorList temp;
      float half = 0.5;
      // Prepare our vertices
      Vector3f bottomFrontLeft(-half, -half, half);
      Vector3f bottomFrontRight(half, -half, half);
      Vector3f topFrontLeft(-half, half, half);
      Vector3f topFrontRight(half, half, half);

      Vector3f bottomBehindLeft(-half, -half, -half);
      Vector3f bottomBehindRight(half, -half, -half);
      Vector3f topBehindLeft(-half, half, -half);
      Vector3f topBehindRight(half, half, -half);

      BGE::Mesh* mesh = new BGE::Mesh("Cube");

      // And draw the cube
      // Front side
      mesh->addRectangle(name, bottomFrontLeft, bottomFrontRight, topFrontLeft, topFrontRight);
      // Right side
      mesh->addRectangle(name, bottomFrontRight, bottomBehindRight, topFrontRight, topBehindRight);
      // Left side
      mesh->addRectangle(name, bottomBehindLeft, bottomFrontLeft, topBehindLeft, topFrontLeft);
      // Bottom side
      mesh->addRectangle(name, bottomBehindLeft, bottomBehindRight, bottomFrontLeft, bottomFrontRight);
      // Behind side
      mesh->addRectangle(name, topBehindLeft, topBehindRight, bottomBehindLeft, bottomBehindRight);
      // Top side
      mesh->addRectangle(name, topFrontLeft, topFrontRight, topBehindLeft, topBehindRight);

      mesh->calculateNormals("cube");
      setMesh(mesh);
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
          m_controlled->rotateX(M_PI / 150);
          break;
        case Qt::Key_S:
          m_controlled->rotateX(-M_PI / 150);
          break;
        case Qt::Key_W:
          m_controlled->rotateY(M_PI / 150);
          break;
        case Qt::Key_E:
          m_controlled->rotateY(-M_PI / 150);
          break;
        case Qt::Key_Space:
          // Camera rotation
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

  // Let's initialize canvas and show it
  BGE::Canvas *canvas = BGE::Canvas::canvas();
  canvas->loadResource("./res.rcc");
  canvas->show();

  // Create a cube with two smaller cubes as it's childs
  Cube* object = new Cube(1);
  object->addChild(new Cube(object->mesh(), 0.5));
  object->addChild(new Cube(object->mesh(), 0.1));
  // Move the childs, to create a little more complicated object
  object->child(0)->move(0.5, 0.5, 0);
  object->child(1)->move(0, 0, 3);

  // Create a camera and set it a child of the "complicated" object
  BGE::Scene::Camera* camera = canvas->createCamera("First camera");
  object->addChild(camera);
  camera->move(0, 0, 3);

  // Create the second camera and move it up
  canvas->addSceneObject(canvas->createCamera("Second camera"));
  canvas->camera("Second camera")->move(0, 20, 5);
  canvas->camera("Second camera")->rotateX(-M_PI / 2);

  // Set the first camera the default camera
  canvas->activateCamera("First camera");

  // Create and use our controller
  Controller controller(object);
  canvas->setController(&controller);

  // Add our object to the scene
  canvas->addSceneObject(object);

  // Create another cube and add it to the scene
  object = new Cube(object->mesh(), 2.0);
  object->move(0, 0, 10);
  canvas->addSceneObject(object);

  // QApplication event loop execution
  return app.exec();
}
