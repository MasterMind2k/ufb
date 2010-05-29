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

#include <QtCore/QTime>

namespace Util{
class Ai;
}

namespace Objects {
class Bullet;

class Fighter : public Object
{
  public:
    Fighter(Util::Ai *ai = 0l);

    inline void setEnginePower(qreal enginePower)
    {
      if (m_hullIntegrity <= 0.0)
        return;

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
      if (m_hullIntegrity <= 0.0)
        return;

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

    void fire();

    static qreal MaxPower;
    static qreal MaxVelocity;
    static quint16 ShieldsRechargeTime;
    static float ShieldsRechargeTick;

    inline float hullIntegrity() const
    {
      return m_hullIntegrity;
    }
    inline float shields() const
    {
      return m_shields;
    }

    inline bool hasWeaponsLock() const
    {
      return m_lockedTarget != 0l && m_acquiringTarget.elapsed() > 500;
    }

    inline void setWeaponsLock(Object *target)
    {
      if (m_lockedTarget != target)
        m_acquiringTarget.restart();
      m_lockedTarget = target;
    }

  private:
    qreal m_enginePower;
    Vector3f m_angularVelocity;
    QTime m_previousShot;
    QTime m_shieldsRecharge;
    QTime m_acquiringTarget;
    Util::Ai *m_ai;
    float m_shields;
    float m_hullIntegrity;
    qint32 m_dyingElapsedTime;
    qint32 m_previousExplosion;
    bool m_exploded;
    Object *m_lockedTarget;

    void calculateTransforms(qint32 timeDiff);
    void collision(BGE::Scene::Object *object);
    void lockLaser(Bullet *bullet, bool isLeft) const;
};

}

#endif
