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
#ifndef __BGE_RENDERING_SHADERMANAGER_H
#define __BGE_RENDERING_SHADERMANAGER_H

#include <QtCore/QString>

namespace BGE {
namespace Storage {
class ShaderProgram;
}

namespace Rendering {

class ShaderManager
{
  public:
    ShaderManager();

    void bindProgram(Storage::ShaderProgram* shaderProgram);
    void unbindProgram(Storage::ShaderProgram* shaderProgram);

    void bindAttribute(const QString& name, quint32 size, quint32 stride, quint32 offset, Storage::ShaderProgram* shaderProgram);

    void useProgram(Storage::ShaderProgram* shaderProgram);
    void unload();

  private:
    char** prepareSource(const QString& source, qint32& count, qint32** length);
};

}
}

#endif
