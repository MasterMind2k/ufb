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
  Vector3f target = m_controlled->globalOrientation().inverse() * ((m_target->globalPosition() + m_target->globalOrientation() * -Vector3f::UnitZ() * m_target->velocity().norm() * m_controlled->distance(m_target) / Objects::Bullet::Velocity) - m_controlled->globalPosition());
  float distance = target.norm();

  // Calculate needed roll, pitch nad engine power to get to the target
  float roll = this->roll(target);
  float pitch = this->pitch(target);
  float engine = distance * 1000;

  // Stop if the target is in range and in front
  if (distance < 5000 && target.z() < 0)
    engine = 0;

  // Make it avoid evil asteroids!
  Objects::Object *nearest = ObjectList::self()->nearest(m_controlled, "Asteroid", 15000); // 15000 is considered safe distance

  // We should have nearest asteroid (if not, then there is no in close range, or there is a bug and fighter will crash :D )
  if (nearest) {
    Vector3f asteroid = m_controlled->globalOrientation().inverse() * (nearest->globalPosition() - m_controlled->globalPosition());
    // Don't care if it is behind us
    if (asteroid.z() < 0) {
      // Adjust roll and pitch to pass it
      bool adjusted = false;
      Vector3f normalized;
      if (pitch < 0) {
        // Go below
        normalized = (asteroid - Vector3f(0, nearest->boundingVolume()->radius() + 100, 0)).normalized();
        float newPitch = -acos(Vector3f(0, normalized.y(), normalized.z()).dot(-Vector3f::UnitZ()));
        if (newPitch < pitch) {
          pitch = newPitch;
          adjusted = true;
        }
      } else {
        // Go over
        normalized = (asteroid + Vector3f(0, nearest->boundingVolume()->radius() - 100, 0)).normalized();
        float newPitch = acos(Vector3f(0, normalized.y(), normalized.z()).dot(-Vector3f::UnitZ()));
        if (newPitch > pitch) {
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
  if (m_controlled->distance(m_target) < 30000 && qAbs(pitch) < 5.0 * M_PI / 180.0)
    m_controlled->fire();

  // Stabilizing
  if (qAbs(pitch) < 0.01)
    pitch = 0;
  else if (qAbs(pitch) > M_PI / 30.0)
    pitch = pitch / qAbs(pitch) * 1;
  if (qAbs(roll) < 0.01)
    roll = 0;
  else if (qAbs(roll) > M_PI / 30.0)
    roll = roll / qAbs(roll) * 1;

  m_controlled->setAngularVelocity(Vector3f(pitch, 0, roll));
  m_controlled->setEnginePower(engine);
}
