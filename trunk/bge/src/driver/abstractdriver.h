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

#ifndef __BGE_DRIVER_ABSTRACTDRIVER_H
#define __BGE_DRIVER_ABSTRACTDRIVER_H

#include "global.h"

class QString;
class QMatrix4x4;

namespace BGE {

namespace Scene {
class Object;
class Light;
}

namespace Storage {
class Mesh;
class Material;
class Texture;
class Shader;
class ShaderProgram;
}

namespace Driver {

class AbstractDriver
{
  public:
    static AbstractDriver* self();

    inline virtual void bindFBO() {}
    inline virtual void unbindFBO() {}
    virtual void bind(Storage::Mesh* mesh) = 0;
    virtual void bind(const QHash<QString, Storage::Material*> &materials) = 0;
    virtual void bind(Storage::Texture* texture) = 0;
    virtual void bind(Storage::ShaderProgram* shaderProgram) = 0;

    virtual void unbind(Storage::Mesh* mesh) = 0;
    virtual void unbind(const QHash<QString, Storage::Material*> &materials) = 0;
    virtual void unbind(Storage::Texture* texture) = 0;
    virtual void unbind(Storage::ShaderProgram* shaderProgram) = 0;

    virtual void unload(Storage::Mesh* mesh) = 0;
    virtual void unload(Storage::Texture* texture) = 0;
    virtual void unload(Storage::ShaderProgram* shaderProgram) = 0;

    virtual void setLight(Scene::Light* light) = 0;
    virtual void resetLighting() = 0;

    virtual void setTransformMatrix(const Transform3f& transform) = 0;

    virtual void draw() = 0;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void setProjection(const Transform3f &projection) = 0;

    virtual void shading() = 0;

  protected:
    inline AbstractDriver() {}

  private:
    static AbstractDriver* m_self;
};

}
}

#endif
