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
#ifndef __BGE_MESH_H
#define __BGE_MESH_H

#include <QtCore/QMultiMap>

#include "storage/item.h"

#include "global.h"

namespace BGE
{

class Mesh : public Item
{
  public:
    enum Primitives {
      Quads = 0x1
    };

    inline Mesh(const QString& name) : Item(name)
    {
      m_bindId = 0;
    }

    inline void addVertices(Primitives primitive, const VectorList& vertices)
    {
      QList<VectorList> temp = m_vertices.value(primitive);
      temp.append(vertices);
      m_vertices.insert(primitive, temp);
    }
    inline QList<VectorList> vertices(Primitives primitive) const
    {
      return m_vertices.value(primitive);
    }

    inline void bind(quint32 id)
    {
      m_bindId = id;
    }
    inline quint32 bindId() const
    {
      return m_bindId;
    }

  private:
    // Should contain normals and colors...
    QMultiMap<Primitives, QList<VectorList> > m_vertices;
    quint32 m_bindId;

};

}

#endif
