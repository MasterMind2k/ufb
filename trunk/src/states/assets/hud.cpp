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

#include <QtCore/QSettings>

#include <QtGui/QPainter>

#include "canvas.h"

#include "scene/camera.h"
#include "scene/boundingvolume.h"

#include "util/objectlist.h"

#include "objects/fighter.h"
#include "objects/bullet.h"
#include "objects/asteroid.h"

#include "statehandler.h"

#include "states/game.h"

using namespace States;
using namespace States::Assets;

HUD::HUD()
  : BGE::AbstractOverlay(),
    m_fighter(0l)
{
}

void HUD::paint(QPainter *painter, qint32 elapsed)
{
  if (m_fighter && !m_fighter->hullIntegrity()) {
    BGE::Canvas::canvas()->camera("Side camera")->deattach();
    BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Side camera"));
    BGE::Canvas::canvas()->activateCamera("Side camera");
    BGE::Canvas::canvas()->activeCamera()->observe(m_fighter);
    m_fighter = 0l;

    // Update time
    QSettings settings;
    quint32 time = settings.value("Hours", 0).toInt() * 3600 + settings.value("Minutes", 0).toInt() * 60 + settings.value("Seconds", 0).toInt();
    qint32 current = floor(StateHandler::self()->game()->time().elapsed() / 1000.0);

    if (current > time) {
      qint32 hours = floor(current / 3600.0);
      qint32 minutes = floor(current / 60.0) - hours * 60;
      qint32 seconds = current - hours * 3600 - minutes * 60;
      settings.setValue("Hours", hours);
      settings.setValue("Minutes", minutes);
      settings.setValue("Seconds", seconds);
    }
  }

  if (!m_fighter) {
    // Draw Game Over sign :)

    painter->setPen(Qt::red);
    QFont font;
    font.setPointSize(80);
    painter->setFont(font);

    painter->drawText(QRectF(QPointF(0, 0), BGE::Canvas::canvas()->size()), Qt::AlignCenter, "Game over");

    return;
  }

  QSizeF size = BGE::Canvas::canvas()->size();
  QColor greenPen(0, 255, 0, 150);
  QColor greenBar(0, 255, 0, 100);
  QColor green(0, 255, 0, 40);
  QColor redPen(255, 0, 0, 150);
  QColor red(255, 0, 0, 40);
  painter->setPen(greenPen);
  painter->setBrush(Qt::NoBrush);
  qreal lockRadius = 20;
  bool isLocked = false;
  QRectF panel(size.width() - 180, size.height() - 95, 180, 95);

  // Center cross
  if (BGE::Canvas::canvas()->activeCamera()->name() == "First person camera") {
    // Weapon's lock area
    painter->drawEllipse(size.width() / 2.0 - lockRadius, size.height() / 2.0 - lockRadius, 2.0 * lockRadius, 2.0 * lockRadius);
  }

  // Engine power
  QRectF rect(0, 0, 50, 150);
  rect.translate(BGE::Canvas::canvas()->width() - 60, 10);
  painter->drawRect(rect);

  rect.setHeight(m_fighter->velocity().norm() / Objects::Fighter::MaxVelocity * 150);
  rect.moveTo(rect.left(), 160 - rect.height());
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(greenBar);
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

    pos.x() = size.width() * ((pos.x() + 1.0) / 2.0);
    pos.y() = size.height() - size.height() * ((pos.y() + 1.0) / 2.0);

    // Don't draw under hud
    if (pos.x() + 100.0 >= panel.left() && pos.y() >= panel.top())
      continue;

    // Set color (Red - Fighter, green - other)
    if (object->name() == "Fighter") {
      painter->setPen(redPen);

      // Let's paint shields and hull integrity
      Objects::Fighter *fighter = static_cast<Objects::Fighter*> (object);
      painter->drawText(pos.x() + 5, pos.y() + 15, QString::number(fighter->hullIntegrity(), 'f', 2) + "%/" + QString::number(fighter->shields(), 'f', 2) + "%");
    } else {
      painter->setPen(greenPen);

      // Let's paint structural integrity
      Objects::Asteroid *asteroid = static_cast<Objects::Asteroid*> (object);
      painter->drawText(pos.x() + 5, pos.y() + 15, QString::number(asteroid->structuralIntegrity() / asteroid->maxStructuralIntegrity() * 100.0, 'f', 2) + "%");
    }
    painter->drawPoint(pos.x(), pos.y());

    float distance = object->distance(m_fighter);
    painter->drawText(pos.x() + 5, pos.y() + 5, QString("%0: %1").arg(object->name()).arg(distance));

    // Is it locked?
    if (!isLocked && distance < (m_fighter->velocity() + m_fighter->globalOrientation() * Vector3f(0, 0, -Objects::Bullet::Velocity)).norm() * Objects::Bullet::MaxLifetime / 1000.0) {
      // It's in range, is it in circle?
      Vector2f position(pos.x() - size.width() / 2.0, pos.y() - size.height() / 2.0);
      if (position.norm() < lockRadius) {
        // We've got a lock!
        m_fighter->setWeaponsLock(object);
        isLocked = true;

        if (m_fighter->hasWeaponsLock()) {
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
    }
  }
  painter->restore();

  // If we don't have any lock, remove the locked target
  if (!isLocked || BGE::Canvas::canvas()->activeCamera()->name() != "First person camera")
    m_fighter->setWeaponsLock(0l);

  // Are we getting a lock?

  // Direction to nearest asteroid
  painter->save();
  Objects::Object *nearest = Util::ObjectList::self()->nearest("Asteroid");
  painter->setBrush(green);
  if (nearest)
    paintNearestArrow(painter, nearest, size);

  // Direction to nearest fighter
  nearest = Util::ObjectList::self()->nearest("Fighter");
  painter->setPen(redPen);
  painter->setBrush(red);
  if (nearest)
    paintNearestArrow(painter, nearest, size);
  painter->restore();

  // Shields power, hull integrity and weapon's lock
  // Draw background
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(green);
  painter->drawRect(panel);
  painter->restore();

  painter->save();
  paintStatus(painter, panel, "Shields: " + QString::number(m_fighter->shields()) + "%");
  panel.setTop(panel.top() + 15);
  paintStatus(painter, panel, "Hull: " + QString::number(m_fighter->hullIntegrity()) + "%");
  panel.setTop(panel.top() + 15);
  if (isLocked && !m_fighter->hasWeaponsLock()) {
    // Make a more visual change
    QPointF center(size.width() / 2.0, size.height() / 2.0);
    painter->drawLine(center + QPointF(-lockRadius, 0), center + QPointF(lockRadius, 0));
    painter->drawLine(center + QPointF(0, -lockRadius), center + QPointF(0, lockRadius));

    // Write status
    painter->setPen(redPen);
    paintStatus(painter, panel, "Locking target...");
  } if (m_fighter->hasWeaponsLock()) {
    QPointF center(size.width() / 2.0, size.height() / 2.0);
    painter->drawLine(center + QPointF(-lockRadius, 0), center + QPointF(lockRadius, 0));
    painter->drawLine(center + QPointF(0, -lockRadius), center + QPointF(0, lockRadius));

    painter->setPen(greenPen);
    paintStatus(painter, panel, "Target locked!");
  }
  painter->restore();

  // Wave status
  panel.setTop(panel.top() + 15);
  paintStatus(painter, panel, QString("Wave: %0").arg(StateHandler::self()->wave()));
  if (StateHandler::self()->nextWaveIn() > 0) {
    // paint timer
    panel.setLeft(panel.left() + 50);
    paintStatus(painter, panel, QString(" / Next wave in: %0").arg(30 - qRound(StateHandler::self()->nextWaveIn() / 1000.0)));
  }

  // Draw timings
  panel.setTop(panel.top() + 15);
  QSettings settings;
  paintStatus(painter, panel, QString("Previous time: %0:%1:%2").arg(settings.value("Hours", 0).toInt()).arg(settings.value("Minutes", 0).toInt()).arg(settings.value("Seconds", 0).toInt()));
  panel.setTop(panel.top() + 15);
  qint32 current = floor(StateHandler::self()->game()->time().elapsed() / 1000.0);
  qint32 hours = floor(current / 3600.0);
  qint32 minutes = floor(current / 60.0) - hours * 60;
  qint32 seconds = current - hours *3600 - minutes * 60;
  paintStatus(painter, panel, QString("Current time: %0:%1:%2").arg(hours).arg(minutes).arg(seconds));
}

void HUD::paintNearestArrow(QPainter *painter, Objects::Object *nearest, const QSizeF &drawingSize)
{
  if (!nearest)
    return;

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

  if ((BGE::Canvas::canvas()->activeCamera()->cameraTransform() * nearest->globalPosition()).z() > 0)
    painter->setBrush(Qt::NoBrush);

  // Draw the arrow if we dont see the object
  if (x || y) {
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
    // Otherwise make a circle
    painter->drawEllipse(pos.x() - 15, pos.y() - 15, 30, 30);
  }
}

void HUD::paintStatus(QPainter *painter, const QRectF &geometry, const QString &text)
{
  // Draw text
  QRectF textRect = painter->boundingRect(geometry, Qt::AlignLeft, text);
  painter->drawText(textRect, text);
}
