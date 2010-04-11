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
#include "size.h"

using namespace BGE;

QVector<Vector3f> Size::points(const Vector3f &center) const
{
  QVector<Vector3f> output;
  output.reserve(8);
  Vector3f size = m_vector / 2.0;
  for (quint8 i = 0; i < 8; i++)
    output << (center + (Vector3f(i & 0x01 ? -1 : 1, i & 0x02 ? -1 : 1, i & 0x04 ? -1 : 1).cwise() * size));
  return output;
}
