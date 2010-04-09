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
    m_objects << object;
    if (m_objects.size() > 5)
      partition();
  } else {
    foreach (Partition *partition, m_partitions) {
      if (partition->isInside(object->globalPosition())) {
        partition->addObject(object);
        break;
      }
    }
  }
}

void Partition::partition()
{
  for (quint8 i = 0; i < 8; i++) {
    Partition *subPartition = new Partition;
    subPartition->setSize(m_size / 2.0);

    subPartition->m_center = m_center;
    // Move center to it's proper position
    subPartition->m_center += (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * subPartition->size().vector()) / 2.0;

    m_partitions << subPartition;

    foreach (Object *object, m_objects) {
      if (subPartition->isInside(object->globalPosition())) {
        m_objects << object;
        removeObject(object);
      }
    }
  }
}
