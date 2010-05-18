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
#ifndef STATES_ASSETS_MENUOVERLAY_H
#define STATES_ASSETS_MENUOVERLAY_H

#include "abstractoverlay.h"

#include <QtGui/QRadialGradient>
#include <QtGui/QFont>

class QPointF;

namespace States {
namespace Assets {

class MenuOverlay : public BGE::AbstractOverlay
{
  public:
    enum Buttons {
      Play,
      Quit
    };

    enum Directions {
      Up,
      Down
    };

    MenuOverlay();

    void moveSelection(Directions direction);

  private:
    QRadialGradient m_buttonGradient;
    QRadialGradient m_selectedButtonGradient;
    QSizeF m_buttonSize;
    QFont m_buttonTextFont;
    Buttons m_selectedButton;

    void paint(QPainter *painter, qint32 elapsed);
    void drawButton(Buttons button, const QRectF &geometry, QPainter *painter);
    static QString buttonText(Buttons button);
};

}
}

#endif
