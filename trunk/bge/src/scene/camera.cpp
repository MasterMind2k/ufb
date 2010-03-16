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
#include "camera.h"

#include <QtCore/QString>

#include <QtGui/QMatrix4x4>

using namespace BGE;
using namespace BGE::Scene;

quint32 Camera::m_serialNumber = 0;

Camera::Camera(const QString& name)
: SceneObject()
{
  if (name.isEmpty())
    m_name = "Camera_" + QString::number(m_serialNumber);
  else
    m_name = name;

  m_observed = 0l;
}

void Camera::lookAt(SceneObject *object)
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

void Camera::calculateTransforms()
{
  if (m_observed && (m_observed->isTransformModified() || isTransformModified()))
    lookAt(m_observed);
}
