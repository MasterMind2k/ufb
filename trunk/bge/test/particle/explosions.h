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
#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include <QtCore/QObject>
#include <QtCore/QQueue>

class QTimer;
class Explosion;

class Explosions : public QObject
{
  Q_OBJECT
  public:
    Explosions(QObject *parent);

  private:
    QTimer *m_timer;

  private slots:
    void ignite();
};

#endif
