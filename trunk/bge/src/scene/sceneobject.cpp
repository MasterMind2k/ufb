/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
 *   Copyright (C) 2010 by Matevž Pesek    <be inserted>                   *
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
  m_transform.setIdentity();
  m_globalTransform.setIdentity();
  m_globalOrientation.setIdentity();
  m_orientation.setIdentity();
  m_position.setZero();
  m_globalPosition.setZero();
  m_parent = 0l;
  m_transformModified = false;
}

void SceneObject::move(Vector3f direction)
{
  m_globalPosition += direction;
  m_position += direction;
  m_transformModified = true;
}

void SceneObject::rotate(AngleAxisf rotation)
{
  m_orientation = m_orientation * rotation;
  m_orientation.normalize();
  m_globalOrientation = m_globalOrientation * rotation;
  m_globalOrientation.normalize();
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

Transform3f SceneObject::transform() const
{
  return m_transform;
}

Vector3f SceneObject::globalPosition() const
{
  return m_globalPosition;
}

Vector3f SceneObject::position() const
{
  return m_position;
}

Quaternionf SceneObject::globalOrientation() const
{
  return m_globalOrientation;
}

Quaternionf SceneObject::orientation() const
{
  return m_orientation;
}

void SceneObject::prepareTransforms()
{
  calculateTransforms();
  if (isTransformModified()) {
    m_transform = Transform3f::Identity();
    m_transform.translate(m_position);
    m_transform *= m_orientation;
    if (parent()) {
      m_globalPosition = parent()->m_globalPosition + m_position;
      m_globalOrientation = parent()->m_globalOrientation * m_orientation;
      m_globalTransform = parent()->m_globalTransform * m_transform;
    } else {
      m_globalTransform = m_transform;
      m_globalPosition = m_position;
      m_globalOrientation = m_orientation;
    }
  }

  foreach (SceneObject* child, m_childs) {
    // Propagate changes downwards
    if (isTransformModified())
      child->m_transformModified = true;
    child->prepareTransforms();
  }
  m_transformModified = false;
}
