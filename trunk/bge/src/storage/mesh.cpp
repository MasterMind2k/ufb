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
#include "mesh.h"

using namespace BGE;

void Mesh::addVertices(const QString& name, const VectorList& vertices)
{
  if (!m_objects.contains(name))
    m_objects << name;

  QVector<Vector3f> temp = m_vertices.value(name);
  temp += vertices.toVector();

  m_vertices.insert(name, temp);
}

void Mesh::addFace(const QString &name, Primitives primitive, const QVector<quint16> &face)
{
  if (!m_objects.contains(name)) {
    m_objects << name;
  }

  // Calculate normal vector
  Vector3f normal = Vector3f::Zero();
  if (face.size() > 3 && !m_vertices.value("name").isEmpty()) {
    Vector3f a = m_vertices.value(name).at(face.at(0)) - m_vertices.value(name).at(face.at(1));
    Vector3f b = m_vertices.value(name).at(face.at(2)) - m_vertices.value(name).at(face.at(1));
    normal = b.cross(a);
    normal.normalize();
  }
  {
    QVector<Vector3f> temp = m_normals.value(name);
    temp << normal;
    m_normals.insert(name, temp);
  }

  // Add the face
  QPair<Primitives, QVector<quint16> > pair(primitive, face);
  QList<Face> temp = m_faces.value(name);
  temp <<  pair;
  m_faces.insert(name, temp);
}

void Mesh::addRectangle(const QString& objectName, const Vector3f& bottomLeft, const Vector3f& bottomRight, const Vector3f& topLeft, const Vector3f& topRight)
{
  if (!m_objects.contains(objectName))
    m_objects << objectName;

  VectorList vertices;
  QVector<Vector3f> vectors = m_vertices.value(objectName);
  if (!vectors.contains(bottomLeft))
    vertices << bottomLeft;
  if (!vectors.contains(bottomRight))
    vertices << bottomRight;
  if (!vectors.contains(topLeft))
    vertices << topLeft;
  if (!vectors.contains(topRight))
    vertices << topRight;
  addVertices(objectName, vertices);

  // Need to get the indexes right
  vectors = m_vertices.value(objectName);
  QVector<quint16> face;

  // Bottom left
  face << vectors.indexOf(bottomLeft);
  // Bottom right
  face << vectors.indexOf(bottomRight);
  // Top right
  face << vectors.indexOf(topRight);
  // Top left
  face << vectors.indexOf(topLeft);

  addFace(objectName, Quads, face);
}
