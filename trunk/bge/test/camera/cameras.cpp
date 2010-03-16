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
#include "cameras.h"

#include <QtCore/QTimer>

#include "canvas.h"

#include "scene/camera.h"

Cameras::Cameras()
 : QObject()
{
  m_currentCamera = 1;

  m_timer = new QTimer(this);
  m_timer->setSingleShot(false);
  m_timer->setInterval(3000);
  m_timer->start();

  connect(m_timer, SIGNAL(timeout()), SLOT(changeCamera()));
}

void Cameras::changeCamera()
{
  BGE::Canvas::canvas()->activateCamera(m_cameras.at(m_currentCamera++)->name());

  if (m_currentCamera >= m_cameras.size())
    m_currentCamera = 0;
}
