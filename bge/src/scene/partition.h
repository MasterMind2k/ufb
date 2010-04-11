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

#include "size.h"

namespace BGE {

namespace Scene {

class Object;

class Partition
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    inline Partition()
    {
      m_partitions.reserve(8);
      m_points.reserve(8);
      m_center = Vector3f::Zero();
      m_parent = 0l;
    }

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

    inline void setSize(const Size &size)
    {
      setSize(size.width(), size.height(), size.depth());
    }
    void setSize(float width, float height, float depth);
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
    inline float radius() const
    {
      return m_radius;
    }

    void partition();
    void departition();

    bool isInside(const Vector3f &point, bool debug = false) const;

    bool isBigger(float radius)
    {
      return radius > m_radius;
    }

    const QVector<Vector3f> &points() const
    {
      return m_points;
    }

    inline Partition *parent() const
    {
      return m_parent;
    }

  private:
    Vector3f m_center;
    Size m_size;
    QList<Object*> m_objects;
    QVector<Partition*> m_partitions;
    QVector<Vector3f> m_points;
    Partition *m_parent;
    float m_radius;
};

}
}

#endif
