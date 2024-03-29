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
#include "abstractcontroller.h"

#include "scene/light.h"
#include "scene/camera.h"

#include "storage/manager.h"
#include "storage/mesh.h"
#include "storage/texture.h"

#include "movinglight.h"
#include "overlay.h"

BGE::Storage::Mesh* createCube()
{
  QString name = "cube";
  BGE::Storage::Mesh* mesh = new BGE::Storage::Mesh("Cube");
  float half = 5;
  // Prepare our vertices
  Vector3f bottomFrontLeft(-half, -half, half);
  Vector3f bottomFrontRight(half, -half, half);
  Vector3f topFrontLeft(-half, half, half);
  Vector3f topFrontRight(half, half, half);

  Vector3f bottomBehindLeft(-half, -half, -half);
  Vector3f bottomBehindRight(half, -half, -half);
  Vector3f topBehindLeft(-half, half, -half);
  Vector3f topBehindRight(half, half, -half);
  // And draw the cube
  // Front side
  mesh->addRectangle(name, bottomFrontLeft, bottomFrontRight, topFrontLeft, topFrontRight);
  // Right side
  mesh->addRectangle(name, bottomFrontRight, bottomBehindRight, topFrontRight, topBehindRight);
  // Left side
  mesh->addRectangle(name, bottomBehindLeft, bottomFrontLeft, topBehindLeft, topFrontLeft);
  // Bottom side
  mesh->addRectangle(name, bottomBehindLeft, bottomBehindRight, bottomFrontLeft, bottomFrontRight);
  // Behind side
  mesh->addRectangle(name, topBehindLeft, topBehindRight, bottomBehindLeft, bottomBehindRight);
  // Top side
  mesh->addRectangle(name, topFrontLeft, topFrontRight, topBehindLeft, topBehindRight);

  mesh->calculateNormals("cube");
  mesh->addFaceMaterial(name, 0, "side");
  mesh->addFaceMaterial(name, 1, "side");
  mesh->addFaceMaterial(name, 2, "side");
  mesh->addFaceMaterial(name, 3, "side");
  mesh->addFaceMaterial(name, 4, "side");
  mesh->addFaceMaterial(name, 5, "side");
  return mesh;
}

class Controller : public BGE::AbstractController
{
  public:
  Controller(const QList<BGE::Scene::Camera*> &cameras)
    {
      m_cameras = cameras;
      m_current = 0;
    }

  private:
    quint8 m_current;
    QList<BGE::Scene::Camera*> m_cameras;
    void keyPressed(QKeyEvent *event)
    {
      if (event->key() == Qt::Key_Space) {
        m_current++;
        m_current %= m_cameras.size();
        BGE::Canvas::canvas()->activateCamera(m_cameras.at(m_current)->name());
      }
    }
};

int main(int argc, char** argv)
{
  Q_INIT_RESOURCE(bge_resources);
  BGE::Scene::Light::setGlobalAmbient(Qt::black);

  QApplication app(argc, argv);

  // Load models and textures
  BGE::Canvas::canvas()->loadResource();

  // Setup overlay
  BGE::Canvas::canvas()->setOverlay(new Overlay);

  // Make a small cube
  BGE::Storage::Manager::self()->set(createCube(), "/models");
  // Make materials for cubes
  {
    BGE::Storage::Material* material = new BGE::Storage::Material("Red light");
    material->setEmission(QColor(0.5 * 255, 0, 0, 255));
    material->setFaceName("side");
    BGE::Storage::Manager::self()->set(material, "/materials/");

    material = new BGE::Storage::Material("Blue light");
    material->setEmission(QColor(0, 0, 0.5 * 255, 255));
    material->setFaceName("side");
    BGE::Storage::Manager::self()->set(material, "/materials/");

    material = new BGE::Storage::Material("Green light");
    material->setEmission(QColor(0, 0.5 * 255, 0, 255));
    material->setFaceName("side");
    BGE::Storage::Manager::self()->set(material, "/materials/");

    material = new BGE::Storage::Material("Yellow light");
    material->setEmission(QColor(0.5 * 255, 0.5 * 255, 0, 255));
    material->setFaceName("side");
    BGE::Storage::Manager::self()->set(material, "/materials/");
  }

  QList<BGE::Scene::Camera*> cameras;
  // Create cameras
  BGE::Canvas::canvas()->createCamera("First camera")->move(0, -30, 400);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("First camera"));
  cameras << BGE::Canvas::canvas()->camera("First camera");
  BGE::Canvas::canvas()->activateCamera("First camera");

  BGE::Canvas::canvas()->createCamera("Second camera")->move(0, 300, 50);
  BGE::Canvas::canvas()->camera("Second camera")->rotateX(-60);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Second camera"));
  cameras << BGE::Canvas::canvas()->camera("Second camera");

  BGE::Canvas::canvas()->createCamera("Third camera")->move(350, 0, -300);
  BGE::Canvas::canvas()->camera("Third camera")->rotateY(130);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Third camera"));
  cameras << BGE::Canvas::canvas()->camera("Third camera");

  BGE::Canvas::canvas()->createCamera("Fourth camera")->move(-150, -300, 50);
  BGE::Canvas::canvas()->camera("Fourth camera")->rotateX(80);
  BGE::Canvas::canvas()->camera("Fourth camera")->rotateY(-20);
  BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->camera("Fourth camera"));
  cameras << BGE::Canvas::canvas()->camera("Fourth camera");

  // Add our model
  BGE::Scene::Object* fighter = new BGE::Scene::Object;
  fighter->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Fighter"));
  fighter->setTexture(BGE::Storage::Manager::self()->get<BGE::Storage::Texture*>("/textures/Fighter"));
  // Add materials
  fighter->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/models/Fighter/sf-01"));
  fighter->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/models/Fighter/sf-01-g"));
  BGE::Canvas::canvas()->addSceneObject(fighter);

  // Create lights
  MovingLight* movingLight = new MovingLight;

  BGE::Scene::Light* light = BGE::Canvas::canvas()->createLight("Red light");
  light->move(-150, -180, -200);
  light->setDiffuseColor(QColor(0.5 * 255, 0, 0));
  light->setSpecularColor(QColor(0.8 * 255, 0, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/materials/Red light"));
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Blue light");
  light->move(150, 80, -250);
  light->setDiffuseColor(QColor(0, 0, 0.5 * 255));
  light->setSpecularColor(QColor(0, 0, 0.8 * 255));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/materials/Blue light"));
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Green light");
  light->move(-150, 80, 250);
  light->setDiffuseColor(QColor(0, 0.5 * 255, 0));
  light->setSpecularColor(QColor(0, 0.8 * 255, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/materials/Green light"));
  light->observe(fighter);
  movingLight->addChild(light);

  light = BGE::Canvas::canvas()->createLight("Yellow light");
  light->move(150, -180, 200);
  light->setDiffuseColor(QColor(0.5 * 255, 0.5 * 255, 0));
  light->setSpecularColor(QColor(0.8 * 255, 0.8 * 255, 0));
  light->setSpotExponent(25);
  light->setMesh(BGE::Storage::Manager::self()->get<BGE::Storage::Mesh*>("/models/Cube"));
  light->addMaterial(BGE::Storage::Manager::self()->get<BGE::Storage::Material*>("/materials/Yellow light"));
  light->observe(fighter);
  movingLight->addChild(light);
  BGE::Canvas::canvas()->addSceneObject(movingLight);

  BGE::Canvas::canvas()->setController(new Controller(cameras));

  // Make FPS shown
  BGE::Canvas::canvas()->setFPSShown(true);

  BGE::Canvas::canvas()->show();

  return app.exec();
}
