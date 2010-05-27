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
class GL1;
}

namespace Scene {

/**
 * A particle representation. Should be used only inside ParticleEmitter.
 */
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

/**
 * A particle emitter. To use it, you have to reimplement at least the
 * calculateParticle method. To gradualy spawn particles, also reimplement
 * the spawnParticles method.
 *
 * To emit a particle, call the emitParticle method.
 *
 * @note If you want to continously spawn particles, don't forget to disable
 * autoremove.
 *
 * @note it does not use the Bullet physics engine.
 *
 * @see calculateParticle
 * @see spawnParticles
 * @see emitParticle
 */
class ParticleEmitter : public Object
{
  public:
    inline ParticleEmitter() : m_autoremove(true), m_lifetime(0)
    {
      m_verticesBufferId = m_indicesBufferId = 0;
    }

    /**
     * Returns the autoremove property.
     */
    inline bool autoremove() const
    {
      return m_autoremove;
    }

    /**
     * Current lifetime of the particle emitter.
     */
    inline qint32 lifetime() const
    {
      m_lifetime;
    }

  protected:
    void postTransformCalculations(qint32 timeDiff);

    /**
     * Calculates the particle properties.
     *
     * Reimplement this method to define your animation.
     */
    virtual void calculateParticle(Particle &particle, qint32 timeDiff) = 0;
    /**
     * This method is called after all calculations are made. Reimplement it
     * to add new particles.
     */
    inline virtual void spawnParticles(qint32 timeDiff) {};

    /**
     * Emits the newly created particle.
     *
     * This method can be used anywhere. It is not neccessary to be used only
     * inside spawnParticles method.
     */
    inline void emitParticle(const Particle &particle)
    {
      setRenderable(true);
      m_particles << particle;
    }

    /**
     * Sets the autoremove.
     *
     * When there are no particles the particle emitter automaticaly removes
     * itself.
     *
     * @note Particle gets removed, when it's alpha is 0.
     */
    inline void setAutoremove(bool autoremove)
    {
      m_autoremove = autoremove;
    }

  private:
    QList<Particle> m_particles;
    quint32 m_verticesBufferId;
    quint32 m_indicesBufferId;
    bool m_autoremove;
    quint32 m_lifetime;

    inline const QList<Particle> &particles() const
    {
      return m_particles;
    }

    friend class Driver::GL3;
    friend class Driver::GL1;
};

}
}

#endif
