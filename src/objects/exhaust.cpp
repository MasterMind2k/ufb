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
#include "exhaust.h"

#include "canvas.h"

#include "storage/material.h"

#include "scene/boundingvolume.h"
#include "scene/light.h"
#include "scene/camera.h"

#include "fighter.h"

using namespace Objects;

Exhaust::Exhaust(Fighter *fighter)
  : m_fighter(fighter)
{
  setAutoremove(false);

  BGE::Storage::Material *material = new BGE::Storage::Material("Particles");
  material->setEmission(QColor(255, 136, 0));
  material->setAmbient(QColor(10, 10, 10));
  material->setDiffuse(QColor(40, 40, 40));
  addMaterial(material);

  setBoundingVolume(new BGE::Scene::BoundingVolume(Vector3f(10, 10, 100), Vector3f(100, 100, 200)));
  m_previousPosition.setZero();

  // Add light
  BGE::Scene::Light *light = BGE::Canvas::canvas()->createLight(QString("Exhaust light_%0").arg((int) this));
  light->setAmbientColor(QColor(180, 96, 0));
  light->setAmbientColor(QColor(180, 96, 0));
  light->setDiffuseColor(QColor(255, 136, 0));
  light->setSpecularColor(Qt::white);
  light->setQuadraticAttenuation(1);
  light->move(0, -40, 40);
  light->rotateY(180);
  addChild(light);
}

Exhaust::~Exhaust()
{
  BGE::Canvas::canvas()->removeLight(static_cast<BGE::Scene::Light*> (child(0))->name());
}

void Exhaust::calculateParticle(BGE::Scene::Particle &particle, qint32 timeDiff)
{
  particle.lifetime += timeDiff;
  particle.position += particle.velocity * timeDiff / 1000.0;

  particle.alpha = qMax(1.0 - particle.lifetime / 150.0, 0.0);
}

void Exhaust::spawnParticles(qint32 timeDiff)
{
  qreal usage = m_fighter->enginePower() / Fighter::MaxPower;

  Vector3f currentPosition = globalTransform() * Vector3f(0, 0, 0);
  float previousAlpha = (1.0 - timeDiff / 150.0);

  quint16 particles = 300 * usage;
  if (!particles) {
    m_previousPosition = currentPosition;
    m_previousOrientation = globalOrientation();
    static_cast<BGE::Scene::Light*> (child(0))->setQuadraticAttenuation(1.0);
    return;
  }

  static_cast<BGE::Scene::Light*> (child(0))->setQuadraticAttenuation(0.001 - usage * 0.001 + 0.0001);

  qreal n = 1.0 / particles;
  for (quint16 i = 0; i < particles; i++) {
    BGE::Scene::Particle particle;
    particle.size = 3.0;
    particle.alpha = 1.0 - i * n * (1.0 - previousAlpha);
    particle.colorWeight = 0;
    particle.lifetime = i * n * timeDiff;

    float t = particle.lifetime;
    Quaternionf orientation = globalOrientation().slerp(t / timeDiff, m_previousOrientation);
    particle.velocity = orientation * Vector3f(qrand() % 4 - 2, qrand() % 4 - 2, 50).normalized() * 2000 * usage;

    // Interpolating points
    particle.position = currentPosition - i * n * (currentPosition - m_previousPosition) + particle.velocity * particle.lifetime / 1000.0 + orientation * Vector3f(qrand() % 20 - 10, qrand() % 20 - 10, 0);

    particle.initialVelocity = particle.velocity;
    emitParticle(particle);
  }

  m_previousPosition = currentPosition;
  m_previousOrientation = globalOrientation();
}
