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
#ifndef STATES_ASSETS_STATEHANDLER_H
#define STATES_ASSETS_STATEHANDLER_H

namespace BGE {
class GameState;
}

namespace States {
namespace Assets {

class StateHandler
{
  public:
    inline static StateHandler *self()
    {
      if (!StateHandler::m_self)
        StateHandler::m_self = new StateHandler;
      return StateHandler::m_self;
    }

    void play();
    void unload();
    void resume();
    inline bool hasGame() const
    {
      return m_game != 0l;
    }

  private:
    static StateHandler *m_self;
    BGE::GameState *m_game;

    /* Singleton class */
    StateHandler();

    void populateAsteroids();
    void setRestraints();
};

}
}

#endif
