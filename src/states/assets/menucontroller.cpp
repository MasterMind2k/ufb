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
#include "menucontroller.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "objects/fighter.h"
#include "objects/asteroid.h"

#include "menuoverlay.h"

#include "states/game.h"

#include "canvas.h"

#include "scene/camera.h"
#include "scene/light.h"

using namespace States;
using namespace States::Assets;

MenuController::MenuController()
{}

void MenuController::keyPressed(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Up:
      m_overlay->moveSelection(MenuOverlay::Up);
      break;

    case Qt::Key_Down:
      m_overlay->moveSelection(MenuOverlay::Down);
      break;

    case Qt::Key_Return:
      execute();
      break;

    default:
      break;
  }
}

void MenuController::execute()
{
  switch (m_overlay->selectedButton()) {
    case MenuOverlay::Play: {
      // Spawn the game
      Objects::Asteroid *asteroid = new Objects::Asteroid;
      asteroid->move(0, 0, -700);

      Game *game = new Game();
      game->fighter()->move(0, 0, 800);
      game->fighter()->initBody();
      asteroid->initBody();
      BGE::Canvas::canvas()->pushGameState(game);
      game->fighter()->addChild(BGE::Canvas::canvas()->createCamera("Global camera"));
      //BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createCamera("Global camera"));
      BGE::Canvas::canvas()->addSceneObject(BGE::Canvas::canvas()->createLight("Global light"));
      BGE::Canvas::canvas()->light("Global light")->setPositional(false);
      BGE::Canvas::canvas()->light("Global light")->setPosition(0, 1, 1);
      BGE::Canvas::canvas()->camera("Global camera")->move(0, 100, 200);
      BGE::Canvas::canvas()->addSceneObject(game->fighter());
      BGE::Canvas::canvas()->addSceneObject(asteroid);
      BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(game->fighter()->body());
      BGE::Canvas::canvas()->dynamicsWorld()->addRigidBody(asteroid->body());
      BGE::Canvas::canvas()->activateCamera("Global camera");

      asteroid->body()->setGravity(btVector3(0, 0, 0));
      game->fighter()->body()->setGravity(btVector3(0, 0, 0));
      break;
    }

    case MenuOverlay::Quit: {
      BGE::Canvas::canvas()->close();
      break;
    }
  }
}
