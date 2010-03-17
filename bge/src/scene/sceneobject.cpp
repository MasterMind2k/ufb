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
#include "scene/sceneobject.h"

#include <cmath>

#include "rendering/renderer.h"

using namespace BGE;
using namespace BGE::Scene;

SceneObject::SceneObject()
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
}

SceneObject::~SceneObject()
{
  qDeleteAll(m_children);
}

void SceneObject::move(const Vector3f& direction)
{
  m_globalPosition += direction;
  m_position += direction;

  // Mark for recalculation
  m_transformModified = true;
}

void SceneObject::rotate(const AngleAxisf& rotation)
{
  m_orientation = (m_orientation * rotation).normalized();

  // Mark for recalculation
  m_transformModified = true;
}

void SceneObject::lookAt(SceneObject *object)
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
  Transform3f matrix;
  matrix(0,0) = side.x();
  matrix(1,0) = side.y();
  matrix(2,0) = side.z();
  matrix(3,0) = 0;

  matrix(0,1) = up.x();
  matrix(1,1) = up.y();
  matrix(2,1) = up.z();
  matrix(3,1) = 0;

  matrix(0,2) = -forward.x();
  matrix(1,2) = -forward.y();
  matrix(2,2) = -forward.z();
  matrix(3,2) = 0;

  matrix(0,3) = 0;
  matrix(1,3) = 0;
  matrix(2,3) = 0;
  matrix(3,3) = 1;

  // And get the rotation :)
  Quaternionf rotation(matrix.rotation());
  setOrientation(rotation);
}

void SceneObject::prepareTransforms()
{
  // Calculate the observing transforms
  if (m_observed && (m_observed->isTransformModified() || isTransformModified()))
    lookAt(m_observed);

  // Calculate any additional transformations
  calculateTransforms();

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
      m_globalPosition = m_position;
      m_globalOrientation = m_orientation;
    }
  }

  foreach (SceneObject* child, m_children) {
    // Propagate changes downwards
    if (m_transformModified)
      child->m_transformModified = true;

    // Make the recursion
    child->prepareTransforms();
  }
}