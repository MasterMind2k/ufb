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

#include "scene/camera.h"
#include "scene/boundingvolume.h"

#include "util/objectlist.h"

#include "objects/fighter.h"
#include "objects/bullet.h"
#include "objects/asteroid.h"

using namespace States;
using namespace Assets;

HUD::HUD()
  : BGE::AbstractOverlay(),
    m_fighter(0l)
{
}

void HUD::paint(QPainter *painter, qint32 elapsed)
{
  if (!m_fighter)
    return;

  QSizeF size = BGE::Canvas::canvas()->size();
  QColor green(0, 255, 0, 150);
  QColor red(255, 0, 0, 150);
  painter->setPen(green);
  painter->setBrush(Qt::NoBrush);

  // Center cross
  if (BGE::Canvas::canvas()->activeCamera()->name() == "First person camera") {
    QPointF center(size.width() / 2.0, size.height() / 2.0);
    painter->drawLine(center + QPointF(-15, 0), center + QPointF(15, 0));
    painter->drawLine(center + QPointF(0, -15), center + QPointF(0, 15));
  }

  // Engine power
  QRectF rect(0, 0, 50, 150);
  rect.translate(BGE::Canvas::canvas()->width() - 60, 10);
  painter->drawRect(rect);

  rect.setHeight(m_fighter->velocity().norm() / Objects::Fighter::MaxVelocity * 150);
  rect.moveTo(rect.left(), 160 - rect.height());
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(0, 255, 0, 150));
  painter->drawRect(rect);
  painter->restore();

  rect.setHeight(m_fighter->enginePower() / Objects::Fighter::MaxPower * 150);
  rect.moveTo(rect.left(), 160 - rect.height());
  painter->drawRect(rect);

  // Paint distances
  painter->save();
  Util::ObjectList::self()->sort();
  foreach (Objects::Object *object, Util::ObjectList::self()->objects()) {
    Vector3f pos = Util::ObjectList::self()->transformedPositions().value(object);
    if (pos.z() < -1 || pos.z() > 1)
      break;

    if (pos.x() < -1 || pos.x() > 1)
      break;
    if (pos.y() < -1 || pos.y() > 1)
      break;

    // Set color (Red - Fighter, green - other)
    if (object->name() == "Fighter")
      painter->setPen(red);
    else
      painter->setPen(green);

    pos.x() = size.width() * ((pos.x() + 1.0) / 2.0);
    pos.y() = size.height() - size.height() * ((pos.y() + 1.0) / 2.0);
    painter->drawPoint(pos.x(), pos.y());

    float distance = object->distance(m_fighter);
    painter->drawText(pos.x() + 5, pos.y() + 5, QString("%0: %1").arg(object->name()).arg(distance));

    // Is it in weapon's range?
    if (distance < (m_fighter->velocity() + m_fighter->globalOrientation() * Vector3f(0, 0, -Objects::Bullet::Velocity)).norm() * Objects::Bullet::MaxLifetime / 1000.0) {
      // Left top corner
      painter->drawLine(pos.x() - 15, pos.y() - 15, pos.x() - 10, pos.y() - 15);
      painter->drawLine(pos.x() - 15, pos.y() - 15, pos.x() - 15, pos.y() - 10);
      // Left bottom corner
      painter->drawLine(pos.x() - 15, pos.y() + 15, pos.x() - 10, pos.y() + 15);
      painter->drawLine(pos.x() - 15, pos.y() + 15, pos.x() - 15, pos.y() + 10);
      // Right top corner
      painter->drawLine(pos.x() + 15, pos.y() - 15, pos.x() + 10, pos.y() - 15);
      painter->drawLine(pos.x() + 15, pos.y() - 15, pos.x() + 15, pos.y() - 10);
      // Right bottom corner
      painter->drawLine(pos.x() + 15, pos.y() + 15, pos.x() + 10, pos.y() + 15);
      painter->drawLine(pos.x() + 15, pos.y() + 15, pos.x() + 15, pos.y() + 10);
    }
  }
  painter->restore();

  // Direction to nearest asteroid
  Objects::Object *nearest = Util::ObjectList::self()->nearest("Asteroid");
  painter->setBrush(green);
  if (nearest)
    paintNearestArrow(painter, nearest, size);

  // Direction to nearest fighter
  nearest = Util::ObjectList::self()->nearest("Fighter");
  painter->setPen(red);
  painter->setBrush(red);
  if (nearest)
    paintNearestArrow(painter, nearest, size);
}

void HUD::paintNearestArrow(QPainter *painter, Objects::Object *nearest, const QSizeF &drawingSize)
{
  Vector3f pos = Util::ObjectList::self()->transformedPositions().value(nearest);

  // Project to the screen
  pos.x() = drawingSize.width() * ((pos.x() + 1.0) / 2.0);
  pos.y() = drawingSize.height() - drawingSize.height() * ((pos.y() + 1.0) / 2.0);

  // Determining the corner
  bool x = true;
  bool y = true;
  if (pos.x() > drawingSize.width())
    pos.x() = drawingSize.width();
  else if (pos.x() < 0)
    pos.x() = 0;
  else
    x = false;

  if (pos.y() > drawingSize.height())
    pos.y() = drawingSize.height();
  else if (pos.y() < 0)
    pos.y() = 0;
  else
    y = false;

  // Draw the arrow if we dont see the object
  if (x || y) {
    painter->setBrush(Qt::NoBrush);
    qreal angle = 0;
    if (x && y) {
      if (pos.x() > 0 && pos.y() > 0)
        angle = 3.0 * M_PI / 4.0;
      else if (pos.x() > 0 && pos.y() == 0)
        angle = M_PI / 4.0;
      else if (pos.x() == 0 && pos.y() > 0)
        angle = -3.0 * M_PI / 4.0;
      else
        angle = -M_PI / 4.0;
    } else if (!x) {
      if (pos.y() > 0)
        angle = M_PI;
    } else if (!y) {
      if (pos.x() > 0)
        angle = M_PI / 2.0;
      else
        angle = -M_PI / 2.0;
    }
    // Prepare the arrow
    Vector2f tip(0, 0), left(-25, 25), right(25, 25);
    Rotation2Df rot(angle);
    tip = rot * tip;
    left = rot * left;
    right = rot * right;

    QPolygon arrow;
    arrow << QPoint(tip.x(), tip.y());
    arrow << QPoint(left.x(), left.y());
    arrow << QPoint(right.x(), right.y());
    arrow.translate(pos.x(), pos.y());

    painter->drawPolygon(arrow);
  } else {
    if ((BGE::Canvas::canvas()->activeCamera()->cameraTransform() * nearest->globalPosition()).z() > 0)
      painter->setBrush(Qt::NoBrush);
    // Otherwise make a circle
    painter->drawEllipse(pos.x() - 15, pos.y() - 15, 30, 30);
  }
}
