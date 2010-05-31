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
#include "statehandler.h"

#include <ctime>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "canvas.h"

#include "scene/camera.h"
#include "scene/light.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "states/game.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"

#include "util/ai.h"
#include "util/objectlist.h"

using namespace States;
using namespace Assets;

StateHandler *StateHandler::m_self = 0l;

StateHandler::StateHandler()
  : m_game(0l)
{
}

void StateHandler::play()
{
  // Cleanup previous game
  if (m_game) {
    Util::ObjectList::self()->clear();
    delete m_game;
  }

  // Create a new one
  Game *game = new Game();
  BGE::Canvas::canvas()->pushGameState(game);

  // Create bounderies
  setRestraints();

  Objects::Fighter *fighter = new Objects::Fighter;
  game->setFighter(fighter);
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
  BGE::Canvas::canvas()->camera("Side camera")->move(2500, 0, 0);
  BGE::Canvas::canvas()->camera("Side camera")->rotateY(90);

  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createLight("Global light"));
  BGE::Canvas::canvas()->light("Global light")->setPositional(false);
  BGE::Canvas::canvas()->light("Global light")->setPosition(0, 0, -1);

  // Default camera
  BGE::Canvas::canvas()->activateCamera("First person camera");

  // Add Ai fighter
  Util::Ai *ai = new Util::Ai(fighter);
  fighter = new Objects::Fighter(ai);
  ai->setControlled(fighter);
  fighter->move(0, -10000, 0);
  fighter->initBody();
  BGE::Canvas::canvas()->addSceneObject(fighter);

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

  // Remember that we have an ongoing game :)
  m_game = game;
}

void StateHandler::unload()
{
  // Do we have anything to unload?
  if (!m_game)
    return;

  // We already remembered the game state
  BGE::Canvas::canvas()->popGameState();
}

void StateHandler::resume()
{
  // Can we resume?
  if (!m_game)
    return;

  BGE::Canvas::canvas()->pushGameState(m_game);
}

void StateHandler::populateAsteroids()
{
  int max = (BGE::Canvas::canvas()->SceneSize.x()) - 2000 / 2.0;
  int min = max / 2 + 4000;
  qsrand(time(0l));
  for (quint16 i = 0; i < 30; i++) {
    Objects::Asteroid *asteroid = new Objects::Asteroid(Objects::Asteroid::Large);
    asteroid->move(qrand() % max - min, qrand() % max - min, qrand() % max - min);
    BGE::Canvas::canvas()->addSceneObject(asteroid);

    asteroid->initBody();

    Vector3f velocity = Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized();
    asteroid->body()->setAngularVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  }
}

void StateHandler::setRestraints()
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
