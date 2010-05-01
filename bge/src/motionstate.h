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
      worldTrans.setFromOpenGLMatrix(m_object->globalTransform().data());
    }

  private:
    Scene::Object *m_object;
};

}

#endif
