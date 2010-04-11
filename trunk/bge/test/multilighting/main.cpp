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

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/material.h"
#include "storage/texture.h"
#include "storage/shaderprogram.h"

#include "scene/camera.h"
#include "scene/light.h"

#include "lighting.h"
#include "camera.h"
#include "controller.h"
#include "overlay.h"

BGE::Storage::Mesh* createCube()
{
  QString name = "cube";
  BGE::Storage::Mesh* mesh = new BGE::Storage::Mesh("Cube");
  float half = 3;
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
  mesh->addFaceMaterial(name, 0, "side");
  mesh->addFaceMaterial(name, 1, "side");
  mesh->addFaceMaterial(name, 2, "side");
  mesh->addFaceMaterial(name, 3, "side");
  mesh->addFaceMaterial(name, 4, "side");
  mesh->addFaceMaterial(name, 5, "side");
  return mesh;
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  // Load resources
  BGE::Storage::StorageManager::self()->load();

  // Load cube
  BGE::Storage::StorageManager::self()->set(createCube(), "/models/");
  BGE::Storage::Material *material = new BGE::Storage::Material("Light");
  material->setEmission(QColor(1 * 255, 1 * 255, 1 * 255));
  material->setFaceName("side");
  BGE::Storage::StorageManager::self()->set(material, "/materials/");

  // Setup camera
  BGE::Canvas::canvas()->createCamera("Global camera");
  BGE::Canvas::canvas()->activateCamera("Global camera");

  // Our controlled camera :D
  Camera *camera = new Camera;
  camera->move(0, 500, 0);
  camera->rotateX(-90);
  camera->addChild(BGE::Canvas::canvas()->camera("Global camera"));
  BGE::Canvas::canvas()->addSceneObject(camera);
  camera->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  camera->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));

  // And the controller
  BGE::Canvas::canvas()->setController(new Controller(camera));

  // Overlay graphics
  BGE::Canvas::canvas()->setOverlay(new Overlay(camera));

  // Setup objects
  BGE::Scene::Object *object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Surface"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Textured"));
  object->move(-150, 0, 150);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Hard coding material :D
  BGE::Storage::Material *surfaceMaterial = new BGE::Storage::Material("Surface material", QColor(0.5 * 255, 0.5 * 255, 0.5 * 255), QColor(255, 255, 255), QColor(0, 0, 0), QColor(0, 0, 0), 80);
  surfaceMaterial->setFaceName("phong2SG");
  BGE::Storage::StorageManager::self()->set(surfaceMaterial, "/materials");
  object->addMaterial(surfaceMaterial);
  object->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/textures/Desert"));

  // First R2-D2
  object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/R2-D2"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));
  object->loadMaterialsFromMesh();
  object->rotateX(-90);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Second R2-D2
  object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/R2-D2"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));
  object->loadMaterialsFromMesh();
  object->move(100, 0, 100);
  object->rotateX(-90);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Third R2-D2
  object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/R2-D2"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));
  object->loadMaterialsFromMesh();
  object->move(-100, 0, -100);
  object->rotateX(-90);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Fourth R2-D2
  object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/R2-D2"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));
  object->loadMaterialsFromMesh();
  object->move(100, 0, -100);
  object->rotateX(-90);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Fifth R2-D2
  object = new BGE::Scene::Object;
  object->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/R2-D2"));
  object->setShaderProgram(BGE::Storage::StorageManager::self()->get<BGE::Storage::ShaderProgram*>("/shaders/Object"));
  object->loadMaterialsFromMesh();
  object->move(-100, 0, 100);
  object->rotateX(-90);
  BGE::Canvas::canvas()->addSceneObject(object);

  // Add light container
  BGE::Canvas::canvas()->addSceneObject(new Lighting);

  // Show canvas
  BGE::Canvas::canvas()->show();

  return app.exec();
}

