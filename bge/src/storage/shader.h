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
#ifndef __BGE_SHADER_H
#define __BGE_SHADER_H

#include "storage/item.h"

namespace BGE {

class Shader : public BGE::Item
{
  public:
    enum Type {
      Vertex,
      Fragment
    };
    Shader(const QString& name);

    inline void setShaderSource(const QString& source, Type type)
    {
      m_source = source;
      m_type = type;
    }
    inline const QString& shaderSource() const
    {
      return m_source;
    }
    inline Type type() const
    {
      return m_type;
    }

  private:
    QString m_source;
    Type m_type;
};

}

#endif
