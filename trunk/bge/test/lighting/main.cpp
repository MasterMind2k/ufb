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

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/shaderprogram.h"

#include "movinglight.h"

BGE::Storage::Mesh* createCube()
{
  QString name = "cube";
  BGE::Storage::Mesh* mesh = new BGE::Storage::Mesh("Cube");
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

  // Load models and textures
  BGE::Canvas::canvas()->loadResource();

  // Make a small cube
  BGE::Storage::StorageManager::self()->set(createCube(), "/models");

  // Create camera
  BGE::Canvas::canvas()->createCamera("Global camera")->move(0, -30, 400);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Global camera"));
  BGE::Canvas::canvas()->activateCamera("Global camera");

  // Add our model
  BGE::Scene::Object* fighter = new BGE::Scene::Object;
  fighter->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Textured"));
  fighter->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Fighter"));
  fighter->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/textures/Fighter"));
  BGE::Canvas::canvas()->addSceneObject(fighter);

  // Create lights
  MovingLight* movingLight = new MovingLight;

  BGE::Scene::Light* light = BGE::Canvas::canvas()->createLight("Red light");
  light->move(-150, -180, -200);
  light->setDiffuseColor(QColor(0.5 * 255, 0, 0));
  light->setSpecularColor(QColor(0.8 * 255, 0, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->observe(fighter);
  light->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Shader"));
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Blue light");
  light->move(150, 80, -250);
  light->setDiffuseColor(QColor(0, 0, 0.5 * 255));
  light->setSpecularColor(QColor(0, 0, 0.8 * 255));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->observe(fighter);
  light->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Shader"));
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Green light");
  light->move(-150, 80, 250);
  light->setDiffuseColor(QColor(0, 0.5 * 255, 0));
  light->setSpecularColor(QColor(0, 0.8 * 255, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->observe(fighter);
  light->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Shader"));
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Yellow light");
  light->move(150, -180, 200);
  light->setDiffuseColor(QColor(0.5 * 255, 0.5 * 255, 0));
  light->setSpecularColor(QColor(0.8 * 255, 0.8 * 255, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->observe(fighter);
  light->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Shader"));
  movingLight->addChild(light);
  BGE::Canvas::canvas()->addSceneObject(movingLight);

  BGE::Canvas::canvas()->show();

  return app.exec();
}
