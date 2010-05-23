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

using namespace Objects;

#include "canvas.h"

#include "scene/boundingvolume.h"
#include "scene/light.h"

#include "storage/material.h"

Explosion::Explosion(const Vector3f &position, Sizes size)
  : BGE::Scene::ParticleEmitter()
{
  qreal a;
  int speed;
  quint16 particles;
  switch (size) {
    case Large:
      break;
    case Small:
      a = 400;
      speed = 40;
      m_lifetime = 1500;
      particles = 500;
      break;
  }

  setBoundingVolume(new BGE::Scene::BoundingVolume(Vector3f::Zero(), Vector3f(a, a, a)));

  for (quint16 i = 0; i < particles; i++) {
    BGE::Scene::Particle particle;
    particle.alpha = 5;
    particle.colorWeight = 0;
    particle.lifetime = 0;
    particle.position = position;
    particle.velocity = Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized();
    particle.velocity *= qrand() % speed + speed * 2;
    particle.initialVelocity = particle.velocity;

    emitParticle(particle);
  }

  BGE::Storage::Material *material = new BGE::Storage::Material("Particles");
  material->setEmission(QColor(255, 136, 0));
  material->setAmbient(QColor(40, 40, 40));
  material->setDiffuse(QColor(40, 40, 40));
  addMaterial(material);

  move(position);

  // Add light
  BGE::Scene::Light *light = BGE::Canvas::canvas()->createLight(QString("Explosive light_%0").arg((int) this));
  light->setAmbientColor(QColor(180, 96, 0));
  light->setDiffuseColor(QColor(255, 136, 0));
  light->setSpecularColor(Qt::white);
  light->setQuadraticAttenuation(0.2);
  addChild(light);
}

Explosion::~Explosion()
{
  BGE::Canvas::canvas()->removeLight(static_cast<BGE::Scene::Light*> (child(0))->name());
}

void Explosion::calculateParticle(BGE::Scene::Particle &particle, qint32 timeDiff)
{
  // Increase light
  static_cast<BGE::Scene::Light*> (child(0))->setQuadraticAttenuation(0.00001 * (1 - particle.alpha / 5.0) * (1 - particle.alpha / 5.0) + 0.000001);

  particle.lifetime += timeDiff;
  particle.position += particle.velocity * timeDiff / 1000.0;
  particle.velocity = particle.initialVelocity * (1.0 - (qreal) particle.lifetime / m_lifetime);
  if (particle.alpha > 0.0)
    particle.alpha = 5.0 - (5.0 * (qreal) particle.lifetime / m_lifetime);
}
