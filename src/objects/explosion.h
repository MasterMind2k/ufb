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
#ifndef OBJECTS_EXPLOSION_H
#define OBJECTS_EXPLOSION_H

#include "scene/particleemitter.h"

namespace Objects {

class Explosion : public BGE::Scene::ParticleEmitter
{
  public:
    enum Sizes {
      Large,
      Medium,
      Small
    };
    Explosion(const Vector3f &position, Sizes size);
    ~Explosion();

  private:
    void calculateParticle(BGE::Scene::Particle &particle, qint32 timeDiff);
    qreal m_lifetime;
    qreal m_particleSize;
};

}

#endif
