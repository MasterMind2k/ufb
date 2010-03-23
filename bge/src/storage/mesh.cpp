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

#include <QtCore/QMap>

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
  if (!m_objects.contains(name))
    m_objects << name;

  // Add the face
  Face pair(primitive, face);
  QList<Face> temp = m_faces.value(name);
  temp <<  pair;
  m_faces.insert(name, temp);
}

void Mesh::calculateNormals(const QString& name)
{
  if (!m_objects.contains(name))
    return;

  // Calculate normal vectors
  QList<Face> faces = m_faces.value(name);
  QVector<Vector3f> vertices = m_vertices.value(name);
  QMultiMap<quint16, Vector3f> faceNormals;
  foreach (Face face, faces) {
    // Let's calculate the normal vector
    Vector3f normal = Vector3f::Zero();
    if (face.second.size() >= 3 && !m_vertices.value(name).isEmpty()) {
      Vector3f a = vertices.at(face.second.at(0)) - vertices.at(face.second.at(1));
      Vector3f b = vertices.at(face.second.at(2)) - vertices.at(face.second.at(1));
      normal = b.cross(a).normalized();
    }

    // For each vertex we add a normal
    foreach (quint16 vertexIdx, face.second)
      faceNormals.insert(vertexIdx, normal);
  }

  // Calculate per-vertex normal
  quint16 size = vertices.size();
  QVector<Vector3f> normals = m_normals.value(name);
  for (quint16 i = 0; i < size; i++) {
    VectorList vertices = faceNormals.values(i);
    Vector3f normal = Vector3f::Zero();

    if (vertices.size()) {
      foreach (Vector3f absoluteNormal, vertices)
        normal += absoluteNormal;
      normal = (normal / vertices.size()).normalized();
    }

    normals << normal;
  }
  m_normals.insert(name, normals);
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
