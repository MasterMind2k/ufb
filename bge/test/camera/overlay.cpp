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
#include "overlay.h"

#include <QtGui/QPainter>

#include "canvas.h"

#include "scene/camera.h"

void Overlay::paint(QPainter *painter, qint32 elapsed)
{
  Q_UNUSED(elapsed);
  painter->save();

  painter->setPen(Qt::white);
  painter->drawText(5, 15, "Active camera: " + canvas()->activeCamera()->name());

  painter->restore();
}
