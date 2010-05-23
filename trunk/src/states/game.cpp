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
#include "game.h"

#include "objects/fighter.h"

#include "assets/controller.h"
#include "assets/hud.h"

using namespace States;

Game::Game()
{
  m_controller = new Assets::Controller;
  setController(m_controller);
  m_hud = new Assets::HUD;
  setOverlay(m_hud);
}

void Game::setFighter(Objects::Fighter *fighter)
{
  m_controller->setFighter(fighter);
  m_hud->setFighter(fighter);
}
