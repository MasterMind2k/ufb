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
#include "Eigen/QtAlignedMalloc"
#include <QtGui/QApplication>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "canvas.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"

#include "scene/light.h"

#include "states/menu.h"

int main(int argc, char **argv)
{
  Q_INIT_RESOURCE(bge_resources);

  QApplication app(argc, argv);

  // Set the scene size
  BGE::Canvas::SceneSize = Vector3f(100000, 100000, 100000);

  // Load data
  BGE::Canvas::canvas()->loadResource("./resources.rcc");

  // Itensify ambient light
  BGE::Scene::Light::setGlobalAmbient(QColor(120, 120, 120));

  // Fix fighter model :)
  BGE::Storage::StorageManager::self()->get<BGE::Storage::Mesh*>("/fighters/models/fighter")->rotate(AngleAxisf(-M_PI / 2, Vector3f::UnitX()));

  BGE::Canvas::canvas()->pushGameState(new States::Menu);

  BGE::Canvas::canvas()->createCamera("Global camera");
  BGE::Canvas::canvas()->activateCamera("Global camera");

  BGE::Canvas::canvas()->show();

  return app.exec();
}
