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

#include "scene/boundingvolume.h"

#include "driver/abstractdriver.h"

#include "storage/material.h"

using namespace BGE;
using namespace BGE::Storage;

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

void Mesh::addFaces(const QString &name, const QList<Face> &faces)
{
  if (!m_objects.contains(name))
    m_objects << name;

  m_faces.insert(name, faces);
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

const Scene::BoundingVolume *Mesh::boundingVolume() const
{
  return m_boundingVolume;
}

Scene::BoundingVolume *Mesh::calculateBoundingVolume()
{
  if (m_boundingVolume)
    return new Scene::BoundingVolume(m_boundingVolume->radius(), m_boundingVolume->min(), m_boundingVolume->max());

  QList<QVector<Vector3f> > vertices = m_vertices.values();
  Vector3f min, max;
  float radius = 0;

  for (QList<QVector<Vector3f> >::const_iterator i = vertices.constBegin(); i != vertices.constEnd(); i++) {
    foreach (Vector3f vertex, *i) {
      radius = qMax(vertex.norm(), radius);
      min.x() = qMin(vertex.x(), min.x());
      max.x() = qMax(vertex.x(), max.x());
      min.y() = qMin(vertex.y(), min.y());
      max.y() = qMax(vertex.y(), max.y());
      min.z() = qMin(vertex.z(), min.z());
      max.z() = qMax(vertex.z(), max.z());
    }
  }

  m_boundingVolume = new Scene::BoundingVolume(radius, min, max);
  return new Scene::BoundingVolume(m_boundingVolume->radius(), m_boundingVolume->min(), m_boundingVolume->max());
}

void Mesh::bind()
{
  Item::bind();
  Driver::AbstractDriver::self()->bind(this);
}

void Mesh::unbind()
{
  Driver::AbstractDriver::self()->unbind(this);
}

void Mesh::unload()
{
  Driver::AbstractDriver::self()->unload(this);
}
