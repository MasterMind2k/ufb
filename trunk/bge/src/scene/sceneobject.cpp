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
  m_orientation = m_orientation * rotation;
  m_globalOrientation = m_globalOrientation * rotation;

  // Mark for recalculation
  m_transformModified = true;
}

void SceneObject::prepareTransforms()
{
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
      m_globalOrientation = parent()->m_globalOrientation * m_orientation;
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

  // The transforms got calculated
  m_transformModified = false;
}
