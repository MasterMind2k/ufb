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
#ifndef __BGE_RENDERING_BUFFERMANAGER_H
#define __BGE_RENDERING_BUFFERMANAGER_H

#include <QtCore/QHash>
#include "global.h"

namespace BGE {
namespace Scene {
class Object;
}
namespace Rendering {

class BufferManager
{
  public:
    static BufferManager* init();

    virtual void bindObject(Scene::Object* object) = 0;
    virtual void removeObject(Scene::Object* object) = 0;
    virtual void unbindObject(Scene::Object* object) = 0;
    virtual void drawObject(Scene::Object* object) const = 0;

  protected:

    struct Plan {
      quint32 primitive;
      quint32 count;
      quint32 offset;
    };

    inline BufferManager() {}
    QHash<quint32, quint32> m_indices;
    QHash<quint32, QList<Plan> > m_drawingPlans;
};

}
}

#endif
