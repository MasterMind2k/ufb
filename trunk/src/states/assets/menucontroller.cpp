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
#include "LinearMath/btDefaultMotionState.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"

#include "menuoverlay.h"

#include "states/game.h"

#include "canvas.h"

#include "storage/manager.h"
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
      BGE::Canvas::canvas()->pushGameState(game);

      // Create bounderies
      setRestraints();

      Objects::Fighter *fighter = new Objects::Fighter;
      game->setFighter(fighter);
      fighter->rotateY(180);
      fighter->initBody();
      BGE::Canvas::canvas()->addSceneObject(fighter);
      populateAsteroids();

      // Setup cameras
      fighter->addChild(BGE::Canvas::canvas()->createCamera("First person camera"));
      BGE::Canvas::canvas()->camera("First person camera")->move(0, 0, -230);
      fighter->addChild(BGE::Canvas::canvas()->createCamera("Third person camera"));
      BGE::Canvas::canvas()->camera("Third person camera")->move(0, 200, 400);
      BGE::Canvas::canvas()->camera("Third person camera")->rotateX(-15);
      fighter->addChild(BGE::Canvas::canvas()->createCamera("Front camera"));
      BGE::Canvas::canvas()->camera("Front camera")->move(0, -50, -500);
      BGE::Canvas::canvas()->camera("Front camera")->rotateY(180);
      BGE::Canvas::canvas()->camera("Front camera")->rotateX(15);
      fighter->addChild(BGE::Canvas::canvas()->createCamera("Side camera"));
      BGE::Canvas::canvas()->camera("Side camera")->move(600, 0, 0);
      BGE::Canvas::canvas()->camera("Side camera")->rotateY(90);

      // Default camera
      BGE::Canvas::canvas()->activateCamera("First person camera");

      //BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createCamera("Global camera"));
      BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createLight("Global light"));
      BGE::Canvas::canvas()->light("Global light")->setPositional(false);
      BGE::Canvas::canvas()->light("Global light")->setPosition(0, 0, -1);
      //BGE::Canvas::canvas()->camera("Global camera")->move(0, 600, 0);
      //BGE::Canvas::canvas()->camera("Global camera")->rotateX(-90);

      // Create skybox for game
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
  int max = BGE::Canvas::canvas()->SceneSize.x() / 1.5 - 2000;
  int min = max / 2 + 4000;
  qsrand(time(0l));
  for (quint16 i = 0; i < 30; i++) {
    Objects::Asteroid *asteroid = new Objects::Asteroid(Objects::Asteroid::Large);
    asteroid->move(qrand() % max - min, qrand() % max - min, qrand() % max - min);
    BGE::Canvas::canvas()->addSceneObject(asteroid);

    asteroid->initBody();
    Vector3f velocity(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30);
    velocity = velocity.normalized() * (qrand() % 2000 + 800);
    asteroid->body()->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));

    velocity = Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized();
    asteroid->body()->setAngularVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  }
}

void MenuController::setRestraints()
{
  qreal width = BGE::Canvas::canvas()->SceneSize.x();
  btRigidBody::btRigidBodyConstructionInfo info(0, 0l, new btBoxShape(btVector3(width, width, 1)), btVector3(0, 0, 0));
  btTransform transform;

  // Front
  transform.setIdentity();
  transform.setOrigin(btVector3(0, 0, -width / 2.0));
  info.m_motionState = new btDefaultMotionState(transform);
  btRigidBody *boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);

  // Behind
  transform.setIdentity();
  transform.setOrigin(btVector3(0, 0, width / 2.0));
  info.m_motionState = new btDefaultMotionState(transform);
  boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);

  // Left
  transform.setIdentity();
  transform.setOrigin(btVector3(-width / 2.0, 0, 0));
  info.m_motionState = new btDefaultMotionState(transform);
  info.m_collisionShape = new btBoxShape(btVector3(1, width, width));
  boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);

  // Right
  transform.setIdentity();
  transform.setOrigin(btVector3(width / 2.0, 0, 0));
  info.m_motionState = new btDefaultMotionState(transform);
  boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);

  // Top
  transform.setIdentity();
  transform.setOrigin(btVector3(0, width / 2.0, 0));
  info.m_collisionShape = new btBoxShape(btVector3(width, 1, width));
  info.m_motionState = new btDefaultMotionState(transform);
  boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);

  // Bottom
  transform.setIdentity();
  transform.setOrigin(btVector3(0, -width / 2.0, 0));
  info.m_motionState = new btDefaultMotionState(transform);
  boundry = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(boundry);
  boundry->setRestitution(1.0);
}
