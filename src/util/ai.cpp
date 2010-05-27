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

#include "objectlist.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"
#include "objects/bullet.h"

using namespace Util;

void Ai::calculateAngularVelocity()
{
  // Transform target's position to controlled eye space
  Vector3f target;
  if (m_running && m_controlled->shields() >= 80.0f || !m_running && m_controlled->shields() > 10.0f) {
    // Attack
    m_running = false;
    target = m_controlled->globalOrientation().inverse() * ((m_target->globalPosition() + (m_target->velocity())
                                                             * m_controlled->distance(m_target) / (Objects::Bullet::Velocity + m_controlled->velocity().norm()))
                                                            - m_controlled->globalPosition());
  } else{
    // Flee
    m_running = true;
    if (m_locationChange.elapsed() > 20000) {
      int range = BGE::Canvas::SceneSize.x() / 2;
      m_fleeLocation = Vector3f(qrand() % range * 2 - range, qrand() % range * 2 - range, qrand() % range * 2 - range);
      m_locationChange.restart();
    }
    target = m_controlled->globalOrientation().inverse() * (m_fleeLocation - m_controlled->globalPosition());
  }
  float distance = target.norm();
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
    if (!nearest || m_controlled->distance(nearest) > m_controlled->distance(m_target))
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

  // Some actions
  float absPitch = qAbs(pitch);
  if (isInFront && distance < 25000 && absPitch < 5.0 * M_PI / 180.0)
    m_controlled->fire();

  if (isInFront) {
    if (absPitch > 0.04)
      pitch /= absPitch;

    float absRoll = qAbs(roll);
    if (absRoll > 0.04)
      roll /= absRoll;
  }

  if (!pitch && isInFront)
    roll = this->roll(m_target->globalOrientation() * Vector3f::UnitY());
  else if (!pitch && m_running)
    roll = this->roll(m_target->globalOrientation() * -Vector3f::UnitY());

  m_controlled->setAngularVelocity(Vector3f(pitch, 0, roll));
  m_controlled->setEnginePower(engine);
}