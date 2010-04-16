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

#include "canvas.h"

#include "scene/boundingvolume.h"

using namespace BGE;
using namespace BGE::Scene;

quint32 Camera::m_serialNumber = 0;
Transform3f Camera::m_projection(Transform3f::Identity());

Containment Camera::isSphereInFrustrum(const BoundingVolume *boundingVolume) const
{
  quint8 c = 0;

  for (QVector<Plane>::const_iterator i = m_frustrum.constBegin(); i != m_frustrum.constEnd(); i++) {
    Plane plane = *i;
    float d;
    d = plane.signedDistance(boundingVolume->transformedCenter());

    if(d <= -boundingVolume->radius())
      return Outside;
    if(d > boundingVolume->radius())
      c++;
  }
  return (c == 6) ? FullyInside : PartialyInside;
}

Containment Camera::isBoxInFrustrum(const BoundingVolume *boundingVolume) const
{
  quint8 c2 = 0;

  for (QVector<Plane>::const_iterator i = m_frustrum.constBegin(); i != m_frustrum.constEnd(); i++) {
    Plane plane = *i;
    quint8 c = 0;

    foreach (Vector3f corner, boundingVolume->transformedCorners()) {
      if (plane.signedDistance(corner) > 0)
        c++;
    }

    if (!c)
      return Outside;
    if (c == 8)
      c2++;
  }
  return c2 == 6 ? FullyInside : PartialyInside;
}

void Camera::calculateCameraTransform() const
{
  Transform3f move(Transform3f::Identity()), rotation(Transform3f::Identity());
  // Calculate camera translation
  move.translate(-globalPosition());

  // Calculate camera rotation
  rotation *= orientation().inverse();

  // And do some fixing for local camera
  if (parent() != Canvas::canvas()->scene()) {
    rotation.translate(-position());
    rotation.rotate(orientation() * globalOrientation().inverse());
    rotation.translate(position());
  }

  m_cameraTransform = rotation * move;
}

void Camera::calculateFrustrum()
{
  if (!isTransformModified())
    return;

  m_frustrum.clear();

  Transform3f move(Transform3f::Identity()), rotation(Transform3f::Identity());
  // Calculate camera translation
  move.translate(globalPosition());

  // Calculate camera rotation
  rotation *= orientation().inverse();

  // And do some fixing for local camera
  if (parent() != Canvas::canvas()->scene()) {
    rotation.translate(-position());
    rotation.rotate(orientation() * globalOrientation());
    rotation.translate(position());
  }

  Matrix4f frustrumMatrix = m_projection.matrix() * cameraTransform().matrix();
  // Right
  Vector4f temp = frustrumMatrix.row(3) - frustrumMatrix.row(0);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
  // Left
  temp = frustrumMatrix.row(3) + frustrumMatrix.row(0);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
  // Bottom
  temp = frustrumMatrix.row(3) + frustrumMatrix.row(1);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
  // Top
  temp = frustrumMatrix.row(3) - frustrumMatrix.row(1);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
  // Far
  temp = frustrumMatrix.row(3) - frustrumMatrix.row(2);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
  // Near
  temp = frustrumMatrix.row(3) + frustrumMatrix.row(2);
  temp.normalize();
  m_frustrum << Plane(temp.segment(0, 3), temp(3));
}

Camera::Camera(const QString& name)
: Object()
{
  m_frustrum.reserve(6);
  if (name.isEmpty())
    m_name = "Camera_" + QString::number(m_serialNumber);
  else
    m_name = name;
}
