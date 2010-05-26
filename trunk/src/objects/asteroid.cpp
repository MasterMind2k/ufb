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
#include "asteroid.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "canvas.h"

#include "scene/camera.h"
#include "scene/boundingvolume.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "util/objectlist.h"

#include "explosion.h"

using namespace Objects;

Asteroid::Asteroid(Sizes size)
  : m_size(size),
    m_dyingElapsedTime(0),
    m_previousExplosion(0),
    m_exploded(false),
    m_stabilized(false)
{
  QList<BGE::Storage::Item*> meshes = BGE::Storage::Manager::self()->get("/asteroids/models")->items();
  setMesh(static_cast<BGE::Storage::Mesh*> (meshes.at(qrand() % meshes.size())));
  loadMaterialsFromMesh();

  // Scale it to the designated size
  scale(radius() / boundingVolume()->radius());

  setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/asteroids/textures/" + mesh()->name()));
  switch (m_size) {
    case Large:
      setMass(2000);
      m_structuralIntegrity = 400;
      break;

    case Medium:
      setMass(1500);
      m_structuralIntegrity = 200;
      break;

    case Small:
      setMass(1000);
      m_structuralIntegrity = 150;
      break;
  }

  setName("Asteroid");
  setRegistered(true);
}

qreal Asteroid::radius() const
{
  switch (m_size) {
    case Large:
      return 3000;

    case Medium:
      return 1000;

    case Small:
      return 500;
  }
}

void Asteroid::postTransformCalculations(qint32 timeDiff)
{
  // Check if we are too fast
  if (!m_stabilized) {
    if (velocity().norm() > 2000.0) {
      body()->setDamping(0.2, 0);
    } else {
      m_stabilized = true;
      body()->setDamping(0, 0);
    }
  }

  if (m_structuralIntegrity <= 0.0) {
    // Let's remove ourselves from the list
    setRegistered(false);

    // Dying animation :)
    m_dyingElapsedTime += timeDiff;
    qreal distance = boundingVolume()->radius();
    if (m_dyingElapsedTime < 4000.0) {
      if (m_previousExplosion > 200) {
        qreal pos = distance - qrand() % (int) (distance / 2.0);
        BGE::Canvas::canvas()->addSceneObject(new Explosion(globalPosition() + pos * Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized(), Explosion::Medium));
        m_previousExplosion = 0;
      } else {
        m_previousExplosion += timeDiff;
      }
    } else if (!m_exploded) {
      // Final explosion
      quint8 explosions = qrand() % 4 + 3;
      for (quint8 i = 0; i < explosions; i++) {
        qreal pos = distance / 2.0;
        BGE::Canvas::canvas()->addSceneObject(new Explosion(globalPosition() + pos * Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized(), Explosion::Large));
      }
      m_exploded = true;

      // Remove itself from physics world
      BGE::Canvas::canvas()->dynamicsWorld()->removeRigidBody(body());

      // Create new asteroids
      spawn();

    } else if (m_dyingElapsedTime < 5000.0) {
      scale(1 - (m_dyingElapsedTime - 4000.0) / 1000.0);
    } else {
      // Remove itself
      setRenderable(false);
      parent()->removeChild(this);
      BGE::Canvas::canvas()->deleteSceneObject(this);
    }
  }

  Object::postTransformCalculations(timeDiff);
}

void Asteroid::collision(BGE::Scene::Object *object)
{
  // We are already dead
  if (m_structuralIntegrity < 0.0)
    return;

  m_structuralIntegrity -= 10;
}

void Asteroid::spawn()
{
  if (m_size == Small)
    return;

  Sizes size;
  quint8 asteroids;
  switch (m_size) {
    case Large:
      size = Medium;
      asteroids = qrand() % 4 + 1;
      break;

    case Medium:
      size = Small;
      asteroids = qrand() % 3 + 1;
      break;
  }

  for (quint8 i = 0; i < asteroids; i++) {
    Asteroid *asteroid = new Asteroid(size);
    asteroid->move(globalPosition());
    asteroid->initBody();
    asteroid->setAngularVelocity(Vector3f(qrand() % 60 - 30, qrand() % 60 - 30, qrand() % 60 - 30).normalized());
    BGE::Canvas::canvas()->addSceneObject(asteroid);
  }
}
