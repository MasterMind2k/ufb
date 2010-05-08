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
#include "explosion.h"

#include "scene/boundingvolume.h"

#include "storage/material.h"

void Explosion::init()
{
  for (quint16 i = 0; i < 1000; i++) {
    BGE::Scene::Particle particle;
    particle.alpha = 1;
    particle.colorWeight = 0;
    particle.lifetime = 0;
    particle.position = Vector3f::Zero();
    particle.velocity = Vector3f(qrand() % 20 - 10, qrand() % 60 - 30, qrand() % 20 - 10).normalized();
    particle.velocity *= 20;
    particle.initialVelocity = particle.velocity;

    emitParticle(particle);
  }

  BGE::Storage::Material *material = new BGE::Storage::Material("Particles");
  material->setEmission(QColor(255, 136, 0));
  material->setAmbient(QColor(20, 20, 20));
  material->setDiffuse(QColor(20, 20, 20));
  addMaterial(material);

  setBoundingVolume(new BGE::Scene::BoundingVolume(Vector3f::Zero(), Vector3f(200, 200, 200)));
}

void Explosion::calc(BGE::Scene::Particle &particle, qint32 timeDiff)
{
  particle.lifetime += timeDiff;
  particle.position += particle.velocity * timeDiff / 1000.0;
  particle.velocity = particle.initialVelocity * (1.0 - (qreal) particle.lifetime / 3000.0);
  if (particle.alpha > 0.0)
    particle.alpha = 1.0 - ((qreal) particle.lifetime / 3000.0);
  if (particle.colorWeight < 1.0) {
    particle.colorWeight = ((qreal) particle.lifetime / 2500.0);
    if (particle.colorWeight > 1.0)
      particle.colorWeight = 1.0;
  }
}
