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
#include "movinglight.h"

#include <QtCore/QTimer>

#include "scene/light.h"

MovingLight::MovingLight()
 : QObject(), BGE::Scene::SceneObject()
{
  QTimer* timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(15000);
  timer->start();

  connect(timer, SIGNAL(timeout()), SLOT(change()));
}

void MovingLight::calculateTransforms()
{
  rotateY(M_PI / 150);
}

void MovingLight::change()
{
  qDebug("Changing light type.");
  foreach (BGE::Scene::SceneObject* object, BGE::Scene::SceneObject::children()) {
    BGE::Scene::Light* light = static_cast<BGE::Scene::Light*> (object);

    light->setSpot(!light->isSpot());
  }
}
