/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
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
#ifndef __BGE_LOADER_SHADERLOADER_H
#define __BGE_LOADER_SHADERLOADER_H

#include <QtCore/QString>

namespace BGE {
class Shader;
namespace Loader {

class ShaderLoader
{
  public:
    ShaderLoader(const QString& filename);

    inline Shader* shader()
    {
      return m_shader;
    }

  private:
    void parse();
    Shader* m_shader;

    QString m_filename;
};

}
}

#endif