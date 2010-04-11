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
#ifndef __BGE_SIZE_H
#define __BGE_SIZE_H

#include "global.h"

namespace BGE {

class Size
{
  public:
    inline Size()
    {
      m_vector = Vector3f::Zero();
    }
    inline Size(float width, float height, float depth)
    {
      m_vector.x() = width;
      m_vector.y() = height;
      m_vector.z() = depth;
    }
    inline Size(const Vector3f &vector)
    {
      m_vector = vector;
    }

    inline float &width()
    {
      return m_vector.x();
    }
    inline float &height()
    {
      return m_vector.y();
    }
    inline float &depth()
    {
      return m_vector.z();
    }
    inline const float width() const
    {
      return m_vector.x();
    }
    inline const float height() const
    {
      return m_vector.y();
    }
    inline const float depth() const
    {
      return m_vector.z();
    }

    inline Vector3f &vector()
    {
      return m_vector;
    }
    inline const Vector3f &vector() const
    {
      return m_vector;
    }

    inline Size operator/(float divider) const
    {
      return Size(m_vector / divider);
    }

    QVector<Vector3f> points(const Vector3f& center) const;

  private:
    Vector3f m_vector;
};

inline QDebug operator<<(QDebug dbg, const Size &size)
{
  dbg.nospace() << "Size(" << size.width() << ", " << size.height() << ", " << size.depth();
  return dbg.space();
}

}

#endif
