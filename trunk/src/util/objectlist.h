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
#ifndef UTIL_OBJECTLIST_H
#define UTIL_OBJECTLIST_H

#include "global.h"

#include <QtCore/QList>

namespace Objects {
class Object;
}

namespace Util {

class ObjectList
{
  public:
    ObjectList();

    static inline ObjectList *self()
    {
      if (!m_self)
        m_self = new ObjectList;
      return m_self;
    }

    void setPosition(Objects::Object *object, const Vector3f &position);

    inline void add(Objects::Object *object)
    {
      m_objects << object;
    }
    inline void remove(Objects::Object *object)
    {
      m_objects.removeOne(object);
      m_transformedPositions.remove(object);
      if (m_nearestAsteroid == object)
        m_nearestAsteroid = 0l;
      else if (m_nearestFighter == object)
        m_nearestFighter = 0l;
    }
    inline const QList<Objects::Object*> &objects() const
    {
      return m_objects;
    }

    void sort();

    const QHash<Objects::Object*, Vector3f> &transformedPositions() const
    {
      return m_transformedPositions;
    }

    /**
     * Nearest object relative to the player.
     */
    inline Objects::Object *nearest(const QString &name) const
    {
      if (name == "Asteroid")
        return m_nearestAsteroid;
      else
        return m_nearestFighter;
    }
    /**
     * Nearest object relative to the reference object.
     */
    Objects::Object *nearest(Objects::Object *reference, const QString &name = QString(), float maxRange = 0) const;

  private:
    QList<Objects::Object*> m_objects;
    QHash<Objects::Object*, Vector3f> m_transformedPositions;
    Objects::Object *m_nearestAsteroid;
    Objects::Object *m_nearestFighter;
    float m_distanceAsteroid;
    float m_distanceFighter;

    static ObjectList *m_self;
};

}

#endif
