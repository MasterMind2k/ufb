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

namespace Objects {
class Fighter;
}

namespace States {
namespace Assets {

class HUD : public BGE::AbstractOverlay
{
  public:
    HUD(Objects::Fighter *fighter);

  private:
    Objects::Fighter *m_fighter;

    void paint(QPainter *painter, qint32 elapsed);
};

}
}

#endif
