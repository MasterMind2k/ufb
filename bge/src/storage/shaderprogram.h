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
#ifndef __BGE_SHADERPROGRAM_H
#define __BGE_SHADERPROGRAM_H

#include "storage/item.h"

namespace BGE {
class Shader;

class ShaderProgram : public BGE::Item
{
  public:
    inline ShaderProgram(const QString& name) : Item(name) {}

    inline void addShader(Shader* shader)
    {
      if (shader)
        m_shaders << shader;
    }
    inline Shader* shader(int i) const
    {
      return m_shaders.at(i);
    }
    inline const QList<Shader*>& shaders() const
    {
      return m_shaders;
    }

    bool prepareProgram();

  private:
    QList<Shader*> m_shaders;
};

}

#endif
