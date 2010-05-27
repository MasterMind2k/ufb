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
#ifndef __BGE_STORAGE_SHADER_H
#define __BGE_STORAGE_SHADER_H

#include "storage/item.h"

#include <QtCore/QStringList>

namespace BGE {
namespace Storage {

/**
 * A shader item. A shader is a part of a shader program.
 */
class Shader : public Item
{
  public:
    /**
     * Supported shader types.
     */
    enum Type {
      VertexShader,
      FragmentShader
    };

    inline Shader(const QString& name) : Item(name) {}

    /**
     * Sets the shader source.
     */
    inline void setShaderSource(const QString& source, Type type)
    {
      m_source = source;
      m_type = type;
    }
    /**
     * Returns the shader source.
     */
    inline const QString &shaderSource() const
    {
      return m_source;
    }
    /**
     * Returns the shader type.
     */
    inline Type type() const
    {
      return m_type;
    }

    /**
     * Adds shader loading dependency.
     */
    inline void addDependency(const QString &dependency)
    {
      m_deps << dependency;
    }
    /**
     * Adds shader loading dependencies.
     */
    inline void addDependencies(const QStringList &dependencies)
    {
      m_deps += dependencies;
    }
    /**
     * Returns list of shader dependencies.
     */
    inline const QStringList &dependencies() const
    {
      return m_deps;
    }

  private:
    QString m_source;
    QStringList m_deps;
    Type m_type;
};

}
}

#endif
