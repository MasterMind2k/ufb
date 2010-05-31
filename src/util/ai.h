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
#ifndef AI_H
#define AI_H

#include "global.h"

#include <QtCore/QTime>

namespace Objects {
class Fighter;
}

namespace Util {

class Ai
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    inline Ai(Objects::Fighter *target) : m_target(target), m_running(false)
    {
      m_locationChange.start();
    }

    inline void setTarget(Objects::Fighter *target)
    {
      m_target = target;
    }

    void calculateAngularVelocity();

    inline void setControlled(Objects::Fighter *controlled)
    {
      m_controlled = controlled;
    }

  private:
    Vector3f m_fleeLocation;
    Objects::Fighter *m_target;
    Objects::Fighter *m_controlled;
    QTime m_locationChange;
    bool m_running;

    inline static float roll(const Vector3f &target)
    {
      float roll = 0;
      Vector3f rollVector = Vector3f(target.x(), target.y(), 0).normalized();
      if (rollVector != Vector3f::UnitY()) {
        roll = acos(rollVector.dot(Vector3f::UnitY()));

        roll *= rollVector.cross(Vector3f::UnitY()).z() > 0 ? -1 : 1;
      }
      return roll;
    }

    inline static float pitch(const Vector3f &target)
    {
      float pitch = 0;

      Vector3f normalized = Vector3f(0, target.y(), target.z()).normalized();
      pitch = acos(normalized.dot(-Vector3f::UnitZ()));

      if (target.z() < 0)
        pitch *= normalized.cross(-Vector3f::UnitZ()).x() > 0 ? -1 : 1;

      return pitch;
    }
};

}

#endif
