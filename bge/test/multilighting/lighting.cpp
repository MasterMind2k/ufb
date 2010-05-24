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
#include "lighting.h"

#include <ctime>

#include <QtCore/QTimer>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "canvas.h"
#include "motionstate.h"

#include "scene/light.h"
#include "scene/boundingvolume.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/material.h"
#include "storage/shaderprogram.h"

Lighting::Lighting()
  : QObject()
{
  qsrand(time(0l));
  BGE::Storage::Material *material = BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/materials/Light");
  // Setup lights
  for (quint16 i = 0; i < 15; i ++) {
    QString lightName = "Light_" + QString::number(i);
    BGE::Scene::Light *light = BGE::Canvas::canvas()->createLight(lightName);
    BGE::Canvas::canvas()->addSceneObject(light);
    light->setQuadraticAttenuation(0.0005);
    light->move(qrand() % 500 - 300, 50, qrand() % 500 - 300);
    light->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
    light->addMaterial(material);

    // Make rigid body
    btRigidBody *body = new btRigidBody(1, new BGE::MotionState(light), new btBoxShape(btVector3(light->boundingVolume()->size().x(), light->boundingVolume()->size().y(), light->boundingVolume()->size().z())), btVector3(0, 0, 0));
    m_bodies << body;
    BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(body);

    // Setup forces
    body->applyCentralImpulse(btVector3(qrand() % 100 - 50, 0, qrand() % 100 - 50));
    body->setRestitution(1);
    body->setGravity(btVector3(0, 0, 0));
    body->applyGravity();
  }

  QTimer *timer = new QTimer(this);
  timer->setInterval(5000);
  timer->start();
  connect(timer, SIGNAL(timeout()), SLOT(setDirections()));
}

void Lighting::setDirections()
{
  foreach (btRigidBody *body, m_bodies)
    body->applyCentralImpulse(btVector3(qrand() % 100 - 50, 0, qrand() % 100 - 50));
}
