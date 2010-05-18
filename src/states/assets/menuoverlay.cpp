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
#include "menuoverlay.h"

#include <QtCore/QRectF>

#include <QtGui/QPainter>
#include <QtGui/QRadialGradient>

#include "canvas.h"

using namespace States;
using namespace States::Assets;

MenuOverlay::MenuOverlay()
{
  m_selectedButton = Play;

  m_buttonGradient = QRadialGradient(0, 0, 200);
  m_buttonGradient.setColorAt(0, QColor(0, 0, 255));
  m_buttonGradient.setColorAt(1, QColor(0, 0, 100));

  m_selectedButtonGradient = QRadialGradient(0, 0, 200);
  m_selectedButtonGradient.setColorAt(0, QColor(255, 0, 0));
  m_selectedButtonGradient.setColorAt(1, QColor(100, 0, 0));

  m_buttonSize = QSizeF(200, 50);

  m_buttonTextFont = QFont("Serif");
  m_buttonTextFont.setBold(true);
  m_buttonTextFont.setPointSize(20);
}

void MenuOverlay::moveSelection(Directions direction)
{
  switch (m_selectedButton) {
    case Play:
      m_selectedButton = direction == Up ? m_selectedButton : Quit;
      break;

    case Quit:
      m_selectedButton = direction == Up ? Play : m_selectedButton;
      break;
  }
}

void MenuOverlay::paint(QPainter *painter, qint32 elapsed)
{
  Q_UNUSED(elapsed);

  // Geometry
  QSize size = BGE::Canvas::canvas()->size();
  QRectF geometry;
  geometry.moveTo(size.width() / 2 - m_buttonSize.width() / 2, 60);
  geometry.setSize(QSizeF(200, 50));
  painter->setFont(m_buttonTextFont);

  // Play
  painter->save();
  drawButton(Play, geometry, painter);
  painter->restore();

  // Quit
  geometry.translate(0, m_buttonSize.height() + 30);
  painter->save();
  drawButton(Quit, geometry, painter);
  painter->restore();
}

void MenuOverlay::drawButton(Buttons button, const QRectF &geometry, QPainter *painter)
{
  QString text = buttonText(button);
  QPointF sizePoint(m_buttonSize.width(), m_buttonSize.height());
  QRadialGradient gradient = m_selectedButton == button ? m_selectedButtonGradient : m_buttonGradient;

  gradient.setCenter(geometry.topLeft() + sizePoint / 2.0);
  gradient.setFocalPoint(geometry.topLeft() + sizePoint);

  painter->setBrush(gradient);
  painter->setPen(Qt::NoPen);
  painter->drawRoundRect(geometry, 10);
  painter->setPen(Qt::white);
  QRectF textRect = painter->boundingRect(geometry, Qt::AlignCenter, text);
  painter->drawText(textRect, text);
}

QString MenuOverlay::buttonText(Buttons button)
{
  switch (button) {
    case Play:
      return "Play";

    case Quit:
      return "Quit";
  }
}
