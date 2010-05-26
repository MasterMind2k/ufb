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
#ifndef STATES_ASSETS_HUD_H
#define STATES_ASSETS_HUD_H

#include "abstractoverlay.h"

class QSizeF;
class QRectF;

namespace Objects {
class Object;
class Fighter;
}

namespace States {
namespace Assets {

class HUD : public BGE::AbstractOverlay
{
  public:
    HUD();

    inline void setFighter(Objects::Fighter *fighter)
    {
      m_fighter = fighter;
    }

  private:
    Objects::Fighter *m_fighter;

    void paint(QPainter *painter, qint32 elapsed);
    static void paintNearestArrow(QPainter *painter, Objects::Object *nearest, const QSizeF &drawingSize);
    static void paintStatus(QPainter *painter, const QRectF &geometry, const QString &text);
};

}
}

#endif
