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

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "canvas.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "scene/camera.h"
#include "scene/boundingvolume.h"

#include "util/ai.h"

#include "bullet.h"
#include "exhaust.h"
#include "explosion.h"
#include "powerup.h"

using namespace Objects;

qreal Fighter::MaxPower = 1000000.0;
qreal Fighter::MaxVelocity = 1435.0;
quint16 Fighter::ShieldsRechargeTime = 2000;
float Fighter::ShieldsRechargeTick = 10.0f;
qint32 Fighter::MaxShakeTime = 300;

Fighter::Fighter(Util::Ai *ai)
  : m_ai(ai),
    m_hullIntegrity(100),
    m_maxHullIntegrity(100),
    m_shields(100),
    m_maxShields(100),
    m_firepower(10),
    m_dyingElapsedTime(0),
    m_previousExplosion(0),
    m_exploded(false),
    m_lockedTarget(0l),
    m_shake(false),
    m_shakeTime(0)
{
  setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter"));
  loadMaterialsFromMesh();
  setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/fighters/textures/fighter"));

  m_enginePower = 0;
  m_angularVelocity = Vector3f::Zero();
  setMass(1000);

  // Add our exhaust :)
  addChild(new Exhaust(this));
  child(0)->move(18, 50, 240);
  addChild(new Exhaust(this));
  child(1)->move(-18, 50, 240);

  // Laser reload
  m_previousShot.start();

  setName("Fighter");

  // Register only ai controlled fighters
  if (ai)
    setRegistered(true);
}

Fighter::~Fighter()
{
  if (!m_ai)
    BGE::Canvas::canvas()->activeCamera()->observe(0l);
}

void Fighter::initBody()
{
  Object::initBody();
  body()->setDamping(0.5, 0);
  body()->setActivationState(DISABLE_DEACTIVATION);
}

void Fighter::fire()
{
  if (m_previousShot.elapsed() < 500)
    return;
  else
    m_previousShot.restart();

  // Right bullet
  Bullet *bullet = new Bullet;
  bullet->setOrientation(globalOrientation());
  bullet->move(globalPosition() + globalOrientation() * Vector3f(150, 0, -450));

  lockLaser(bullet, false);

  parent()->addChild(bullet);
  bullet->initBody();

  Vector3f velocity(0, 0, -Bullet::Velocity);
  velocity = bullet->orientation() * velocity + this->velocity();
  bullet->body()->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  bullet->body()->activate();

  // Left bullet
  bullet = new Bullet;
  bullet->setOrientation(globalOrientation());
  bullet->move(globalPosition() + globalOrientation() * Vector3f(-150, 0, -450));

  lockLaser(bullet, true);

  parent()->addChild(bullet);
  bullet->initBody();

  velocity = Vector3f(0, 0, -Bullet::Velocity);
  velocity = bullet->orientation() * velocity + this->velocity();
  bullet->body()->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
  bullet->body()->activate();
}

void Fighter::calculateTransforms(qint32 timeDiff)
{
  if (m_hullIntegrity <= 0.0) {
    // Let's remove ourselves from the list
    setRegistered(false);

    // Dying animation :)
    m_dyingElapsedTime += timeDiff;
    qreal distance = boundingVolume()->radius();
    if (m_dyingElapsedTime < 4000.0) {
      if (m_previousExplosion > 300) {
        qreal pos = distance - qrand() % (int) (distance / 2.0);
        BGE::Canvas::canvas()->addSceneObject(new Explosion(globalPosition() + pos * Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized(), Explosion::Small));
        m_previousExplosion = 0;
      } else {
        m_previousExplosion += timeDiff;
      }

    } else if (!m_exploded) {
      qreal pos = distance / 2.0;
      BGE::Canvas::canvas()->addSceneObject(new Explosion(globalPosition() + pos * Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized(), Explosion::Large));
      m_exploded = true;

      // Remove itself from physics world
      BGE::Canvas::canvas()->dynamicsWorld()->removeRigidBody(body());

    } else if (m_dyingElapsedTime < 5000.0) {
      scale(1 - (m_dyingElapsedTime - 4000.0) / 1000.0);
    } else {
      // Spawn a powerup
      if (m_ai) {
        PowerUp *powerup = new PowerUp;
        powerup->move(globalPosition());
        BGE::Canvas::canvas()->addSceneObject(powerup);

        m_ai->setControlled(0l);
        m_ai = 0l;
      }

      // Remove itself
      setRenderable(false);
      parent()->removeChild(this);
      BGE::Canvas::canvas()->deleteSceneObject(this);
    }
  } else {
    // Animate camera shake
    if (m_shake) {
      m_shakeTime += timeDiff;
      qreal angle = (qreal) m_shakeTime / (qreal) MaxShakeTime * 2.0 * M_PI;
      BGE::Canvas::canvas()->camera("First person camera")->rotateX((qrand() % 3 - 1) * sin(angle));
      BGE::Canvas::canvas()->camera("First person camera")->rotateY((qrand() % 3 - 1) * sin(angle));

      if (m_shakeTime >= MaxShakeTime) {
        m_shakeTime = MaxShakeTime;
        m_shake = false;
        BGE::Canvas::canvas()->camera("First person camera")->setOrientation(Quaternionf::Identity());
      }
    }

    if (m_shields < m_maxShields && m_shieldsRecharge.elapsed() > ShieldsRechargeTime) {
      m_shields += ShieldsRechargeTick;
      m_shields = qMin(m_shields, m_maxShields);
      m_shieldsRecharge.restart();
    }

    if (m_ai)
      m_ai->calculateAngularVelocity();
  }

  Vector3f enginePower = globalOrientation() * Vector3f(0, 0, -m_enginePower);
  body()->applyCentralForce(btVector3(enginePower.x(), enginePower.y(), enginePower.z()));
  Vector3f angularVelocity = globalOrientation() * m_angularVelocity;
  body()->setAngularVelocity(btVector3(angularVelocity.x(), angularVelocity.y(), angularVelocity.z()));
}

void Fighter::collision(BGE::Scene::Object *object)
{
  // Nothing to hit :D
  if (m_hullIntegrity <= 0.0f)
    return;

  if (object->name() == "Laser") {
    // Divert power to shields!
    m_shieldsRecharge.start();

    // Shake the camera
    if (!m_ai) {
      m_shake = true;
      m_shakeTime = 0;
    }

    if (m_shields > 0.0) {
      // We have shields!
      m_shields -= 10;
      if (m_shields < 0.0) {
        // Shields collapsed! (electrical surges, falling wiring, etc. :P )
        m_hullIntegrity += m_shields;
        m_shields = 0;
      }
    } else {
      // No shields
      m_hullIntegrity = qMax(0.0f, m_hullIntegrity - 10.0f);
    }
  } else if (object->name() == "Asteroid" || object->name() == "Fighter") {
    // We are done for!
    m_shields = 0;
    m_hullIntegrity = 0;
  } else if (object->name() == "PowerUp") {
    // We got a cookie :)
    switch (static_cast<PowerUp*> (object)->type()) {
      case PowerUp::LaserEnhancement:
        m_firepower += 5;
        break;

      case PowerUp::HullEnhancement:
        m_maxHullIntegrity += 10;
        m_hullIntegrity += 10;
        break;

      case PowerUp::HullRepair:
        m_hullIntegrity = qMin(m_maxHullIntegrity, m_hullIntegrity + 50);
        break;

      case PowerUp::ShieldsBatteries:
        m_maxShields += 10;
        break;
    }
  }
}

void Fighter::lockLaser(Bullet *bullet, bool isLeft) const
{
  if (!m_lockedTarget || m_acquiringTarget.elapsed() < 500)
    return;

  Quaternionf rotation = bullet->orientation();

  Vector3f target = rotation.inverse() * (m_lockedTarget->globalPosition() - (bullet->position() + rotation * Vector3f(0, 0, 2 * bullet->boundingVolume()->radius())));
  qreal distance = target.norm();

  // Take it into account that bullet needs time, and the target is moving (so are we)
  qreal time = distance / (Bullet::Velocity + velocity().norm() - m_lockedTarget->velocity().norm());
  target += rotation.inverse() * m_lockedTarget->velocity() * time - rotation.inverse() * velocity() * time;

  // Calculate the angles
  Quaternionf angle;
  angle.setFromTwoVectors(-Vector3f::UnitZ(), target.normalized());
  bullet->rotate(angle);
}
