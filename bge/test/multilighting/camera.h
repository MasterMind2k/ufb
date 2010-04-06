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
#ifndef CAMERA_H
#define CAMERA_H

#include "scene/object.h"

class Camera : public BGE::Scene::Object
{
  public:
    enum States {
      Stay,
      Forward,
      Backward,
      Downward,
      Upward,
      TurnLeft,
      TurnRight,
      TurnUp,
      TurnDown
    };

    inline Camera() : BGE::Scene::Object()
    {
      setState(Stay);
    }

    void setState(States state);
    inline States state() const
    {
      return m_state;
    }

    inline const QString& stateString() const
    {
      return m_stateString;
    }

  private:
    States m_state;
    QString m_stateString;

    void calculateTransforms(qint32 timeDiff);

};

#endif
