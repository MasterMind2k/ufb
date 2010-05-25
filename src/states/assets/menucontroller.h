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
#ifndef STATES_ASSETS_MENUCONTROLLER_H
#define STATES_ASSETS_MENUCONTROLLER_H

#include "abstractcontroller.h"

namespace States {
namespace Assets {
class MenuOverlay;

class MenuController : public BGE::AbstractController
{
  public:
    MenuController();

    inline void setOverlay(MenuOverlay *overlay)
    {
      m_overlay = overlay;
    }

    void keyPressed(QKeyEvent *event);

  private:
    MenuOverlay *m_overlay;

    void execute();
    void populateAsteroids();
    void setRestraints();
};

}
}

#endif
