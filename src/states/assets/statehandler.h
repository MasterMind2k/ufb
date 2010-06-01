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

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtCore/QList>

namespace Util {
class Ai;
}

namespace States {
class Game;
namespace Assets {

  class StateHandler : public QObject
{
  Q_OBJECT
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
    inline const Game *game() const
    {
      return m_game;
    }

    inline quint8 wave() const
    {
      return m_wave;
    }
    inline qint32 nextWaveIn() const
    {
      return m_waitingWave ? m_nextWave.elapsed() : 0;
    }

  private:
    static StateHandler *m_self;
    Game *m_game;
    QList<Util::Ai*> m_ais;
    quint8 m_wave;
    qint32 m_previousTime;
    QTime m_nextWave;
    QTimer *m_checkTimer;
    bool m_waitingWave;

    /* Singleton class */
    StateHandler();

    void populateAsteroids();
    void setRestraints();

  private slots:
    void checkAis();
    void nextWave();
};

}
}

#endif
