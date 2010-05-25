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
#include "Eigen/QtAlignedMalloc"
#include <QtGui/QApplication>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "canvas.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/material.h"

#include "scene/light.h"
#include "scene/camera.h"

#include "states/menu.h"

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
  qsrand(time(0l));
  Q_INIT_RESOURCE(bge_resources);

  QApplication app(argc, argv);
  QApplication::setApplicationName("UFB");

  // Set the scene size
  BGE::Canvas::SceneSize = Vector3f(100000, 100000, 100000);

  // Load data
  BGE::Canvas::canvas()->loadResource("./resources.rcc");

  // Create sky box quad
  createSkyboxQuad();

  // Itensify ambient light
  BGE::Scene::Light::setGlobalAmbient(QColor(120, 120, 120));

  // Fix fighter model :)
  BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter")->rotate(AngleAxisf(-M_PI / 2, Vector3f::UnitX()));
  BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/projectiles/bullet")->rotate(AngleAxisf(-M_PI / 2, Vector3f::UnitX()));

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

  BGE::Canvas::canvas()->show();

  return app.exec();
}
