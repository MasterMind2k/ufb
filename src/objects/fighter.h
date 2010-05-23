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
#ifndef OBJECTS_FIGHTER_H
#define OBJECTS_FIGHTER_H

#include "object.h"

namespace Objects {

class Fighter : public Object
{
  public:
    Fighter();

    inline void setEnginePower(qreal enginePower)
    {
      m_enginePower = enginePower;
      if (m_enginePower > MaxPower)
        m_enginePower = MaxPower;
      else if (m_enginePower < 0)
        m_enginePower = 0;
    }
    inline qreal enginePower() const
    {
      return m_enginePower;
    }
    inline void setAngularVelocity(const Vector3f &velocity)
    {
      m_angularVelocity = velocity;
      if (m_angularVelocity.x() > 1)
        m_angularVelocity.x() = 1;
      if (m_angularVelocity.y() > 1)
        m_angularVelocity.y() = 1;
      if (m_angularVelocity.z() > 1)
        m_angularVelocity.z() = 1;
    }
    inline const Vector3f angularVelocity() const
    {
      return m_angularVelocity;
    }

    void initBody();

    static const qreal MaxPower = 1000000;
    static const qreal MaxVelocity = 1435;

  private:
    qreal m_enginePower;
    Vector3f m_angularVelocity;

    void calculateTransforms(qint32 timeDiff);
};

}

#endif
