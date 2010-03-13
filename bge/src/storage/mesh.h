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
#include <QtCore/QVector>
#include <QtCore/QStringList>
#include <QtCore/QSet>

#include "storage/item.h"

#include "global.h"

namespace BGE
{

class Mesh : public Item
{
  public:
    enum Primitives {
      Quads,
      Triangles
    };

    inline Mesh(const QString& name) : Item(name)
    {
      m_bindId = 0;
    }

    void addVertices(const QString& name, const VectorList& vertices);
    inline void addVertices(const QString& name, const QVector<Vector3f>& vertices)
    {
      addVertices(name, vertices.toList());
    }
    inline QVector<Vector3f> vertices(const QString& name) const
    {
      return m_vertices.value(name);
    }

    void addFace(const QString& name, Primitives primitive, const QVector<quint16>& face);
    inline QList<QPair<Primitives, QVector<quint16> > > faces(const QString& name) const
    {
      return m_faces.value(name);
    }

    inline QVector<Vector3f> normals(const QString& name) const
    {
      return m_normals.value(name);
    }

    inline void bind(quint32 id)
    {
      m_bindId = id;
    }
    inline quint32 bindId() const
    {
      return m_bindId;
    }

    inline void createObject(const QString& name)
    {
      m_objects << name;
    }
    inline QStringList objects() const
    {
      return m_objects.toList();
    }

    void addRectangle(const QString& objectName, const Vector3f& bottomLeft, const Vector3f& bottomRight, const Vector3f& topLeft, const Vector3f& topRight);

  private:
    QSet<QString> m_objects;
    QHash<QString, QVector<Vector3f> > m_vertices;
    QHash<QString, QList<QPair<Primitives, QVector<quint16> > > > m_faces;
    QHash<QString, QVector<Vector3f> > m_normals;

    quint32 m_bindId;

};

}

inline uint qHash(BGE::Mesh::Primitives primitive)
{
  return (uint) primitive;
}

typedef QPair<BGE::Mesh::Primitives, QVector<quint16> > Face;

#endif
