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
#ifndef STATES_GAME_H
#define STATES_GAME_H

#include "gamestate.h"

namespace Objects {
class Fighter;
}

namespace States {

class Game : public BGE::GameState
{
  public:
    Game();

    inline Objects::Fighter *fighter() const
    {
      return m_fighter;
    }

  private:
    Objects::Fighter *m_fighter;
};

}

#endif
