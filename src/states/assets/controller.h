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
#ifndef STATES_ASSETS_CONTROLLER_H
#define STATES_ASSETS_CONTROLLER_H

#include "abstractcontroller.h"

#include <QtCore/QObject>

class QTimer;

namespace Objects {
class Fighter;
}

namespace States {
namespace Assets {

class Controller : public QObject, public BGE::AbstractController
{
  Q_OBJECT
  public:
    Controller(Objects::Fighter *fighter);

    void keyPressed(QKeyEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseButtonPressed(QMouseEvent *event);
    void mouseButtonReleased(QMouseEvent *event);

  private:
    Objects::Fighter *m_fighter;
    QTimer *m_timer;

  private slots:
    void fire();
};

}
}

#endif
