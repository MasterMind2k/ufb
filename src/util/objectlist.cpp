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
#include "objectlist.h"

#include "canvas.h"

#include "objects/object.h"

#include "scene/camera.h"
#include "scene/boundingvolume.h"
#include "scene/partition.h"

using namespace Util;

ObjectList *ObjectList::m_self = 0l;

bool lessThen(Objects::Object *a1, Objects::Object *a2)
{
  Vector3f position = ObjectList::self()->transformedPositions().value(a1);
  if (position.z() < -1 || position.z() > 1)
    return false;

  if (position.x() < -1 || position.x() > 1)
    return false;

  if (position.y() < -1 || position.y() > 1)
    return false;

  return true;
}

ObjectList::ObjectList()
{
  m_nearestAsteroid = 0l;
}

void ObjectList::setPosition(Objects::Object *asteroid, const Vector3f &position)
{
  m_transformedPositions.insert(asteroid, position);

  if (!m_nearestAsteroid) {
    m_nearestAsteroid = asteroid;
    m_distance = (BGE::Canvas::canvas()->activeCamera()->globalPosition() - asteroid->globalPosition()).norm() - asteroid->boundingVolume()->radius();
  } else {
    float distance = (BGE::Canvas::canvas()->activeCamera()->globalPosition() - asteroid->globalPosition()).norm() - asteroid->boundingVolume()->radius();
    if (distance < m_distance) {
      m_nearestAsteroid = asteroid;
      m_distance = distance;
    }
  }
}

void ObjectList::sort()
{
  qSort(m_objects.begin(), m_objects.end(), lessThen);
}

Objects::Object *ObjectList::nearest(Objects::Object *reference, const QString &name, float maxRange) const
{
  float min = maxRange;
  if (!min)
    min = BGE::Canvas::SceneSize.norm();

  Objects::Object *nearest = 0l;
  BGE::Scene::Partition *currentParent = 0l;
  QQueue<BGE::Scene::Object*> objects;
  objects.append(reference->partition()->objects());
  while (!objects.isEmpty()) {
    BGE::Scene::Object *object = objects.dequeue();
    // Exclude the reference object
    if (object == reference)
      continue;

    if (!name.isNull() && object->name() != name) {
      // Widen the search, if we didn't find any nearest objects
      if (objects.isEmpty() && object->partition()->parent()) {
        if (object->partition()->parent() != currentParent) {
          currentParent = object->partition()->parent();
          foreach (BGE::Scene::Partition *partition, object->partition()->parent()->partitions()) {
            if (partition == object->partition())
              continue;

            // Add only objects that are closer than min
            foreach (BGE::Scene::Object *object, partition->objects()) {
              if ((object->globalPosition() - reference->globalPosition()).norm() < min)
                objects.enqueue(object);
            }
          }
        } else {
          // Same for parent partition
          foreach (BGE::Scene::Object *object, object->partition()->parent()->objects()) {
            if ((object->globalPosition() - reference->globalPosition()).norm() < min)
              objects.enqueue(object);
          }
        }
      }

      continue;
    }

    // Let's find our minimum
    float distance = (object->globalPosition() - reference->globalPosition()).norm();
    if (distance < min) {
      // A nearest asteroid!
      min = distance;
      nearest = static_cast<Objects::Object*> (object);
    }
  }

  return nearest;
}
