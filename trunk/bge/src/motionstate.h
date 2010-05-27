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
#ifndef __BGE_MOTIONSTATE_H
#define __BGE_MOTIONSTATE_H

#include "LinearMath/btMotionState.h"

#include "scene/object.h"

namespace BGE {

/**
 * A simple bridge between BGE and Bullet.
 *
 * Use this class to map BGE Object with Bullet's body. If you need something
 * extra, subclass btMotionState from Bullet.
 */
class MotionState : public btMotionState
{
  public:
    inline MotionState(Scene::Object *object) : btMotionState()
    {
      m_object = object;
    }

    inline void setWorldTransform(const btTransform &worldTrans)
    {
      btVector3 pos = worldTrans.getOrigin();
      btQuaternion rot = worldTrans.getRotation();
      m_object->setPosition(pos.x(), pos.y(), pos.z());
      m_object->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
    }
    inline void getWorldTransform(btTransform &worldTrans) const
    {
      if (m_object->isTransformModified()) {
        /* Calculate from local position and rotation, since physics should
           be used only on global objects */
        worldTrans.setRotation(btQuaternion(m_object->orientation().x(), m_object->orientation().y(), m_object->orientation().z(), m_object->orientation().w()));
        worldTrans.setOrigin(btVector3(m_object->position().x(), m_object->position().y(), m_object->position().z()));
      } else {
        worldTrans.setFromOpenGLMatrix(m_object->globalTransform().data());
      }
    }

  private:
    Scene::Object *m_object;
};

}

#endif
