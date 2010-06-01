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
#include "powerup.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "canvas.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/material.h"

#include "scene/light.h"

using namespace Objects;

PowerUp::PowerUp()
  : m_initialized(false),
    m_spawnAnimation(0)
{
  setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/powerups/cube"));

  m_glow = new BGE::Storage::Material("material");
  addMaterial(m_glow);

  qint8 decision = qrand() % 4;
  if (decision < 1) {
    m_type = LaserEnhancement;
    setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/powerups/laser"));
    m_glow->setEmission(Qt::yellow);
    m_color = QColor(40, 40, 0);
  } else if (decision < 2) {
    m_type = HullRepair;
    setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/powerups/repair"));
    m_glow->setEmission(Qt::red);
    m_color = QColor(40, 0, 0);
  } else if (decision < 3) {
    m_type = HullEnhancement;
    setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/powerups/hull"));
    m_glow->setEmission(Qt::green);
    m_color = QColor(0, 40, 0);
  } else if (decision < 4) {
    m_type = ShieldsBatteries;
    setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/powerups/shields"));
    m_glow->setEmission(Qt::blue);
    m_color = Qt::blue;
  }

  disableLighting();
  setName("PowerUp");
  setScale(0);
}

PowerUp::~PowerUp()
{
}

void PowerUp::initBody()
{
  Object::initBody();
  body()->setCollisionFlags(body()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  body()->setActivationState(DISABLE_DEACTIVATION);
}

void PowerUp::collision(BGE::Scene::Object *object)
{
  if (object->name() == "Fighter") {
    setRenderable(false);
    parent()->removeChild(this);
    BGE::Canvas::canvas()->dynamicsWorld()->removeRigidBody(body());
    BGE::Canvas::canvas()->deleteSceneObject(this);
  }
}

void PowerUp::calculateTransforms(qint32 timeDiff)
{
  m_spawnAnimation += timeDiff;

  if (!m_initialized) {
    setScale((float) m_spawnAnimation / (float) SpawnTime);
    if (m_spawnAnimation >= SpawnTime)
      setScale(1);
  }

  if (m_initialized && m_spawnAnimation / 1000.0 >= 2.0 * M_PI)
    m_spawnAnimation -= 2.0 * M_PI * 1000.0;
  float density = qMax(cos(m_spawnAnimation / 1000.0), 0.0);
  m_glow->setEmission(QColor(m_color.red() * density, m_color.green() * density, m_color.blue() * density));
}

void PowerUp::postTransformCalculations(qint32 timeDiff)
{
  if (!m_initialized && m_spawnAnimation >= SpawnTime) {
    initBody();
    m_initialized = true;

    Vector3f velocity(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30);
    velocity.normalize();
    body()->setAngularVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  }
}
