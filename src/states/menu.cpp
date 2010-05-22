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
#include "menu.h"

#include "assets/menuoverlay.h"
#include "assets/menucontroller.h"

using namespace States;

Menu::Menu()
{
  Assets::MenuOverlay *overlay = new Assets::MenuOverlay;
  setOverlay(overlay);
  Assets::MenuController *controller = new Assets::MenuController;
  controller->setOverlay(overlay);
  setController(controller);
}