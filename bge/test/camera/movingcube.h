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
#ifndef MOVINGCUBE_H
#define MOVINGCUBE_H

#include <QtCore/QObject>

#include "scene/object.h"

class MovingCube : public QObject, public BGE::Scene::Object
{
  Q_OBJECT
  public:
    MovingCube(int seed);

  private:
    void calculateTransforms();

    Vector3f m_direction;
    AngleAxisf m_rotation;

  private slots:
    void change();
};

#endif
