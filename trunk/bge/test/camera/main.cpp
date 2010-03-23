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

#include "scene/object.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "storage/mesh.h"

#include "movingcube.h"
#include "cameras.h"

BGE::Storage::Mesh* createCube()
{
  QString name = "cube";
  BGE::Storage::Mesh* mesh = new BGE::Storage::Mesh(name);
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
  return mesh;
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Cameras cameras;

  // Setup global light
  BGE::Scene::Light* light = BGE::Canvas::canvas()->createLight("Sun");
  light->move(0, 30, 0);
  BGE::Canvas::canvas()->addSceneObject(light);

  // Create global camera
  BGE::Scene::Camera* camera = BGE::Canvas::canvas()->createCamera("Global camera");
  camera->move(0, 30, 0);
  camera->rotateX(-M_PI / 2);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(camera);
  BGE::Canvas::canvas()->activateCamera("Global camera");

  // Our cube :D
  BGE::Storage::Mesh* mesh = createCube();

  // Create moving cubes
  MovingCube* movingCube = new MovingCube(0);
  movingCube->setMesh(mesh);

  // Lower left
  MovingCube* child = new MovingCube(1);
  child->setMesh(mesh);
  child->scale(0.5);
  child->move(-2, -2, 2);
  movingCube->addChild(child);
  camera = BGE::Canvas::canvas()->createCamera("First moving camera");
  camera->move(1.2, 1.2, -1.2);
  child->addChild(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);

  // Lower right
  child = new MovingCube(2);
  child->setMesh(mesh);
  child->scale(0.5);
  child->move(2, -2, 2);
  movingCube->addChild(child);
  camera = BGE::Canvas::canvas()->createCamera("Second moving camera");
  camera->move(-1.2, 1.2, -1.2);
  child->addChild(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);

  // Top left
  child = new MovingCube(3);
  child->setMesh(mesh);
  child->scale(0.5);
  child->move(-2, 2, -2);
  movingCube->addChild(child);
  camera = BGE::Canvas::canvas()->createCamera("Third moving camera");
  camera->move(1.2, -1.2, 1.2);
  child->addChild(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);

  // Top right
  child = new MovingCube(4);
  child->setMesh(mesh);
  child->scale(0.5);
  child->move(2, 2, -2);
  movingCube->addChild(child);
  BGE::Canvas::canvas()->addSceneObject(movingCube);
  camera = BGE::Canvas::canvas()->createCamera("Fourth moving camera");
  camera->move(-1.2, -1.2, 1.2);
  child->addChild(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);

  // Big stacionary cubes
  BGE::Scene::Object* bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(10, 10, -10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("First static camera");
  camera->move(-3.1, -3.1, 3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(10, -10, -10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Second static camera");
  camera->move(-3.1, 3.1, 3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(-10, 10, -10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Third static camera");
  camera->move(3.1, -3.1, 3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(-10, -10, -10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Fourth static camera");
  camera->move(3.1, 3.1, 3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(10, 10, 10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Fifth static camera");
  camera->move(-3.1, -3.1, -3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(10, -10, 10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Sixth static camera");
  camera->move(-3.1, 3.1, -3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(-10, 10, 10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Seventh static camera");
  camera->move(3.1, -3.1, -3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  bigCube = new BGE::Scene::Object;
  bigCube->setMesh(mesh);
  bigCube->scale(3);
  bigCube->move(-10, -10, 10);
  // Camera
  camera = BGE::Canvas::canvas()->createCamera("Eigth static camera");
  camera->move(3.1, 3.1, -3.1);
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->observe(movingCube);
  cameras.addCamera(camera);
  BGE::Canvas::canvas()->addSceneObject(bigCube);

  // Show the canvas
  BGE::Canvas::canvas()->show();

  return app.exec();
}
