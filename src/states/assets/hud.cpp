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
#include "hud.h"

#include <QtGui/QPainter>

#include "canvas.h"

#include "objects/fighter.h"

// Odstrani
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "scene/camera.h"

#include "objects/asteroidlist.h"
#include "objects/asteroid.h"

using namespace States;
using namespace Assets;

HUD::HUD(Objects::Fighter *fighter)
  : BGE::AbstractOverlay(),
    m_fighter(fighter)
{
}

void HUD::paint(QPainter *painter, qint32 elapsed)
{
  QSizeF size = BGE::Canvas::canvas()->size();
  painter->setPen(Qt::green);
  painter->setBrush(Qt::NoBrush);

  Vector2f center(BGE::Canvas::canvas()->width() / 2.0, BGE::Canvas::canvas()->height() / 2.0);
  Vector2f left(center.x() / (float) 2.0, center.y());
  float width = 2.0 * BGE::Canvas::canvas()->width() / 4.0;
  Vector2f right(left.x() + width, left.y());

  Vector2f tip(left.x(), center.y() / 2);
  Vector2f base(left.x(), left.y());
  Vector2f widthDirection(1, 0);

  // Z rotation
  float angle = -atan2(2 * (m_fighter->globalOrientation().x() * m_fighter->globalOrientation().y()
                            + m_fighter->globalOrientation().w() * m_fighter->globalOrientation().z()),
                            m_fighter->globalOrientation().w() * m_fighter->globalOrientation().w()
                            + m_fighter->globalOrientation().x() * m_fighter->globalOrientation().x()
                            - m_fighter->globalOrientation().y() * m_fighter->globalOrientation().y()
                            - m_fighter->globalOrientation().z() * m_fighter->globalOrientation().z());
  Rotation2Df rotate(angle);

  left = rotate * (left - center) + center;
  right = rotate * (right - center) + center;
  painter->drawLine(QPointF(left.x(), left.y()), QPointF(right.x(), right.y()));

  // X rotation
  angle = atan2(2 * (m_fighter->globalOrientation().y() * m_fighter->globalOrientation().z()
                     + m_fighter->globalOrientation().w() * m_fighter->globalOrientation().x()),
                     m_fighter->globalOrientation().w() * m_fighter->globalOrientation().w()
                     - m_fighter->globalOrientation().x() * m_fighter->globalOrientation().x()
                     - m_fighter->globalOrientation().y() * m_fighter->globalOrientation().y()
                     + m_fighter->globalOrientation().z() * m_fighter->globalOrientation().z());
  angle = qAbs(angle / M_PI);
  tip.y() = center.y() + angle * tip.y() - tip.y() / 2;

  tip = rotate * (tip - center) + center;
  base = rotate * (base - center) + center;
  widthDirection = rotate * widthDirection;

  painter->drawLine(QPointF(tip.x(), tip.y()), QPointF(base.x(), base.y()));
  tip += width * widthDirection;
  base += width * widthDirection;
  painter->drawLine(QPointF(tip.x(), tip.y()), QPointF(base.x(), base.y()));

  // Engine power
  QRectF rect(0, 0, 50, 150);
  rect.translate(BGE::Canvas::canvas()->width() - 60, 10);
  painter->drawRect(rect);

  // Tested, and gor max velocity 1435
  rect.setHeight(m_fighter->body()->getLinearVelocity().length() / 1435 * 150);
  rect.moveTo(rect.left(), 160 - rect.height());
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::blue);
  painter->drawRect(rect);
  painter->restore();

  rect.setHeight(m_fighter->enginePower() / Objects::Fighter::MaxPower * 150);
  rect.moveTo(rect.left(), 160 - rect.height());
  painter->drawRect(rect);

  // Paint distances
  Objects::AsteroidList::self()->sort();
  foreach (Objects::Asteroid *asteroid, Objects::AsteroidList::self()->asteroids()) {
    Vector3f pos = Objects::AsteroidList::self()->transformedPositions().value(asteroid);
    if (pos.z() < -1 || pos.z() > 1)
      continue;

    if (pos.x() < -1 || pos.x() > 1)
      continue;
    if (pos.y() < -1 || pos.y() > 1)
      continue;

    pos.x() = size.width() * ((pos.x() + 1.0) / 2.0);
    pos.y() = size.height() - size.height() * ((pos.y() + 1.0) / 2.0);
    painter->drawPoint(pos.x(), pos.y());

    float distance = (BGE::Canvas::canvas()->activeCamera()->globalPosition() - asteroid->globalPosition()).norm();
    painter->drawText(pos.x(), pos.y(), "Distance: " + QString::number(distance));
  }
}
