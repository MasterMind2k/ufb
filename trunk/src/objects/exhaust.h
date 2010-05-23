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
#ifndef OBJECTS_EXHAUST_H
#define OBJECTS_EXHAUST_H

#include "scene/particleemitter.h"

namespace Objects {
class Fighter;

class Exhaust : public BGE::Scene::ParticleEmitter
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Exhaust(Fighter *fighter);
    ~Exhaust();

  private:
    Fighter *m_fighter;
    Transform3f m_previousTransform;

    void calculateParticle(BGE::Scene::Particle &particle, qint32 timeDiff);
    void spawnParticles(qint32 timeDiff);
};

}

#endif
