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
  Vector3f position;
  Vector3f velocity;
  qreal colorWeight;
  qreal alpha;
  quint32 lifetime;
  QVariant custom;
};

class ParticleEmitter : public Object
{
  public:
    inline ParticleEmitter()
    {
      setRenderable(true);
      m_verticesBufferId = m_indicesBufferId = 0;
    }

    inline const QList<Particle> &particles() const
    {
      return m_particles;
    }

    inline void emitParticle(const Particle &particle)
    {
      m_particles << particle;
    }

  protected:
    inline void postTransformCalculations(qint32 timeDiff)
    {
      for (qint32 i = 0; i < m_particles.size(); i++)
        calculateParticle(m_particles[i], timeDiff);
    }

    virtual void calculateParticle(Particle &particle, qint32 timeDiff) = 0;

  private:
    QList<Particle> m_particles;
    quint32 m_verticesBufferId;
    quint32 m_indicesBufferId;

    friend class Driver::GL3;
};

}
}

#endif
