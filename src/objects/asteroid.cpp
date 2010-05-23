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
#include "asteroid.h"

#include "canvas.h"

#include "scene/camera.h"

#include "asteroidlist.h"

#include "storage/storagemanager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

using namespace Objects;

Asteroid::Asteroid()
{
  QList<BGE::Storage::Item*> meshes = BGE::Storage::StorageManager::self()->get("/asteroids/models")->items();
  setMesh(static_cast<BGE::Storage::Mesh*> (meshes.at(qrand() % meshes.size())));
  scale(20);

  setTexture(BGE::Storage::StorageManager::self()->get<BGE::Storage::Texture*>("/asteroids/textures/" + mesh()->name()));

  AsteroidList::self()->addAsteroid(this);
}

void Asteroid::postTransformCalculations(qint32 timeDiff)
{
  AsteroidList::self()->setPosition(this, BGE::Scene::Camera::projection() * BGE::Canvas::canvas()->activeCamera()->cameraTransform() * globalPosition());
}
