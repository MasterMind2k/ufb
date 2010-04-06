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
#include "camera.h"

void Camera::setState(States state)
{
  m_state = state;
  switch (m_state) {
    case Forward:
      m_stateString = "Forward";
      break;

    case Backward:
      m_stateString = "Backward";
      break;

    case Downward:
      m_stateString = "Downward";
      break;

    case Upward:
      m_stateString = "Upward";
      break;

    case TurnLeft:
      m_stateString = "TurnLeft";
      break;

    case TurnRight:
      m_stateString = "TurnRight";
      break;

    case TurnUp:
      m_stateString = "TurnUp";
      break;

    case TurnDown:
      m_stateString = "TurnDown";
      break;

    case Stay:
      m_stateString = "Stay";
      break;
  }
}

void Camera::calculateTransforms(qint32 timeDiff)
{
  qreal diffInSec = (qreal) timeDiff / 1000.0;
  // Used for forward and backward
  Vector3f forwardVector(0, 0, -50);
  // Used for upward and downward
  Vector3f upVector(0, 50, 0);
  // Used for rotations
  qreal unitAngle = 50.0;

  switch (m_state) {
    case Forward:
      move((orientation() * forwardVector) * diffInSec);
      break;

    case Backward:
      move((orientation() * -forwardVector) * diffInSec);
      break;

    case Downward:
      move(-upVector * diffInSec);
      break;

    case Upward:
      move(upVector * diffInSec);
      break;

    case TurnLeft:
      rotateY(unitAngle * diffInSec);
      break;

    case TurnRight:
      rotateY(-unitAngle * diffInSec);
      break;

    case TurnUp:
      rotateX(unitAngle * diffInSec);
      break;

    case TurnDown:
      rotateX(-unitAngle* diffInSec);
      break;
  }
}
