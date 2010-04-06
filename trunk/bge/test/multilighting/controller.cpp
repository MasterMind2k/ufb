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
#include "controller.h"

#include "camera.h"

Controller::Controller(Camera *camera)
  : BGE::AbstractController()
{
  m_camera = camera;
}

void Controller::keyPressed(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Up:
      // Move forward
      m_camera->setState(Camera::Forward);
      break;

    case Qt::Key_Down:
      // Move backward
      m_camera->setState(Camera::Backward);
      break;

    case Qt::Key_Right:
      // Rotate right
      m_camera->setState(Camera::TurnRight);
      break;

    case Qt::Key_Left:
      // Rotate left
      m_camera->setState(Camera::TurnLeft);
      break;

    case Qt::Key_Plus:
      // Move up
      m_camera->setState(Camera::Upward);
      break;

    case Qt::Key_Minus:
      // Move down
      m_camera->setState(Camera::Downward);
      break;

    case Qt::Key_PageUp:
      // Rotate up
      m_camera->setState(Camera::TurnUp);
      break;

    case Qt::Key_PageDown:
      // Rotate down
      m_camera->setState(Camera::TurnDown);
      break;

    case Qt::Key_Space:
      // Stay! :)
      m_camera->setState(Camera::Stay);
      break;
  }
}
