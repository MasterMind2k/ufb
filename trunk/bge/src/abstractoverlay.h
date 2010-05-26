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
#ifndef __BGE_ABSTRACTOVERLAY_H
#define __BGE_ABSTRACTOVERLAY_H

#include "global.h"

#include <QtCore/QObject>

class QPainter;

namespace BGE {
class Canvas;

class AbstractOverlay
{
  public:
    inline AbstractOverlay() {}

    virtual void paint(QPainter* painter, qint32 elapsed) = 0;

    inline Canvas* canvas() const
    {
      return m_canvas;
    }

  private:
    Canvas* m_canvas;

  friend class BGE::Canvas;
};

}

#endif
