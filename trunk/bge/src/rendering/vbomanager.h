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
#ifndef __BGE_RENDERING_VBOMANAGER_H
#define __BGE_RENDERING_VBOMANAGER_H

#include "buffermanager.h"

namespace BGE {
namespace Scene {
class Object;
}
namespace Rendering {

class VBOManager : public BufferManager
{
  public:
    /*
     * Use this constructor only in BufferManager::init()!
     */
    VBOManager();

    /*
     * Make sure you check if VBO is available!
     */
    bool isAvailable() const;

    void bindObject(Scene::Object* object);
    void removeObject(Scene::Object* object);
    void unbindObject(Scene::Object* object);
    void drawObject(Scene::Object* object) const;

  private:
    void prepareObjectBuffer(Scene::Object* object);

};

}
}

#endif
