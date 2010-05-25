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
#ifndef OBJECTS_ASTEROIDLIST_H
#define OBJECTS_ASTEROIDLIST_H

#include "global.h"

#include <QtCore/QList>

namespace Objects {
class Asteroid;

class AsteroidList
{
  public:
    AsteroidList();

    static inline AsteroidList *self()
    {
      if (!m_self)
        m_self = new AsteroidList;
      return m_self;
    }

    void setPosition(Asteroid *asteroid, const Vector3f &position);

    inline void addAsteroid(Asteroid *asteroid)
    {
      m_asteroids << asteroid;
    }
    inline void removeAsteroid(Asteroid *asteroid)
    {
      m_asteroids.removeOne(asteroid);
      m_transformedPositions.remove(asteroid);
      if (m_nearestAsteroid == asteroid)
        m_nearestAsteroid = 0l;
    }
    inline const QList<Asteroid*> &asteroids() const
    {
      return m_asteroids;
    }

    void sort();

    const QHash<Asteroid*, Vector3f> &transformedPositions() const
    {
      return m_transformedPositions;
    }

    inline Asteroid *nearestAsteroid() const
    {
      return m_nearestAsteroid;
    }

  private:
    QList<Asteroid*> m_asteroids;
    QHash<Asteroid*, Vector3f> m_transformedPositions;
    Asteroid *m_nearestAsteroid;
    float m_distance;

    static AsteroidList *m_self;
};

}

#endif
