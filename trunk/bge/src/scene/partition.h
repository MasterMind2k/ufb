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

class Size
{
  public:
    inline Size()
    {
      m_vector = Vector3f::Zero();
    }
    inline Size(float width, float height, float depth)
    {
      m_vector.x() = width;
      m_vector.y() = height;
      m_vector.z() = depth;
    }
    inline Size(const Vector3f &vector)
    {
      m_vector = vector;
    }

    inline float &width()
    {
      return m_vector.x();
    }
    inline float &height()
    {
      return m_vector.y();
    }
    inline float &depth()
    {
      return m_vector.z();
    }
    inline const float width() const
    {
      return m_vector.x();
    }
    inline const float height() const
    {
      return m_vector.y();
    }
    inline const float depth() const
    {
      return m_vector.z();
    }

    inline Vector3f &vector()
    {
      return m_vector;
    }
    inline const Vector3f &vector() const
    {
      return m_vector;
    }

    inline Size operator/(float divider) const
    {
      return Size(m_vector / divider);
    }

  private:
    Vector3f m_vector;
};

namespace Scene {

class Object;

class Partition
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    inline Partition()
    {
      m_partitions.reserve(8);
      m_center = Vector3f::Zero();
    }

    void addObject(Object *object);
    inline void removeObject(int index)
    {
      m_objects.removeAt(index);
    }
    inline void removeObject(Object *object)
    {
      m_objects.removeOne(object);
    }
    inline const QList<Object*> &objects() const
    {
      return m_objects;
    }
    inline Object *object(int index) const
    {
      return m_objects.at(index);
    }

    inline void setSize(const Size &size)
    {
      m_size = size;
    }
    inline void setSize(float width, float height, float depth)
    {
      m_size.width() = width;
      m_size.height() = height;
      m_size.depth() = depth;
    }
    inline const Size &size() const
    {
      return m_size;
    }

    inline const QVector<Partition*> &partitions() const
    {
      return m_partitions;
    }
    inline Partition *partition(int index) const
    {
      return m_partitions.at(index);
    }

    inline const Vector3f &center() const
    {
      return m_center;
    }

    void partition();

    bool isInside(const Vector3f &point) const
    {
      return point <= size().vector() + m_center && point >= m_center - size().vector();
    }

  private:
    Vector3f m_center;
    Size m_size;
    QList<Object*> m_objects;
    QVector<Partition*> m_partitions;
};

}
}

#endif
