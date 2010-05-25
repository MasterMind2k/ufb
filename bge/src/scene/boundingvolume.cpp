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
#include "boundingvolume.h"

using namespace BGE;
using namespace BGE::Scene;

bool BoundingVolume::isInside(const BoundingVolume *boundingVolume) const
{
  return m_radius > boundingVolume->m_radius && isInside(boundingVolume->transformedCenter());
}

bool BoundingVolume::isInside(const Vector3f &point) const
{
  // Let's perform min-max test :D
  return (max().cwise() >= point).all() && (min().cwise() <= point).all();
}

void BoundingVolume::calculateCorners()
{
  m_corners.clear();
  Vector3f size = m_size / 2.0;
  for (quint8 i = 0; i < 8; i++)
    m_corners << (m_center + (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * size));
}

void BoundingVolume::calculateTransformedCorners() const
{
  m_transformedCorners.clear();
  foreach (Vector3f corner, m_corners)
    m_transformedCorners << m_transform * (corner);

  m_isCacheValid = true;
}

void BoundingVolume::calculateRadius()
{
  m_radius = 0;
  foreach (Vector3f corner, m_corners)
    m_radius = qMax((m_center - corner).norm(), m_radius);
}
