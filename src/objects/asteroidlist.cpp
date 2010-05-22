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

  return AsteroidList::self()->transformedPositions().value(a2) > position;
}

AsteroidList::AsteroidList()
{
}

void AsteroidList::sort()
{
  qSort(m_asteroids.begin(), m_asteroids.end(), lessThen);
}
