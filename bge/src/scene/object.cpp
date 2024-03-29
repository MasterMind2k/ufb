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
#include "scene/object.h"

#include <cmath>

#include "canvas.h"

#include "scene/partition.h"
#include "scene/boundingvolume.h"

#include "rendering/renderer.h"

#include "storage/mesh.h"

using namespace BGE;
using namespace BGE::Scene;

Object::Object()
  : m_isRenderable(false),
    m_isCulled(true),
    m_ignoreLights(false)
{
  // Set the default transforms
  m_transform.setIdentity();
  m_globalTransform.setIdentity();
  // Set the default position, orientation and scale
  m_orientation.setIdentity();
  m_globalOrientation.setIdentity();
  m_position.setZero();
  m_globalPosition.setZero();
  m_scale = Scaling3f(1, 1, 1);

  // And init other properties
  m_parent = 0l;
  m_transformModified = false;
  m_mesh = 0l;
  m_texture = 0l;
  m_observed = 0l;
  m_partition = 0l;
  m_boundingVolume = new BoundingVolume;

  m_name = "Object";
}

Object::~Object()
{
  if (m_boundingVolume)
    delete m_boundingVolume;
  if (m_partition)
    m_partition->removeObject(this);
  qDeleteAll(m_children);
}

void Object::move(const Vector3f& direction)
{
  m_position += direction;

  // Mark for recalculation
  m_transformModified = true;
}

void Object::scale(const Scaling3f &scale)
{
  m_scale = m_scale * scale;
  m_transformModified = true;

  if (m_boundingVolume) {
    Vector3f size = m_boundingVolume->size();
    size = m_scale * size;
    m_boundingVolume->setSize(size);
  }
}

void Object::setScale(const Scaling3f &scale)
{
  m_scale = scale;
  m_transformModified = true;

  if (m_boundingVolume) {
    Vector3f size = m_boundingVolume->size();
    size = m_scale * size;
    m_boundingVolume->setSize(size);
  }
}

void Object::rotate(const AngleAxisf& rotation)
{
  m_orientation = (m_orientation * rotation).normalized();

  // Mark for recalculation
  m_transformModified = true;
}

void Object::addChild(Object *child)
{
  m_children << child;
  child->setParent(this);
  if (m_partition)
    m_partition->addObject(child);
  else if (this == BGE::Canvas::canvas()->scene())
    BGE::Canvas::canvas()->partition()->addObject(child);
}

void Object::removeChild(Object *child)
{
  if (m_children.removeOne(child)) {
    child->setParent(0l);
    if (m_partition)
      m_partition->removeObject(child);
  }
}

void Object::setMesh(Storage::Mesh *mesh)
{
  m_mesh = mesh;
  delete m_boundingVolume;
  m_boundingVolume = m_mesh->calculateBoundingVolume();
  setRenderable(true);
}

void Object::lookAt(Object *object)
{
  if (!object)
    return;

  // Map object position to our local coordinate system
  Vector3f objPos = parent()->globalOrientation().conjugate() * (object->globalPosition() - parent()->globalPosition());
  objPos += parent()->globalPosition();

  // Calculate the needed params
  Vector3f forward = (objPos - globalPosition()).normalized();
  Vector3f up(0, 1, 0);
  Vector3f side = forward.cross(up).normalized();
  up = side.cross(forward).normalized();

  // Setup transform matrix
  Matrix3f matrix;
  matrix(0,0) = side.x();
  matrix(1,0) = side.y();
  matrix(2,0) = side.z();

  matrix(0,1) = up.x();
  matrix(1,1) = up.y();
  matrix(2,1) = up.z();

  matrix(0,2) = -forward.x();
  matrix(1,2) = -forward.y();
  matrix(2,2) = -forward.z();

  // And convert the rotation matrix to quaternion
  Quaternionf rotation(matrix);
  setOrientation(rotation);
}

Object* Object::objectify(const QString &objectName)
{
  if (!m_mesh || !m_mesh->objects().contains(objectName))
    return 0l;

  Object *newChild = new Object;
  Storage::Mesh *mesh = new Storage::Mesh(objectName);
  mesh->addVertices(objectName, m_mesh->vertices(objectName));
  mesh->addFaces(objectName, m_mesh->faces(objectName));
  mesh->addFacesMaterials(objectName, m_mesh->faceMaterials(objectName));
  mesh->addTextureMaps(objectName, m_mesh->textureMaps(objectName));
  mesh->calculateNormals(objectName);
  newChild->setMesh(mesh);

  foreach (QString materialName, m_mesh->faceMaterials(objectName))
    newChild->addMaterial(materials().value(materialName));

  m_mesh->removeObject(objectName);

  addChild(newChild);
  return newChild;
}

void Object::loadMaterialsFromMesh()
{
  if (!m_mesh)
    return;

  foreach (Storage::Item* item, m_mesh->items()) {
    Storage::Material *material = dynamic_cast<Storage::Material*> (item);
    if (material)
      addMaterial(material);
  }
}

void Object::deattach()
{
  if (!m_parent)
    return;

  m_position = m_globalPosition;
  m_orientation = m_globalOrientation;
  m_transform = m_globalTransform;
  parent()->removeChild(this);
}

void Object::prepareTransforms(qint32 timeDiff)
{
  // Calculate the observing transforms
  if (m_observed && (m_observed->isTransformModified() || isTransformModified()))
    lookAt(m_observed);

  // Calculate any additional transformations
  calculateTransforms(timeDiff);

  // Refresh the transform matrices
  if (isTransformModified()) {
    // Calculate the local transform
    m_transform = Transform3f::Identity();
    m_transform.translate(m_position);
    m_transform *= m_scale;
    m_transform *= m_orientation;

    // And then the global transforms
    if (parent()) {
      // When we have a parent
      m_globalPosition = parent()->m_globalPosition + m_position;
      m_globalOrientation = (parent()->m_globalOrientation * m_orientation).normalized();
      m_globalTransform = parent()->m_globalTransform * m_transform;
    } else {
      // Just copy if we don't have a parent
      m_globalTransform = m_transform;
    }

    m_boundingVolume->setTransform(m_globalTransform);
  }

  // Check if we are still in this octree
  if (m_partition) {
    if (!m_partition->boundingVolume()->isInside(m_boundingVolume)) {
      Partition *partition = m_partition;
      while (partition->parent() && !partition->boundingVolume()->isInside(m_boundingVolume))
        partition = partition->parent();
      if (m_partition != partition) {
        m_partition->removeObject(this);
        partition->addObject(this);
      }
    }
  }

  foreach (Object* child, m_children) {
    // Propagate changes downwards
    if (m_transformModified)
      child->m_transformModified = true;

    // Make the recursion
    child->prepareTransforms(timeDiff);
  }

  postTransformCalculations(timeDiff);
}

void Object::setPartition(Partition *partition)
{
  bool havePartition = m_partition != 0l;
  m_partition = partition;
  if (!havePartition) {
    foreach (Object *object, m_children)
      partition->addObject(object);
  }
}
