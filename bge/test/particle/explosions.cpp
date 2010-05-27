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
#include "explosions.h"

#include <ctime>

#include <QtCore/QTimer>

#include "canvas.h"

#include "explosion.h"

Explosions::Explosions(QObject *parent)
  : QObject(parent)
{
  qsrand(time(0l));
  m_timer = new QTimer(this);

  connect(m_timer, SIGNAL(timeout()), SLOT(ignite()));

  ignite();
}

void Explosions::ignite()
{
  Explosion *explosion = new Explosion(Vector3f(qrand() % 100 - 50, qrand() % 100 - 50, qrand() % 100 - 50));
  BGE::Canvas::canvas()->addSceneObject(explosion);

  m_timer->start(qrand() % 1000);
}
