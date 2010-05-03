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
  QVariant custom;
};

class ParticleEmitter : public Object
{
  public:
    inline ParticleEmitter()
    {
      setRenderable(true);
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

  private:
    QList<Particle> m_particles;
};

}
}

#endif
