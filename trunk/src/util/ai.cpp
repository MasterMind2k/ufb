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
#include "ai.h"

#include <QtCore/QQueue>

#include "canvas.h"

#include "scene/boundingvolume.h"
#include "scene/partition.h"
#include "scene/camera.h"

#include "objectlist.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"
#include "objects/bullet.h"

using namespace Util;

void Ai::calculateAngularVelocity()
{
  if (!m_target->hullIntegrity())
    m_target = 0l;

  if (!m_target)
    return;

  // Transform target's position to controlled eye space
  Vector3f target;
  float distance = m_controlled->distance(m_target);
  if (m_running && m_controlled->shields() >= 80.0f || !m_running && m_controlled->shields() > 10.0f) {
    // Attack
    m_running = false;
    target = m_controlled->globalOrientation().inverse() * (m_target->globalPosition() - m_controlled->globalPosition());
  } else{
    // Flee
    if (m_locationChange.elapsed() > 2000) {
      m_locationChange.restart();
      if (!m_running) {
        int range = BGE::Canvas::SceneSize.x() / 2;
        m_fleeLocation = Vector3f(qrand() % range * 2 - range, qrand() % range * 2 - range, qrand() % range * 2 - range);
      } else {
        m_fleeLocation *= -1;
      }
    }
    m_running = true;
    target = m_controlled->globalOrientation().inverse() * (m_fleeLocation - m_controlled->globalPosition());
  }
  float engine = distance * 1000;

  // Are we too close to target? (if we are not running)
  if (!m_running && !m_makeSpace && distance < 5000) {
    // Let's "overfly" the bastard and shoot him from behind!
    // Recalculate target position
    target = m_controlled->globalOrientation().inverse() * (m_target->globalPosition() - m_controlled->globalPosition());
    target.z() -= 16000;
    m_fleeLocation = m_controlled->globalOrientation() * target + m_controlled->globalPosition();
    m_makeSpace = true;
    engine = Objects::Fighter::MaxPower;
  } else if (m_makeSpace && distance > 5000) {
    // Turn and start shootin'!
    m_makeSpace = false;
  } else if (m_makeSpace) {
    // Still too close
    target = m_controlled->globalOrientation().inverse() * (m_fleeLocation - m_controlled->globalPosition());
    engine = Objects::Fighter::MaxPower;
  }

  bool isInFront = !m_running && !m_makeSpace && target.z() < 0;

  // Calculate needed roll, pitch nad engine power to get to the target
  float roll = this->roll(target);
  float pitch = this->pitch(target);

  // Stop if the target is in range and in front
  if (isInFront && distance < 15000)
    engine = 0;

  // Make it avoid evil asteroids!
  Objects::Object *nearest = ObjectList::self()->nearest(m_controlled, "Asteroid", 15000); // 15000 is considered safe distance

  if (m_running || m_makeSpace) {
    if (!nearest || m_controlled->distance(nearest) > distance)
      nearest = m_target;
  }

  // We should have nearest asteroid (if not, then there is no in close range, or there is a bug and fighter will crash :D )
  if (nearest) {
    Vector3f obsticle = m_controlled->globalOrientation().inverse() * (nearest->globalPosition() + nearest->velocity() - m_controlled->globalPosition());
    // Don't care if it is behind us
    if (obsticle.z() < 0 && obsticle.norm() + nearest->boundingVolume()->radius() < 3000) {
      // Adjust roll and pitch to pass it
      bool adjusted = false;
      Vector3f normalized;
      float compensation = 1000;

      if (pitch < 0) {
        // Go below
        normalized = (obsticle - Vector3f(0, nearest->boundingVolume()->radius() - compensation, 0)).normalized();
        float newPitch = acos(Vector3f(0, normalized.y(), normalized.z()).normalized().dot(-Vector3f::UnitZ()));
        if (qAbs(newPitch) > qAbs(pitch)) {
          pitch = newPitch;
          adjusted = true;
        }
      } else {
        // Go over
        normalized = (obsticle + Vector3f(0, nearest->boundingVolume()->radius() + compensation, 0)).normalized();
        float newPitch = -acos(Vector3f(0, normalized.y(), normalized.z()).normalized().dot(-Vector3f::UnitZ()));
        if (qAbs(newPitch) > qAbs(pitch)) {
          pitch = newPitch;
          adjusted = true;
        }
      }

      // Calculate roll
      if (adjusted)
        roll = this->roll(normalized);
    }
  }

  // Do we have a lock?
  if (isInFront) {
    QSizeF size = BGE::Canvas::canvas()->size();
    Vector3f position = BGE::Scene::Camera::projection() * target;
    Vector2f pos(size.width() * ((position.x() + 1.0) / 2.0), size.height() - size.height() * ((position.y() + 1.0) / 2.0));
    pos.x() -= size.width() / 2.0;
    pos.y() -= size.height() / 2.0;

    if (pos.norm() < 20.0)
      // We have a lock!
      m_controlled->setWeaponsLock(m_target);
    else
      m_controlled->setWeaponsLock(0l);
  } else {
    m_controlled->setWeaponsLock(0l);
  }

  // Some actions
  if (m_controlled->hasWeaponsLock())
    m_controlled->fire();

  if (!pitch && isInFront)
    roll = this->roll(m_target->globalOrientation() * Vector3f::UnitY());
  else if (!pitch && m_running)
    roll = this->roll(m_target->globalOrientation() * -Vector3f::UnitY());

  if (isInFront) {
    float absPitch = qAbs(pitch);
    if (absPitch > 0.5)
      pitch /= absPitch;

    float absRoll = qAbs(roll);
    if (absRoll > 0.5)
      roll /= absRoll;
  }

  m_controlled->setAngularVelocity(Vector3f(pitch, 0, roll));
  m_controlled->setEnginePower(engine);
}
