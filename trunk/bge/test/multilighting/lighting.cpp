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

#include "canvas.h"

#include "scene/light.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/material.h"
#include "storage/shaderprogram.h"

Lighting::Lighting()
  : QObject(), BGE::Scene::Object()
{
  BGE::Storage::Material *material = BGE::Storage::StorageManager::self()->get<BGE::Storage::Material*>("/materials/Light");
  // Setup lights
  for (quint16 i = 0; i < 15; i ++) {
    QString lightName = "Light_" + QString::number(i);
    addChild(BGE::Canvas::canvas()->createLight(lightName));
    BGE::Canvas::canvas()->light(lightName)->setQuadraticAttenuation(0.0005);
    BGE::Canvas::canvas()->light(lightName)->move(qrand() % 500 - 300, 50, qrand() % 500 - 300);
    BGE::Canvas::canvas()->light(lightName)->setMesh(BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
    BGE::Canvas::canvas()->light(lightName)->addMaterial(material);
  }

  qsrand(time(0l));

  // Init directions
  setDirections();
}

void Lighting::calculateTransforms(qint32 timeDiff)
{
  quint32 i = 0;
  foreach (BGE::Scene::Object *light, BGE::Scene::Object::children()) {
    Vector3f direction = m_directions.at(i);
    if (light->globalPosition().x() > 300 || light->globalPosition().x() < -300)
      direction = direction.cwise() * Vector3f(-1, 1, 1);

    if (light->globalPosition().z() > 300 || light->globalPosition().z() < -300)
      direction = direction.cwise() * Vector3f(1, 1, -1);

    m_directions[i++] = direction;
    light->move(direction * (qreal) timeDiff / 1000.0);
  }
}

void Lighting::setDirections()
{
  quint32 size = BGE::Canvas::canvas()->lights().size();
  m_directions.clear();
  for (quint32 i = 0; i < size; i++)
    m_directions << Vector3f(qrand() % 50, 0, qrand() % 50);
}
