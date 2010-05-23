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
#include "object.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"

#include "motionstate.h"

using namespace Objects;

Object::Object()
  : m_body(0)
{
}

void Object::initBody()
{
  btConvexHullShape *boundingVolume = new btConvexHullShape(mesh()->vertices(), mesh()->numVertices(), 3 * sizeof(float));
  Vector3f scaled = this->scaled();
  boundingVolume->setLocalScaling(btVector3(scaled.x(), scaled.y(), scaled.z()));

  btRigidBody::btRigidBodyConstructionInfo info(1000, new BGE::MotionState(this), boundingVolume);
  m_body = new btRigidBody(info);
}
