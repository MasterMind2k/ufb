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
#ifndef __BGE_DRIVER_GL3_H
#define __BGE_DRIVER_GL3_H

#include "driver/abstractdriver.h"

#include <QtCore/QHash>

namespace BGE {
namespace Driver {

class GL3 : public AbstractDriver
{
  public:
    GL3();

    void bind(Storage::Mesh* mesh);
    void bind(Storage::Texture* texture);
    void bind(Storage::ShaderProgram* shaderProgram);

    void unbind(Storage::Mesh* mesh);
    inline void unbind(Storage::Texture* texture) {/* Ignore */}
    void unbind(Storage::ShaderProgram* shaderProgram);

    void unload(Storage::Mesh* mesh);
    void unload(Storage::Texture* texture);
    void unload(Storage::ShaderProgram* shaderProgram);

    void setLight(Scene::Light* light);
    void resetLighting();

    void setTransformMatrix(const Transform3f& transform);

    void draw(Scene::Object* object);

    void init();
    void clear();
    void setProjection(const QMatrix4x4& transform);

  private:
    struct Plan {
      quint32 primitive;
      quint32 count;
      quint32 offset;
    };
    quint8 m_usedLights;
    QHash<quint32, quint32> m_indices;
    QHash<quint32, QList<Plan> > m_plans;

    void load(Storage::Mesh* mesh);
    void load(Storage::ShaderProgram* shaderProgram);

    static void bindAttribute(Storage::ShaderProgram* shaderProgram, QString name, qint32 size, quint32 type, quint32 stride, quint32 offset);
    static void unbindAttribute(Storage::ShaderProgram* shaderProgram, QString name);

    static char** prepareShaderSource(const QString &source, qint32 &count, qint32 **length);
};

}
}

#endif
