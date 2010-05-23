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
#ifndef OBJECTS_ASTEROID_H
#define OBJECTS_ASTEROID_H

#include "object.h"

namespace Objects {

class Asteroid : public Object
{
  public:
    enum Sizes {
      Large, /* 2000 */
      Medium, /* 1500 */
      Small /* 1000 */
    };

    Asteroid(Sizes size);

    qreal radius() const;

    inline Sizes size() const
    {
      return m_size;
    }

  private:
    Sizes m_size;

    void postTransformCalculations(qint32 timeDiff);
};

}

#endif
