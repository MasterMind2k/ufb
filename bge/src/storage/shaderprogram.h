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
#ifndef __BGE_STORAGE_SHADERPROGRAM_H
#define __BGE_STORAGE_SHADERPROGRAM_H

#include "storage/item.h"

namespace BGE {
namespace Storage {
class Shader;

class ShaderProgram : public Item
{
  public:
    inline ShaderProgram(const QString& name) : Item(name)
    {
      m_hasFailed = false;
    }

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

    inline void setFailed(bool failed)
    {
      m_hasFailed = failed;
    }
    inline bool hasFailed() const
    {
      return m_hasFailed;
    }

    void bind();
    void unbind();
    void unload();

  private:
    QList<Shader*> m_shaders;
    bool m_hasFailed;
};

}
}

#endif
