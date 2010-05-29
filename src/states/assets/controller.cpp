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

#include <QtCore/QTimer>

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "canvas.h"

#include "objects/fighter.h"

#include "statehandler.h"

using namespace States;
using namespace States::Assets;

Controller::Controller()
  : QObject(),
    m_fighter(0l)
{
  m_timer = new QTimer(this);
  m_timer->setInterval(500);
  connect(m_timer, SIGNAL(timeout()), SLOT(fire()));
}

void Controller::keyPressed(QKeyEvent *event)
{
  if (m_fighter && !m_fighter->hullIntegrity())
    m_fighter = 0l;

  switch (event->key()) {
    case Qt::Key_R : {
      if (event->modifiers() & Qt::CTRL)
        BGE::Canvas::canvas()->setFPSShown(!BGE::Canvas::canvas()->isFPSShown());
      break;
    }

    case Qt::Key_Plus: {
      if (!m_fighter)
        return;
      m_fighter->setEnginePower(m_fighter->enginePower() + 50000);
      break;
    }

    case Qt::Key_Minus: {
      if (!m_fighter)
        return;
      m_fighter->setEnginePower(m_fighter->enginePower() - 50000);
      break;
    }

    case Qt::Key_Escape: {
      StateHandler::self()->unload();
      break;
    }

    // Camera controls
    case Qt::Key_1: {
      if (!m_fighter)
        return;
      BGE::Canvas::canvas()->activateCamera("First person camera");
      break;
    }
    case Qt::Key_2: {
      if (!m_fighter)
        return;
      BGE::Canvas::canvas()->activateCamera("Third person camera");
      break;
    }
    case Qt::Key_3: {
      if (!m_fighter)
        return;
      BGE::Canvas::canvas()->activateCamera("Front camera");
      break;
    }
    case Qt::Key_4: {
      if (!m_fighter)
        return;
      BGE::Canvas::canvas()->activateCamera("Side camera");
      break;
    }
  }
}

void Controller::mouseMoved(QMouseEvent *event)
{
  if (m_fighter && !m_fighter->hullIntegrity())
    m_fighter = 0l;

  if (!m_fighter)
    return;

  QSizeF size = BGE::Canvas::canvas()->size();
  QPointF normalized((-(float) event->x() / size.width()) + 0.5, (-(float) event->y() / size.height()) + 0.5);
  m_fighter->setAngularVelocity(Vector3f(normalized.y() * 2, 0, normalized.x() * 2));
  m_fighter->body()->activate();
}

void Controller::mouseButtonPressed(QMouseEvent *event)
{
  if (m_fighter && !m_fighter->hullIntegrity())
    m_fighter = 0l;

  if (!m_fighter)
    return;

  switch (event->button()) {
    case Qt::LeftButton:
      fire();
      m_timer->start();
      break;
  }
}

void Controller::mouseButtonReleased(QMouseEvent *event)
{
  if (m_fighter && !m_fighter->hullIntegrity())
    m_fighter = 0l;

  if (!m_fighter)
    return;

  switch (event->button()) {
    case Qt::LeftButton:
      m_timer->stop();
      break;
  }
}

void Controller::fire()
{
  if (!m_fighter)
    return;

  m_fighter->fire();
}
