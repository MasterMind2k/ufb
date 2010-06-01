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
#ifndef OBJECTS_POWERUP_H
#define OBJECTS_POWERUP_H

#include "objects/object.h"

namespace BGE {
namespace Storage {
class Material;
}
}

namespace Objects {

class PowerUp : public Objects::Object
{
  public:
    enum Type {
      LaserEnhancement,
      HullRepair,
      HullEnhancement,
      ShieldsBatteries
    };

    PowerUp();
    ~PowerUp();

    inline Type type() const
    {
      return m_type;
    }

    void initBody();

  private:
    Type m_type;
    qint32 m_spawnAnimation;
    static const qint32 SpawnTime = 1000;
    bool m_initialized;
    BGE::Storage::Material *m_glow;
    QColor m_color;

    void collision(BGE::Scene::Object *object);
    void calculateTransforms(qint32 timeDiff);
    void postTransformCalculations(qint32 timeDiff);
};

}

#endif
