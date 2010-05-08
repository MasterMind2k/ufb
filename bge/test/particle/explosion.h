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
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "scene/particleemitter.h"

class Explosion : public BGE::Scene::ParticleEmitter
{
  public:
    inline Explosion() : BGE::Scene::ParticleEmitter()
    {
      init();
    }

    void init();

  protected:
    inline void calculateParticle(BGE::Scene::Particle &particle, qint32 timeDiff)
    {
      calc(particle, timeDiff);
    }

  private:
    void calc(BGE::Scene::Particle &particle, qint32 timeDiff);
};

#endif
