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
#ifndef OBJECTS_OBJECT_H
#define OBJECTS_OBJECT_H

#include "scene/object.h"

#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"

class btRigidBody;

namespace Objects {

class Object : public BGE::Scene::Object
{
  public:
    Object();
    virtual ~Object();

    virtual void initBody();

    inline btRigidBody *body() const
    {
      return m_body;
    }

    Vector3f velocity() const;
    qreal distance(Object *object) const;

    inline void setMass(qreal mass)
    {
      m_mass = mass;
    }
    inline qreal mass() const
    {
      return m_mass;
    }

    void applyCentralImpulse(const Vector3f &direction);
    inline void applyCentralImpulse(qreal x, qreal y, qreal z)
    {
      applyCentralImpulse(Vector3f(x, y, z));
    }

    void applyCentralForce(const Vector3f &direction);
    inline void applyCentralForce(qreal x, qreal y, qreal z)
    {
      applyCentralForce(Vector3f(x, y, z));
    }

    virtual void setAngularVelocity(const Vector3f &velocity);
    inline void setAngularVelocity(qreal x, qreal y, qreal z)
    {
      setAngularVelocity(Vector3f(x, y, z));
    }

  private:
    btRigidBody *m_body;
    qreal m_mass;
    static btVoronoiSimplexSolver m_solver;
};

}

#endif
