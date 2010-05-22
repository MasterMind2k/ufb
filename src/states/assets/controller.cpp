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

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "canvas.h"

#include "objects/fighter.h"

using namespace States;
using namespace States::Assets;

Controller::Controller(Objects::Fighter *fighter)
  : m_fighter(fighter)
{
}

void Controller::keyPressed(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_R : {
      if (event->modifiers() & Qt::CTRL)
        BGE::Canvas::canvas()->setFPSShown(!BGE::Canvas::canvas()->isFPSShown());
      break;
    }

    case Qt::Key_Plus: {
      m_fighter->setEnginePower(m_fighter->enginePower() + 50000);
      break;
    }

    case Qt::Key_Minus: {
      m_fighter->setEnginePower(m_fighter->enginePower() - 50000);
      break;
    }
  }
  m_fighter->body()->activate();
}

void Controller::mouseMoved(QMouseEvent *event)
{
  QSizeF size = BGE::Canvas::canvas()->size();
  QPointF normalized((-(float) event->x() / size.width()) + 0.5, (-(float) event->y() / size.height()) + 0.5);
  m_fighter->setAngularVelocity(Vector3f(normalized.y() * 2, 0, normalized.x() * 2));
  m_fighter->body()->activate();
}
