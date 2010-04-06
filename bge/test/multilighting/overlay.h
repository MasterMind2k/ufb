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
#ifndef OVERLAY_H
#define OVERLAY_H

#include "abstractoverlay.h"

class Camera;

class Overlay : public BGE::AbstractOverlay
{
  public:
    Overlay(Camera *camera) : BGE::AbstractOverlay()
    {
      m_camera = camera;
    }

  private:
    Camera *m_camera;

    void paint(QPainter *painter, qint32 elapsed);
};

#endif
