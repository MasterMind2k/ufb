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

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"

#include "canvas.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"

#include "motionstate.h"

using namespace Objects;

btVoronoiSimplexSolver Object::m_solver;

Object::Object()
  : m_body(0),
    m_mass(1.0)
{
}

Object::~Object()
{
  delete m_body;
}

void Object::initBody()
{
  btConvexHullShape *boundingVolume = new btConvexHullShape(mesh()->vertices(), mesh()->numVertices(), 3 * sizeof(float));
  Vector3f scaled = this->scaled();
  boundingVolume->setLocalScaling(btVector3(scaled.x(), scaled.y(), scaled.z()));

  btRigidBody::btRigidBodyConstructionInfo info(m_mass, new BGE::MotionState(this), boundingVolume);
  m_body = new btRigidBody(info);
  BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(m_body);
  m_body->setGravity(btVector3(0, 0, 0));
  m_body->applyGravity();
}

Vector3f Object::velocity() const
{
  btVector3 velocity = m_body->getLinearVelocity();
  return Vector3f(velocity.x(), velocity.y(), velocity.z());
}

qreal Object::distance(Object *object) const
{
  btGjkPairDetector raycast(static_cast<btConvexShape*> (body()->getCollisionShape()), static_cast<btConvexShape*> (object->body()->getCollisionShape()), &m_solver, 0l);

  btPointCollector output;
  btDiscreteCollisionDetectorInterface::ClosestPointInput input;
  input.m_transformA = body()->getWorldTransform();
  input.m_transformB = object->body()->getWorldTransform();

  raycast.getClosestPoints(input, output, 0l);
  if (output.m_hasResult) {
    return output.m_distance < 0 ? 0 : output.m_distance;
  } else {
    return BGE::Canvas::SceneSize.norm();
  }
}
