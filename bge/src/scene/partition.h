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
#ifndef __BGE_SCENE_PARTITION_H
#define __BGE_SCENE_PARTITION_H

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QSizeF>

#include "global.h"

namespace BGE {

namespace Scene {

class Object;
class BoundingVolume;

class Partition
{
  public:
    inline Partition()
    {
      m_partitions.reserve(8);
      m_parent = 0l;
      m_boundingVolume = 0l;
    }
    Partition(const Vector3f &size);
    Partition(float width, float height, float depth);
    ~Partition();

    void addObject(Object *object);
    inline void removeObject(int index)
    {
      m_objects.removeAt(index);
    }
    void removeObject(Object *object);
    inline const QList<Object*> &objects() const
    {
      return m_objects;
    }
    inline Object *object(int index) const
    {
      return m_objects.at(index);
    }

    inline void setBoundingVolume(BoundingVolume *boundingVolume)
    {
      m_boundingVolume = boundingVolume;
    }
    inline const BoundingVolume *size() const
    {
      return m_boundingVolume;
    }

    inline const QVector<Partition*> &partitions() const
    {
      return m_partitions;
    }
    inline Partition *partition(int index) const
    {
      return m_partitions.at(index);
    }

    inline const BoundingVolume *boundingVolume() const
    {
      return m_boundingVolume;
    }

    void partition();
    void departition();

    inline Partition *parent() const
    {
      return m_parent;
    }

  private:
    BoundingVolume *m_boundingVolume;
    QList<Object*> m_objects;
    QVector<Partition*> m_partitions;
    Partition *m_parent;
};

}
}

#endif
