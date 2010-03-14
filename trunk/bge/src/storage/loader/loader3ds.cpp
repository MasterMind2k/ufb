/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
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
#include "storage/loader/loader3ds.h"

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QVector>

#include "global.h"

#include "storage/mesh.h"

using namespace BGE;
using namespace BGE::Loader;

void Loader3DS::parse()
{
  QFile modelFile(m_filename);
  if (!modelFile.open(QFile::ReadOnly)) {
    qWarning("BGE::Utils::ModelImporter::parse: Cannot open model.");
    return;
  }

  QString name = m_filename.split("/").last();
  m_mesh = new Mesh(name.split(".").first());

  // Temporary variables
  VectorList vertices;
  QString objectName;

  // Used the format definition from
  // http://jerome.jouvie.free.fr/OpenGl/Projects/3dsInfo.txt
  while (!modelFile.atEnd()) {
    // Read the identifier
    QByteArray temp = modelFile.read(2);
    quint16 identifier = *(quint16*) temp.data();
    temp = modelFile.read(4);
    quint32 length = *(quint32*) temp.data();

    switch (identifier) {
      // Main chunk
      case 0x4D4D:
      // 3D editor chunk
      case 0x3D3D:
      // Triangular mesh
      case 0x4100:
        // Nothing to do :)
        break;

      // Object block
      case 0x4000: {
        objectName = readString(modelFile);

        qDebug("BGE::Storage::Loader3DS::parse(): Parsing object '%s'", objectName.toUtf8().data());
        break;
      }

      // Vertices list
      case 0x4110: {
        vertices.clear();
        quint16 verticesNumber = *(quint16*) modelFile.read(2).data();
        size_t size = 3 * sizeof(float);
        float* coordinates = (float*) malloc(size);
        for (quint16 i = 0; i < verticesNumber; i++) {
          modelFile.read((char*) coordinates, size);
          vertices << Vector3f(coordinates);
        }
        m_mesh->addVertices(objectName, vertices);

        if (!m_mesh->faces(objectName).isEmpty())
          m_mesh->calculateNormals(objectName);
        free(coordinates);

        qDebug("BGE::Storage::Loader3DS::parse(): Parsed %d vertices.", verticesNumber);
        break;
      }

      // Faces description
      case 0x4120: {
        bool calculateNormals = !vertices.isEmpty();

        quint16 facesNumber = *(quint16*) modelFile.read(2).data();
        size_t size = 4 * sizeof(quint16);
        quint16* vertexList = (quint16*) malloc(size);
        for (quint16 i = 0; i < facesNumber; i++) {
          modelFile.read((char*) vertexList, size);
          QVector<quint16> temp;
          temp << vertexList[0] << vertexList[1] << vertexList[2];
          m_mesh->addFace(objectName, Mesh::Triangles, temp);
        }
        free(vertexList);

        if (calculateNormals)
          m_mesh->calculateNormals(objectName);

        qDebug("BGE::Storage::Loader3DS::parse(): Parsed %d faces.", facesNumber);
        break;
      }

      // Mapping coordinates list for each vertex
      case 0x4140: {
        quint16 verticesNumber = *(quint16*) modelFile.read(2).data();
        size_t size = 2* sizeof(float);
        float* raw = (float*) malloc(size);
        for (quint16 i = 0; i < verticesNumber; i++) {
          modelFile.read((char*) raw, size);
          m_mesh->addTextureMap(objectName, Vector2f(raw));
        }
        free(raw);

        qDebug("BGE::Storage::Loader3DS::parse(): Parsed %d texture mappings for vertices.", verticesNumber);
        break;
      }

      // A complete chunk skip
      default:
        modelFile.seek(modelFile.pos() + length - 6);
    }
  }
}

QString Loader3DS::readString(QFile& file)
{
  QString output;
  quint8 byte;
  while (true) {
    byte = file.read(1).at(0);
    if (byte == 0x00)
      break;

    output.append(byte);
  }
  return output;
}
