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
#include "scene/light.h"

void Overlay::paint(QPainter *painter, qint32 elapsed)
{
  Q_UNUSED(elapsed);
  painter->save();

  painter->setPen(Qt::white);
  painter->drawText(5, 15, "Active camera: " + BGE::Canvas::canvas()->activeCamera()->name());

  QString lightsType = static_cast<BGE::Scene::Light*> (BGE::Canvas::canvas()->lights().first())->isSpot() ? "spotlight" : "omni light";
  painter->drawText(5, 30, "Active lights type: " + lightsType);

  painter->restore();
}
