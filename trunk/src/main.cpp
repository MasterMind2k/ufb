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

#include <QtGui/QSplashScreen>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "canvas.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/material.h"

#include "scene/light.h"
#include "scene/camera.h"

#include "states/menu.h"

void createCube()
{
  BGE::Storage::Mesh* mesh = new BGE::Storage::Mesh("cube");
  float half = 200;
  // Prepare our vertices
  Vector3f bottomFrontLeft(-half, -half, half);
  Vector3f bottomFrontRight(half, -half, half);
  Vector3f topFrontLeft(-half, half, half);
  Vector3f topFrontRight(half, half, half);

  Vector3f bottomBehindLeft(-half, -half, -half);
  Vector3f bottomBehindRight(half, -half, -half);
  Vector3f topBehindLeft(-half, half, -half);
  Vector3f topBehindRight(half, half, -half);
  QVector<Vector2f> uvMapping;
  uvMapping << Vector2f(0, 0) << Vector2f(1, 0) << Vector2f(0, 1) << Vector2f(1, 1);
  // And draw the cube
  // Front side
  mesh->addRectangle("front", bottomFrontLeft, bottomFrontRight, topFrontLeft, topFrontRight);
  mesh->addTextureMaps("front", uvMapping);
  // Right side
  mesh->addRectangle("right", bottomFrontRight, bottomBehindRight, topFrontRight, topBehindRight);
  mesh->addTextureMaps("right", uvMapping);
  // Left side
  mesh->addRectangle("left", bottomBehindLeft, bottomFrontLeft, topBehindLeft, topFrontLeft);
  mesh->addTextureMaps("left", uvMapping);
  // Bottom side
  mesh->addRectangle("bottom", bottomBehindLeft, bottomBehindRight, bottomFrontLeft, bottomFrontRight);
  mesh->addTextureMaps("bottom", uvMapping);
  // Behind side
  mesh->addRectangle("behind", topBehindLeft, topBehindRight, bottomBehindLeft, bottomBehindRight);
  mesh->addTextureMaps("behind", uvMapping);
  // Top side
  mesh->addRectangle("top", topFrontLeft, topFrontRight, topBehindLeft, topBehindRight);
  mesh->addTextureMaps("top", uvMapping);

  mesh->calculateNormals("front");
  mesh->calculateNormals("right");
  mesh->calculateNormals("left");
  mesh->calculateNormals("bottom");
  mesh->calculateNormals("behind");
  mesh->calculateNormals("top");

  mesh->addFaceMaterial("front", 0, "material");
  mesh->addFaceMaterial("right", 0, "material");
  mesh->addFaceMaterial("left", 0, "material");
  mesh->addFaceMaterial("bottom", 0, "material");
  mesh->addFaceMaterial("behind", 0, "material");
  mesh->addFaceMaterial("top", 0, "material");

  BGE::Storage::Manager::self()->set(mesh, "/powerups/");
}

void createSkyboxQuad()
{
  BGE::Storage::Mesh *skybox = new BGE::Storage::Mesh("model");

  skybox->createObject("quad");
  float maxSize = BGE::Canvas::SceneSize.x();
  skybox->addRectangle("quad", Vector3f(-maxSize, -maxSize, 0), Vector3f(maxSize, -maxSize, 0), Vector3f(-maxSize, maxSize, 0), Vector3f(maxSize, maxSize, 0));
  skybox->addTextureMaps("quad", QVector<Vector2f>() << Vector2f(0, 0) << Vector2f(1, 0) << Vector2f(0, 1) << Vector2f(1, 1));

  skybox->addNormals("quad", QVector<Vector3f>() << Vector3f(0, 0, -1) << Vector3f(0, 0, -1) << Vector3f(0, 0, -1) << Vector3f(0, 0, -1));

  BGE::Storage::Manager::self()->set(skybox, "/skybox/");
}

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("UFB");
  QApplication::setOrganizationName("UFB");

  QSplashScreen splash(QPixmap(":/ufb.png"));
  splash.showMessage("Initializing...", Qt::AlignLeft | Qt::AlignBottom);
  splash.show();

  Q_INIT_RESOURCE(bge_resources);

  // Set the scene size
  BGE::Canvas::SceneSize = Vector3f(500000, 500000, 500000);

  // Load data
  splash.showMessage("Loading resources...", Qt::AlignLeft | Qt::AlignBottom);
  BGE::Canvas::canvas()->loadResource("./resources.rcc");

  // Create sky box quad and cube
  splash.showMessage("Preparing menu...", Qt::AlignLeft | Qt::AlignBottom);
  createSkyboxQuad();
  createCube();

  // Itensify ambient light
  BGE::Scene::Light::setGlobalAmbient(QColor(120, 120, 120));

  // Fix fighter model :)
  BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter")->rotate(AngleAxisf(-M_PI / 2.0, Vector3f::UnitX()));
  BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/projectiles/bullet")->rotate(AngleAxisf(-M_PI / 2.0, Vector3f::UnitX()));

  BGE::Canvas::canvas()->pushGameState(new States::Menu);

  // Make the skybox for menu
  // Front
  BGE::Scene::Object *skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_FT"));
  skybox->move(0, 0, -BGE::Canvas::SceneSize.z());
  BGE::Canvas::canvas()->addSceneObject(skybox);
  // Back
  skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_BK"));
  skybox->rotateY(180);
  skybox->move(0, 0, BGE::Canvas::SceneSize.z());
  BGE::Canvas::canvas()->addSceneObject(skybox);
  // Left
  skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_LT"));
  skybox->rotateY(90);
  skybox->move(-BGE::Canvas::SceneSize.x(), 0, 0);
  BGE::Canvas::canvas()->addSceneObject(skybox);
  // Right
  skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_RT"));
  skybox->rotateY(-90);
  skybox->move(BGE::Canvas::SceneSize.x(), 0, 0);
  BGE::Canvas::canvas()->addSceneObject(skybox);
  // Top
  skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_UP"));
  skybox->rotateX(90);
  skybox->move(0, BGE::Canvas::SceneSize.y(), 0);
  BGE::Canvas::canvas()->addSceneObject(skybox);
  // Bottom
  skybox = new BGE::Scene::Object;
  skybox->disableCulling();
  skybox->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
  skybox->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_DN"));
  skybox->rotateX(-90);
  skybox->move(0, -BGE::Canvas::SceneSize.y(), 0);
  BGE::Canvas::canvas()->addSceneObject(skybox);

  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createCamera("Global camera"));
  BGE::Canvas::canvas()->camera("Global camera")->move(0, 0, 600);
  BGE::Canvas::canvas()->activateCamera("Global camera");

  BGE::Canvas::canvas()->createLight("Global light")->setPosition(0, 0, -1);
  BGE::Canvas::canvas()->light("Global light")->setPositional(false);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->light("Global light"));

  splash.showMessage("Finished. Have fun!", Qt::AlignLeft | Qt::AlignBottom);

  BGE::Canvas::canvas()->show();
  splash.finish(BGE::Canvas::canvas());

  return app.exec();
}
