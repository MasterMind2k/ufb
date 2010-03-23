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
#ifndef __BGE_STORAGE_MESH_H
#define __BGE_STORAGE_MESH_H

#include <QtCore/QMultiMap>
#include <QtCore/QVector>
#include <QtCore/QStringList>
#include <QtCore/QSet>

#include <QtGui/QColor>

#include "storage/item.h"

#include "global.h"

namespace BGE {
namespace Storage {

class Mesh : public Item
{
  public:
    /**
     * Rendering primitives
     */
    enum Primitives {
      Quads,
      Triangles
    };

    inline Mesh(const QString& name) : Item(name) {}

    /**
     * Adds vertices to the list.
     */
    void addVertices(const QString& name, const VectorList& vertices);
    /**
     * \overload
     */
    inline void addVertices(const QString& name, const QVector<Vector3f>& vertices)
    {
      addVertices(name, vertices.toList());
    }
    /**
     * Gets vertices.
     */
    inline QVector<Vector3f> vertices(const QString& name) const
    {
      return m_vertices.value(name);
    }

    /**
     * Adds a face.
     *
     * @param primitive The primitive used at rendering/binding
     * @param face A vector of _index-es_ of bounding vertices
     */
    void addFace(const QString& name, Primitives primitive, const QVector<quint16>& face);
    /**
     * Gets a list of faces.
     *
     * @note You can use Face type for conviniance.
     */
    inline QList<QPair<Primitives, QVector<quint16> > > faces(const QString& name) const
    {
      return m_faces.value(name);
    }

    /**
     * Calculates and sets the normals.
     */
    void calculateNormals(const QString& name);
    /**
     * Gets the normals.
     */
    inline QVector<Vector3f> normals(const QString& name) const
    {
      return m_normals.value(name);
    }

    /**
     * Adds uv mapping for textures.
     *
     * @warning Make sure you add as much mappings as you have vertices!
     */
    inline void addTextureMap(const QString& name, const Vector2f& map)
    {
      QVector<Vector2f> temp = m_textureMaps.value(name);
      temp += map;
      m_textureMaps.insert(name, temp);
    }
    /**
     * Gets the uv texture mappings.
     */
    inline QVector<Vector2f> textureMaps(const QString& name) const
    {
      return m_textureMaps.value(name);
    }

    /**
     * Adds the object name to the list.
     */
    inline void createObject(const QString& name)
    {
      m_objects << name;
    }
    /**
     * Get a lst of all object names.
     */
    inline QStringList objects() const
    {
      return m_objects.toList();
    }

    /**
     * A conveniance method for creating rectangles.
     */
    void addRectangle(const QString& objectName, const Vector3f& bottomLeft, const Vector3f& bottomRight, const Vector3f& topLeft, const Vector3f& topRight);

  private:
    QSet<QString> m_objects;
    /* Vertices */
    QHash<QString, QVector<Vector3f> > m_vertices;
    /* Faces */
    QHash<QString, QList<QPair<Primitives, QVector<quint16> > > > m_faces;
    /* Normal vectors */
    QHash<QString, QVector<Vector3f> > m_normals;
    /* The uv texture mapping */
    QHash<QString, QVector<Vector2f> > m_textureMaps;
};

}
}

inline uint qHash(BGE::Storage::Mesh::Primitives primitive)
{
  return (uint) primitive;
}

typedef QPair<BGE::Storage::Mesh::Primitives, QVector<quint16> > Face;

#endif
