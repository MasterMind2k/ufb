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

using namespace BGE;
using namespace BGE::Scene;

void Partition::addObject(Object *object)
{
  if (!m_partitions.size()) {
    if (m_objects.size() > 4) {
      partition();
      // Retry
      addObject(object);
      qDebug("partitioning");
    } else {
      object->setPartition(this);
      m_objects << object;
    }
  } else {
    if (m_partitions.first()->isBigger(object->boundingSphereRadius())) {
      qDebug("oo");
      object->setPartition(this);
      m_objects << object;
    } else {
      bool isAdded = false;
      foreach (Partition *partition, m_partitions) {
        if (partition->isInside(object->globalPosition())) {
          /*object->setPartition(this);
          partition->m_objects << object;*/
          isAdded = true;
          partition->addObject(object);
          break;
        }
      }
      if (!isAdded) {
        qDebug("OMG");
        if (parent()) {
          parent()->addObject(object);
        } else {
          object->setPartition(this);
          m_objects << object;
        }
      }
    }
  }
}

void Partition::removeObject(Object *object)
{
  m_objects.removeOne(object);

  if (!m_partitions.size() && !m_objects.size())
    parent()->departition();
}

void Partition::setSize(float width, float height, float depth)
{
  m_size.width() = width;
  m_size.height() = height;
  m_size.depth() = depth;

  float radius = 0;
  for (quint8 i = 0; i < 8; i++) {
    Vector3f point = (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * m_size.vector()) / 2.0;
    radius = qMax(point.norm(), radius);
  }

  m_points.clear();
  m_points = m_size.points(m_center);
  m_radius = radius;
}

void Partition::partition()
{
  Size size = m_size / 2.0;
  for (quint8 i = 0; i < 8; i++) {
    Partition *subPartition = new Partition;

    subPartition->m_center = m_center;
    // Move center to it's proper position
    subPartition->m_center += (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * size.vector()) / 2.0;
    subPartition->setSize(size);

    m_partitions << subPartition;
    subPartition->m_parent = this;

    foreach (Object *object, m_objects) {
      if (subPartition->isInside(object->globalPosition()) && !subPartition->isBigger(object->boundingSphereRadius())) {
        subPartition->addObject(object);
        removeObject(object);
      }
    }
  }
}

void Partition::departition()
{
  /*qDebug("departicioneram");
  quint8 size = m_objects.size();
  foreach (Partition *partition, m_partitions)
    size += partition->m_objects.size();

  if (size < 4) {
    foreach (Partition *partition, m_partitions)
      m_objects += partition->m_objects;
    foreach (Object *object, m_objects) {
      object->setPartition(this);
    }

    qDeleteAll(m_partitions);
    m_partitions.clear();
  }*/
}

bool Partition::isInside(const Vector3f &point, bool debug) const
{
  if (m_points.isEmpty())
    return false;
  // Let's perform min-max test :D
  Vector3f min = m_points.last();
  Vector3f max = m_points.first();
  if (debug) {
    qDebug("min (%f %f %f)", min.x(), min.y(), min.z());
    qDebug("max (%f %f %f)", max.x(), max.y(), max.z());
    qDebug("point (%f %f %f)", point.x(), point.y(), point.z());
    qDebug("%s", (max.cwise() >= point).all() && (min.cwise() <= point).all() ? "true" : "false");
  }
  return (max.cwise() >= point).all() && (min.cwise() <= point).all();
}
