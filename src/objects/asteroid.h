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

#include <QtCore/QHash>

#include "scene/object.h"

class btRigidBody;
class btConvexHullShape;

namespace Objects {

class Asteroid : public BGE::Scene::Object
{
  public:
    Asteroid();

    void initBody();

    inline btRigidBody *body() const
    {
      return m_body;
    }

  private:
    btRigidBody *m_body;

    void postTransformCalculations(qint32 timeDiff);

    static QHash<void*, QPair<float*, quint16> > m_convexHullCache;
};

}

#endif
