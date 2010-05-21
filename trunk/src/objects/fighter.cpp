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
#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "motionstate.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "scene/boundingvolume.h"

using namespace Objects;

Fighter::Fighter()
{
  setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter"));
  setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/fighters/textures/fighter"));

  m_body = 0l;

  m_enginePower = 0;
  m_angularVelocity = Vector3f::Zero();
}

void Fighter::initBody()
{
  btRigidBody::btRigidBodyConstructionInfo info(1000, new BGE::MotionState(this), new btBoxShape(btVector3(boundingVolume()->size().x(), boundingVolume()->size().y(), boundingVolume()->size().z())));
  m_body = new btRigidBody(info);
  m_body->setDamping(0.5, 0);
}

void Fighter::calculateTransforms(qint32 timeDiff)
{
  Q_UNUSED(timeDiff);

  Vector3f enginePower = globalOrientation() * Vector3f(0, 0, -m_enginePower);
  m_body->applyCentralForce(btVector3(enginePower.x(), enginePower.y(), enginePower.z()));
  Vector3f angularVelocity = globalOrientation() * m_angularVelocity;
  m_body->setAngularVelocity(btVector3(angularVelocity.x(), angularVelocity.y(), angularVelocity.z()));
}
