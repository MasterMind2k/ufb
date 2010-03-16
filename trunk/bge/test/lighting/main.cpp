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

#include "scene/light.h"
#include "scene/camera.h"

#include "storage/storage.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "movinglight.h"

BGE::Mesh* createCube()
{
  QString name = "cube";
  BGE::Mesh* mesh = new BGE::Mesh(name);
  float half = 5;
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

  // Make a small cube
  BGE::Mesh* cube = createCube();

  // Load models and textures
  BGE::Canvas::canvas()->loadResource();

  // Create camera
  BGE::Canvas::canvas()->createCamera("Global camera")->move(0, -30, 400);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Global camera"));
  BGE::Canvas::canvas()->activateCamera("Global camera");

  // Add our model
  BGE::Scene::SceneObject* fighter = new BGE::Scene::SceneObject;
  fighter->setMesh(BGE::Storage::self()->get<BGE::Mesh*>("/models/Fighter"));
  fighter->setTexture(BGE::Storage::self()->get<BGE::Texture*>("/textures/Fighter"));
  BGE::Canvas::canvas()->addSceneObject(fighter);

  // Create lights
  MovingLight* movingLight = new MovingLight;

  BGE::Scene::Light* light = BGE::Canvas::canvas()->createLight("Red light");
  light->move(-150, -80, -200);
  light->setDiffuseColor(Qt::red);
  light->setSpotExponent(100);
  light->setMesh(cube);
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Blue light");
  light->move(150, 80, -200);
  light->setDiffuseColor(Qt::blue);
  light->setSpotExponent(100);
  light->setMesh(cube);
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Green light");
  light->move(-150, 80, 200);
  light->setDiffuseColor(Qt::green);
  light->setSpotExponent(100);
  light->setMesh(cube);
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Yellow light");
  light->move(150, -80, 200);
  light->setDiffuseColor(Qt::yellow);
  light->setSpotExponent(100);
  light->setMesh(cube);
  light->observe(fighter);
  movingLight->addChild(light);
  BGE::Canvas::canvas()->addSceneObject(movingLight);

  BGE::Canvas::canvas()->show();

  return app.exec();
}
