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
#include "partition.h"

#include "scene/object.h"
#include "scene/boundingvolume.h"

using namespace BGE;
using namespace BGE::Scene;

Partition::Partition(float width, float height, float depth)
{
  m_partitions.reserve(8);
  m_parent = 0l;
  m_boundingVolume = new BoundingVolume(Vector3f::Zero(), Vector3f(width, height, depth));
}

Partition::~Partition()
{
  if (m_boundingVolume)
    delete m_boundingVolume;
  foreach (Object *object, m_objects)
    object->setPartition(0l);

  qDeleteAll(m_partitions);
  m_partitions.clear();
}

void Partition::addObject(Object *object)
{
  // If object is bigger, go up
  if (parent() && m_boundingVolume->isBigger(object->boundingVolume())) {
    parent()->addObject(object);
    return;
  }

  if (!m_partitions.size()) {
    if (m_objects.size() > 20) {
      partition();
      // Retry
      addObject(object);
    } else {
      object->setPartition(this);
      m_objects << object;
    }
  } else {
    bool isAdded = false;
    foreach (Partition *partition, m_partitions) {
      if (partition->m_boundingVolume->isInside(object->boundingVolume())) {
        isAdded = true;
        partition->addObject(object);
        break;
      }
    }
    if (!isAdded) {
      object->setPartition(this);
      m_objects << object;
    }
  }
}

void Partition::removeObject(Object *object)
{
  m_objects.removeOne(object);

  if (parent() && !m_partitions.size() && !m_objects.size())
    parent()->departition();
}

void Partition::partition()
{
  Vector3f size = m_boundingVolume->size() / 2.0;
  for (quint8 i = 0; i < 8; i++) {
    Partition *subPartition = new Partition;

    // Move center to it's proper position
    Vector3f center = m_boundingVolume->center() + (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * size) / 2.0;
    subPartition->m_boundingVolume = new BoundingVolume(Vector3f::Zero(), size);
    Transform3f transform(Transform3f::Identity());
    transform.translate(center);
    subPartition->m_boundingVolume->setTransform(transform);

    m_partitions << subPartition;
    subPartition->m_parent = this;

    foreach (Object *object, m_objects) {
      if (subPartition->m_boundingVolume->isInside(object->boundingVolume())) {
        removeObject(object);
        subPartition->addObject(object);
      }
    }
  }
}

void Partition::departition()
{
  foreach (Partition *partition, m_partitions) {
    if (partition->m_objects.size())
      return;
  }

  QList<Object*> objects;
  foreach (Partition *partition, m_partitions) {
    if (partition->m_partitions.size()) {
      partition->departition();
      return;
    }
    objects += partition->m_objects;
  }
  foreach (Object *object, objects)
    addObject(object);

  qDeleteAll(m_partitions);
  m_partitions.clear();
}

