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
#ifndef __BGE_DRIVER_GL1_H
#define __BGE_DRIVER_GL1_H

#include <QtCore/QHash>

#include "driver/abstractdriver.h"

namespace BGE {

namespace Driver {

class GL1 : public AbstractDriver
{
  public:
    GL1();

    void bind(Storage::Mesh* mesh);
    inline void bind(const QHash<QString, Storage::Material*> &materials)
    {
      m_materials = materials;
    }
    void bind(Storage::Texture* texture);
    inline void bind(Storage::ShaderProgram* shaderProgram) {/* Ignore */}

    inline void unbind(Storage::Mesh* mesh) {/* Ignore */}
    inline void unbind(const QHash<QString, Storage::Material*> &materials)
    {
      m_materials.clear();
    }
    inline void unbind(Storage::Texture* texture) {/* Ignore */}
    inline void unbind(Storage::ShaderProgram* shaderProgram) {/* Ignore */}

    void unload(Storage::Mesh* mesh);
    void unload(Storage::Texture* texture);
    inline void unload(Storage::ShaderProgram* shaderProgram) {/* Ignore */};

    void setLight(Scene::Light* light);
    void resetLighting();

    void setTransformMatrix(const Transform3f& transform);

    void draw(Scene::Object* object);

    void init();
    void clear();
    void setProjection(const QMatrix4x4& transform);

  private:
    quint8 m_usedLights;
    QHash<QString, Storage::Material*> m_materials;
    QHash<QPair<quint32, QString>, quint32> m_meshBindings;
};

}
}

#endif
