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
#include "particleemitter.h"

#include "canvas.h"

using namespace BGE;
using namespace BGE::Scene;

void ParticleEmitter::postTransformCalculations(qint32 timeDiff)
{
  if (m_autoremove && m_particles.isEmpty()) {
    setRenderable(false);
    // Remove itself
    parent()->removeChild(this);
    Canvas::canvas()->deleteSceneObject(this);
    return;
  }

  QList<quint16> removeList;
  for (quint16 i = 0; i < m_particles.size(); i++) {
    calculateParticle(m_particles[i], timeDiff);
    if (m_particles.at(i).alpha < 0.01)
      removeList << i;
  }

  // Remove invisible particles
  for (qint32 i = removeList.size() - 1; i >= 0; i--)
    m_particles.removeAt(i);
}
