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
#include "asteroid.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "canvas.h"

#include "scene/camera.h"

#include "asteroidlist.h"

#include "motionstate.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "scene/boundingvolume.h"

using namespace Objects;

Asteroid::Asteroid()
{
  QList<BGE::Storage::Item*> meshes = BGE::Storage::StorageManager::self()->get("/asteroids/models")->items();
  setMesh(static_cast<BGE::Storage::Mesh*> (meshes.at(qrand() % meshes.size())));

  setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/asteroids/textures/" + mesh()->name()));

  m_body = 0l;

  AsteroidList::self()->addAsteroid(this);
}

void Asteroid::initBody()
{
  btRigidBody::btRigidBodyConstructionInfo info(1000, new BGE::MotionState(this), new btBoxShape(btVector3(boundingVolume()->size().x(), boundingVolume()->size().y(), boundingVolume()->size().z())));
  info.m_friction = 0;
  m_body = new btRigidBody(info);
}

void Asteroid::postTransformCalculations(qint32 timeDiff)
{
  AsteroidList::self()->setPosition(this, BGE::Scene::Camera::projection() * BGE::Canvas::canvas()->activeCamera()->cameraTransform() * globalPosition());
}