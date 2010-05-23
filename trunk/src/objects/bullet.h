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
#ifndef OBJECTS_BULLET_H
#define OBJECTS_BULLET_H

#include "object.h"

namespace Objects {

class Bullet : public Objects::Object
{
  public:
    Bullet();

    static const quint16 MaxLifetime = 3000;
    static qreal Velocity;

  private:
    quint16 m_lifetime;

    void postTransformCalculations(qint32 timeDiff);
};

}

#endif
