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
#include "fighter.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "bullet.h"

using namespace Objects;

Fighter::Fighter()
{
  setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter"));
  loadMaterialsFromMesh();
  setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/fighters/textures/fighter"));

  m_enginePower = 0;
  m_angularVelocity = Vector3f::Zero();
  setMass(1000);
}

void Fighter::initBody()
{
  Object::initBody();
  body()->setDamping(0.5, 0);
}

void Fighter::fire()
{
  // Right bullet
  Bullet *bullet = new Bullet;
  bullet->setOrientation(globalOrientation());
  bullet->move(globalPosition() + globalOrientation() * Vector3f(150, -10, 0));

  parent()->addChild(bullet);
  bullet->initBody();

  Vector3f velocity(0, 0, -Bullet::Velocity);
  velocity = globalOrientation() * velocity + this->velocity();
  bullet->body()->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  bullet->body()->activate();

  // Left bullet
  bullet = new Bullet;
  bullet->setOrientation(globalOrientation());
  bullet->move(globalPosition() + globalOrientation() * Vector3f(-150, -10, 0));

  parent()->addChild(bullet);
  bullet->initBody();

  bullet->body()->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  bullet->body()->activate();
}

void Fighter::calculateTransforms(qint32 timeDiff)
{
  Q_UNUSED(timeDiff);

  Vector3f enginePower = globalOrientation() * Vector3f(0, 0, -m_enginePower);
  body()->applyCentralForce(btVector3(enginePower.x(), enginePower.y(), enginePower.z()));
  Vector3f angularVelocity = globalOrientation() * m_angularVelocity;
  body()->setAngularVelocity(btVector3(angularVelocity.x(), angularVelocity.y(), angularVelocity.z()));
}
