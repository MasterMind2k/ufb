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
#include <QtGui/QApplication>

#include "canvas.h"

#include "storage/storagemanager.h"

#include "scene/camera.h"
#include "scene/light.h"

#include "explosions.h"

int main(int argc, char **argv)
{
  Q_INIT_RESOURCE(bge_resources);

  QApplication app(argc, argv);

  BGE::Canvas::canvas()->loadResource();

  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createCamera("Global Camera"));
  BGE::Canvas::canvas()->camera("Global Camera")->move(0, 0, 100);
  BGE::Canvas::canvas()->activateCamera("Global Camera");

  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createLight("Global light"));
  BGE::Canvas::canvas()->light("Global light")->setPositional(false);
  BGE::Canvas::canvas()->light("Global light")->move(0, 0, 1);

  // Let's ignite some explosions ;)
  Explosions *explosions = new Explosions(BGE::Canvas::canvas());

  BGE::Canvas::canvas()->show();
  BGE::Canvas::canvas()->setFPSShown(true);

  return app.exec();
}
