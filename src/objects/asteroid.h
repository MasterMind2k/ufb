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

    inline qreal structuralIntegrity() const
    {
      return m_structuralIntegrity;
    }

    void initBody();

    qreal maxStructuralIntegrity() const;

  private:
    Sizes m_size;
    qreal m_structuralIntegrity;
    qint32 m_dyingElapsedTime;
    bool m_exploded;
    bool m_stabilized;
    qint32 m_previousExplosion;

    void postTransformCalculations(qint32 timeDiff);
    void collision(BGE::Scene::Object *object);

    void spawn();
};

}

#endif
