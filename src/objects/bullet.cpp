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
#include "bullet.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "canvas.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/material.h"

using namespace Objects;

qreal Bullet::Velocity = 10000;

Bullet::Bullet()
{
  setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/projectiles/bullet"));
  BGE::Storage::Material *material = BGE::Storage::StorageManager::self()->get<BGE::Storage::Material*>("/projectiles/bullet/Material");
  material->setEmission(material->diffuse());
  loadMaterialsFromMesh();
  scale(100);

  m_lifetime = 0;
}

void Bullet::postTransformCalculations(qint32 timeDiff)
{
  m_lifetime += timeDiff;
  if (m_lifetime > MaxLifetime) {
    setRenderable(false);
    parent()->removeChild(this);
    BGE::Canvas::canvas()->dynamicsWorld()->removeRigidBody(body());
    BGE::Canvas::canvas()->deleteSceneObject(this);
  }
}
