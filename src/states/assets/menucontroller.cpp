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
#include "menucontroller.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"

#include "menuoverlay.h"

#include "states/game.h"

#include "canvas.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/material.h"

#include "scene/camera.h"
#include "scene/light.h"

using namespace States;
using namespace States::Assets;

MenuController::MenuController()
{}

void MenuController::keyPressed(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_R : {
      if (event->modifiers() & Qt::CTRL)
        BGE::Canvas::canvas()->setFPSShown(!BGE::Canvas::canvas()->isFPSShown());
      break;
    }

    case Qt::Key_Up:
      m_overlay->moveSelection(MenuOverlay::Up);
      break;

    case Qt::Key_Down:
      m_overlay->moveSelection(MenuOverlay::Down);
      break;

    case Qt::Key_Return:
      execute();
      break;

    default:
      break;
  }
}

void MenuController::execute()
{
  switch (m_overlay->selectedButton()) {
    case MenuOverlay::Play: {
      Game *game = new Game();
      game->fighter()->move(0, 0, 0);
      game->fighter()->rotateY(180);
      game->fighter()->initBody();
      BGE::Canvas::canvas()->pushGameState(game);
      populateAsteroids();
      game->fighter()->addChild(BGE::Canvas::canvas()->createCamera("Global camera"));
      //BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createCamera("Global camera"));
      BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createLight("Global light"));
      BGE::Canvas::canvas()->light("Global light")->setPositional(false);
      BGE::Canvas::canvas()->light("Global light")->setPosition(0, 0, -1);
      BGE::Canvas::canvas()->camera("Global camera")->move(0, 0, -227);
      BGE::Canvas::canvas()->addSceneObject(game->fighter());
      BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(game->fighter()->body());
      BGE::Canvas::canvas()->activateCamera("Global camera");

      game->fighter()->body()->setGravity(btVector3(0, 0, 0));
      game->fighter()->body()->applyGravity();

      // Create skybox for game
      // Front
      BGE::Scene::Object *skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_FT"));
      skybox->move(0, 0, -BGE::Canvas::SceneSize.z());
      BGE::Canvas::canvas()->addSceneObject(skybox);
      // Back
      skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_BK"));
      skybox->rotateY(180);
      skybox->move(0, 0, BGE::Canvas::SceneSize.z());
      BGE::Canvas::canvas()->addSceneObject(skybox);
      // Left
      skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_LT"));
      skybox->rotateY(90);
      skybox->move(-BGE::Canvas::SceneSize.x(), 0, 0);
      BGE::Canvas::canvas()->addSceneObject(skybox);
      // Right
      skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_RT"));
      skybox->rotateY(-90);
      skybox->move(BGE::Canvas::SceneSize.x(), 0, 0);
      BGE::Canvas::canvas()->addSceneObject(skybox);
      // Top
      skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_UP"));
      skybox->rotateX(90);
      skybox->move(0, BGE::Canvas::SceneSize.y(), 0);
      BGE::Canvas::canvas()->addSceneObject(skybox);
      // Bottom
      skybox = new BGE::Scene::Object;
      skybox->disableCulling();
      skybox->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/skybox/model"));
      skybox->setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/skybox/Galaxy_DN"));
      skybox->rotateX(-90);
      skybox->move(0, -BGE::Canvas::SceneSize.y(), 0);
      BGE::Canvas::canvas()->addSceneObject(skybox);

      break;
    }

    case MenuOverlay::Quit: {
      BGE::Canvas::canvas()->close();
      break;
    }
  }
}

void MenuController::populateAsteroids()
{
  int max = BGE::Canvas::canvas()->SceneSize.x();
  int min = BGE::Canvas::canvas()->SceneSize.x() / 2;
  qsrand(time(0l));
  for (quint16 i = 0; i < 30; i++) {
    Objects::Asteroid *asteroid = new Objects::Asteroid;
    asteroid->move(qrand() % max - min, qrand() % max - min, qrand() % max - min);
    BGE::Canvas::canvas()->addSceneObject(asteroid);

    asteroid->initBody();
    BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(asteroid->body());
    asteroid->body()->setGravity(btVector3(0, 0, 0));
    asteroid->body()->applyGravity();
  }
}
