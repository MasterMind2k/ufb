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
class Canvas;

namespace Scene {
class Object;
class BoundingVolume;

/**
 * A node in the spatial octree.
 *
 * When there are many objects inside one partition, that partition gets divided
 * to eight parts.
 */
class Partition
{
  public:
    ~Partition();

    /**
     * Returns tobjects that are inside this partition.
     *
     * @note This list does not contain objects inside subpartitions.
     */
    inline const QList<Object*> &objects() const
    {
      return m_objects;
    }

    /**
     * Returns the subpartitions.
     */
    inline const QVector<Partition*> &partitions() const
    {
      return m_partitions;
    }

    /**
     * Returns the bounding volume of this partition.
     */
    inline const BoundingVolume *boundingVolume() const
    {
      return m_boundingVolume;
    }

    /**
     * Returns the parent partition.
     */
    inline const Partition *parent() const
    {
      return m_parent;
    }

  private:
    BoundingVolume *m_boundingVolume;
    QList<Object*> m_objects;
    QVector<Partition*> m_partitions;
    Partition *m_parent;

    // Partitioning methods
    void partition();
    void departition();

    // Accessable only from partition
    inline void setBoundingVolume(BoundingVolume *boundingVolume)
    {
      m_boundingVolume = boundingVolume;
    }

    // Accessable from Canvas
    inline Partition()
    {
      m_partitions.reserve(8);
      m_parent = 0l;
      m_boundingVolume = 0l;
    }
    Partition(const Vector3f &size);
    Partition(float width, float height, float depth);

    // Accessable from Canvas and Object
    void addObject(Object *object);
    inline void removeObject(int index)
    {
      m_objects.removeAt(index);
    }
    void removeObject(Object *object);

    inline Partition *partition(int index)
    {
      return m_partitions.at(index);
    }
    inline Partition *parent()
    {
      return m_parent;
    }

    friend class BGE::Canvas;
    friend class BGE::Scene::Object;
};

}
}

#endif
