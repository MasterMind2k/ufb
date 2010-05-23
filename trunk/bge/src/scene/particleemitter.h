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
#ifndef __BGE_SCENE_PARTICLEEMITTER_H
#define __BGE_SCENE_PARTICLEEMITTER_H

#include <QtCore/QVariant>

#include "scene/object.h"

namespace BGE {
namespace Driver {
class GL3;
}

namespace Scene {

struct Particle {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Vector3f position;
  Vector3f velocity;
  Vector3f initialVelocity;
  qreal colorWeight;
  qreal alpha;
  quint32 lifetime;
  QVariant custom;
};

class ParticleEmitter : public Object
{
  public:
  inline ParticleEmitter() : m_autoremove(true), m_lifetime(0)
    {
      m_verticesBufferId = m_indicesBufferId = 0;
    }

    inline const QList<Particle> &particles() const
    {
      return m_particles;
    }

    inline void emitParticle(const Particle &particle)
    {
      setRenderable(true);
      m_particles << particle;
    }

    inline void setAutoremove(bool autoremove)
    {
      m_autoremove = autoremove;
    }
    inline bool autoremove() const
    {
      return m_autoremove;
    }

    inline qint32 lifetime() const
    {
      m_lifetime;
    }

  protected:
    void postTransformCalculations(qint32 timeDiff);

    virtual void calculateParticle(Particle &particle, qint32 timeDiff) = 0;
    inline virtual void spawnParticles(qint32 timeDiff) {};

  private:
    QList<Particle> m_particles;
    quint32 m_verticesBufferId;
    quint32 m_indicesBufferId;
    bool m_autoremove;
    quint32 m_lifetime;

    friend class Driver::GL3;
};

}
}

#endif
