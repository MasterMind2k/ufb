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
#include "movingcube.h"

#include <QtCore/QTimer>

MovingCube::MovingCube(int seed)
  : BGE::Scene::SceneObject(), QObject()
{
  m_direction = Vector3f::Zero();

  QTimer* timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(5000);
  timer->start();

  connect(timer, SIGNAL(timeout()), SLOT(change()));

  qsrand(seed);
  change();
}

void MovingCube::calculateTransforms()
{
  move(m_direction);
  rotate(m_rotation);
}

void MovingCube::change()
{
  m_direction = Vector3f((float) (qrand() % 200 - 100)  / (float) 100000, (float) (qrand() % 200 - 100)  / (float) 100000, (float) (qrand() % 200 - 100)  / (float) 100000);

  // Flip if we are escaping :D
  if (globalPosition().x() > 10)
    m_direction(0) *= -1;
  else if (globalPosition().x() < -10)
    m_direction(0) *= -1;
  if (globalPosition().y() > 10)
    m_direction(1) *= -1;
  else if (globalPosition().y() < -10)
    m_direction(1) *= -1;
  if (globalPosition().z() > 10)
    m_direction(2) *= -1;
  else if (globalPosition().z() < -10)
    m_direction(2) *= -1;

  m_rotation = AngleAxisf(M_PI / (float) (qrand() % 1000 + 1000), Vector3f::UnitX());
  m_rotation = m_rotation * AngleAxisf(M_PI / (float) (qrand() % 1000 + 1000), Vector3f::UnitY());
  m_rotation = m_rotation * AngleAxisf(M_PI / (float) (qrand() % 1000 + 1000), Vector3f::UnitZ());
}
