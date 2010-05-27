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

#include "canvas.h"

#include "menuoverlay.h"
#include "statehandler.h"

using namespace States;
using namespace States::Assets;

MenuController::MenuController()
{}

void MenuController::keyPressed(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_R : {
      if (event->modifiers() & Qt::CTRL)
        BGE::Canvas::canvas()->setFPSShown(!BGE::Canvas::canvas()->isFPSShown());
      break;
    }

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
      StateHandler::self()->play();
      break;
    }

    case MenuOverlay::Continue: {
      StateHandler::self()->resume();
      break;
    }

    case MenuOverlay::Quit: {
      BGE::Canvas::canvas()->close();
      break;
    }
  }
}
