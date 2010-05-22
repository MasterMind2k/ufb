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
#include "asteroidlist.h"

#include "canvas.h"

#include "asteroid.h"

#include "scene/camera.h"
#include "scene/boundingvolume.h"

using namespace Objects;

AsteroidList *AsteroidList::m_self = 0l;

bool lessThen(Asteroid *a1, Asteroid *a2)
{
  Vector3f position = AsteroidList::self()->transformedPositions().value(a1);
  if (position.z() < -1 || position.z() > 1)
    return false;

  if (position.x() < -1 || position.x() > 1)
    return false;

  if (position.y() < -1 || position.y() > 1)
    return false;

  return true;
}

AsteroidList::AsteroidList()
{
  m_nearestAsteroid = 0l;
}

void AsteroidList::setPosition(Asteroid *asteroid, const Vector3f &position)
{
  m_transformedPositions.insert(asteroid, position);

  if (!m_nearestAsteroid) {
    m_nearestAsteroid = asteroid;
    m_distance = (BGE::Canvas::canvas()->activeCamera()->globalPosition() - asteroid->globalPosition()).norm() - asteroid->boundingVolume()->radius();
  } else {
    float distance = (BGE::Canvas::canvas()->activeCamera()->globalPosition() - asteroid->globalPosition()).norm() - asteroid->boundingVolume()->radius();
    if (distance < m_distance) {
      m_nearestAsteroid = asteroid;
      m_distance = distance;
    }
  }
}

void AsteroidList::sort()
{
  qSort(m_asteroids.begin(), m_asteroids.end(), lessThen);
}
