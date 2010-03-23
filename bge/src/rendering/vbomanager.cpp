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
#include "vbomanager.h"

#include "buffer_p.h"

#include "scene/object.h"

#include "storage/mesh.h"

struct BufferElement {
  GLfloat vertex[3];
  GLfloat normal[3];
  GLfloat uvMap[2];
  char padding[32];
};

#define NORMAL_OFFSET (void*) (3 * sizeof(GLfloat))
#define UV_OFFSET (void*) (6 * sizeof(GLfloat))

using namespace BGE;
using namespace BGE::Rendering;

VBOManager::VBOManager()
 : BufferManager()
{
  getBufferFunctions();
}

bool VBOManager::isAvailable() const
{
  return hasBuffers;
}

void VBOManager::bindObject(Scene::Object *object)
{
  if (!object->mesh()->bindId()) {
    prepareObjectBuffer(object);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, object->mesh()->bindId());
    glNormalPointer(GL_FLOAT, sizeof(BufferElement), NORMAL_OFFSET);
    glTexCoordPointer(2, GL_FLOAT, sizeof(BufferElement), UV_OFFSET);
    glVertexPointer(3, GL_FLOAT, sizeof(BufferElement), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices.value(object->mesh()->bindId()));
    glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
  }
}

void VBOManager::removeObject(Scene::Object *object)
{
  if (!object->mesh() || !object->mesh()->bindId())
    return;

  quint32 bindId = object->mesh()->bindId();
  glDeleteBuffers(1, &bindId);
  bindId = m_indices.value(object->mesh()->bindId());
  glDeleteBuffers(1, &bindId);
  m_indices.remove(object->mesh()->bindId());
  object->mesh()->unbind();
}

void VBOManager::unbindObject(Scene::Object *object)
{
  if (!object->mesh() || !object->mesh()->bindId())
    return;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VBOManager::drawObject(Scene::Object *object) const
{
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  foreach (Plan plan, m_drawingPlans.value(object->mesh()->bindId()))
    glDrawElements(plan.primitive, plan.count, GL_UNSIGNED_SHORT, (GLushort*)0 + plan.offset);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void VBOManager::prepareObjectBuffer(Scene::Object *object)
{
  // Bind VBO for vertices and normals
  quint32 bindId;
  glGenBuffers(1, &bindId);
  glBindBuffer(GL_ARRAY_BUFFER, bindId);
  object->mesh()->setBindId(bindId);

  // Make vertices, normals and faces list
  QList<Vector3f> vertices;
  QList<Vector3f> normals;
  QList<Vector2f> textureMaps;
  QList<Face> faces;
  foreach (QString objectName, object->mesh()->objects()) {
    vertices += object->mesh()->vertices(objectName).toList();
    normals += object->mesh()->normals(objectName).toList();
    textureMaps += object->mesh()->textureMaps(objectName).toList();
    faces += object->mesh()->faces(objectName);
  }

  // Process vertices and normals
  {
    // Size of vertices and normals should be the same
    quint64 size = vertices.size();

    BufferElement* buffer = (BufferElement*) malloc(size * sizeof(BufferElement));
    BufferElement* bufferPtr = buffer;
    QList<Vector3f>::const_iterator i = vertices.constBegin();
    QList<Vector3f>::const_iterator j = normals.constBegin();
    QList<Vector2f>::const_iterator k = textureMaps.constBegin();
    QList<Vector3f>::const_iterator end = vertices.constEnd();
    while (i != end) {
      Vector3f vertex = *i++;
      Vector3f normal = *j++;
      Vector2f textureMap = Vector2f::Zero();
      if (!textureMaps.isEmpty())
        textureMap = *k++;

      // Prepare element
      BufferElement element;
      memcpy(element.vertex, vertex.data(), 3 * sizeof(GLfloat));
      memcpy(element.normal, normal.data(), 3 * sizeof(GLfloat));
      memcpy(element.uvMap, textureMap.data(), 2 * sizeof(GLfloat));

      // Copy element
      memcpy(bufferPtr, &element, sizeof(BufferElement));
      // Iterate
      bufferPtr++;
    }

    // Copy it to GPU
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(BufferElement), buffer, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, sizeof(BufferElement), NORMAL_OFFSET);
    glTexCoordPointer(2, GL_FLOAT, sizeof(BufferElement), UV_OFFSET);
    glVertexPointer(3, GL_FLOAT, sizeof(BufferElement), 0);
    free(buffer);
  }

  // Bind VBO for faces
  glGenBuffers(1, &bindId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bindId);
  m_indices.insert(object->mesh()->bindId(), bindId);

  // Process faces
  {
    quint32 size = 0;
    QList<Face>::const_iterator i = faces.constBegin();
    QList<Face>::const_iterator end = faces.constEnd();
    quint32 currentPrimitive = 0;
    quint32 count = 0;
    QList<Plan> plans;
    QVector<quint16> idxs;
    while (i != end) {
      Face face = *i++;
      quint32 primitive;
      switch (face.first) {
        case Storage::Mesh::Quads:
          primitive = GL_QUADS;
          break;
        case Storage::Mesh::Triangles:
          primitive = GL_TRIANGLES;
          break;
      }

      // Save the plan
      if (currentPrimitive != primitive) {
        if (currentPrimitive) {
          Plan plan;
          plan.primitive = primitive;
          plan.count = count;
          plan.offset = size - count;
          plans << plan;
          count = 0;
        }
        currentPrimitive = primitive;
      }

      idxs += face.second;
      count += face.second.size();
      size += face.second.size();
    }
    // And the last plan
    Plan plan;
    plan.count = count;
    plan.primitive = currentPrimitive;
    plan.offset = size - count;
    plans << plan;
    m_drawingPlans.insert(object->mesh()->bindId(), plans);

    quint32 padding = 0;
    if (size % 32)
      padding = 32 - size % 32;

    quint16* indices = (quint16*) malloc((size + padding) * sizeof(quint16));
    memcpy(indices, idxs.constData(), size * sizeof(quint16));
    // Copy the array to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size + padding) * sizeof(quint16), indices, GL_STATIC_DRAW);
    glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
    free(indices);
  }
}
