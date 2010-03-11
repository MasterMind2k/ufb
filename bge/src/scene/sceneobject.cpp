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
  // Set the default position and orientation
  m_orientation.setIdentity();
  m_globalOrientation.setIdentity();
  m_position.setZero();
  m_globalPosition.setZero();

  // And init other properties
  m_parent = 0l;
  m_transformModified = false;
  m_isBindable = false;
  m_meshId = 0;
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

void SceneObject::rotateX(qreal angle)
{
  AngleAxisf rotation(angle, Vector3f::UnitX());
  rotate(rotation);
}

void SceneObject::rotateY(qreal angle)
{
  AngleAxisf rotation(angle, Vector3f::UnitY());
  rotate(rotation);
}

void SceneObject::rotateZ(qreal angle)
{
  AngleAxisf rotation(angle, Vector3f::UnitZ());
  rotate(rotation);
}

void SceneObject::bindMesh(Rendering::Renderer* renderer, quint32 bindId)
{
  /// @TODO render the model

  m_meshId = bindId;
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
    if (isTransformModified())
      child->m_transformModified = true;

    // Make the recursion
    child->prepareTransforms();
  }

  // The transforms got calculated
  m_transformModified = false;
}
