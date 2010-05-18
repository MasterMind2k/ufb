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
#include "storage/loader/loader3ds.h"

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QVector>

#include "global.h"

#include "storage/mesh.h"
#include "storage/material.h"

using namespace BGE;
using namespace BGE::Storage;
using namespace BGE::Storage::Loader;

Item* Loader3DS::load()
{
  QFile modelFile(filename());
  if (!modelFile.open(QFile::ReadOnly)) {
    qWarning("BGE::Loader::Loader3DS::load(): Cannot open model '%s'.", filename().toUtf8().data());
    return 0l;
  }

  Mesh* mesh = new Mesh(name());

  // Material hash
  QHash<QString, Material*> materials;

  // Temporary variables
  VectorList vertices;
  QString objectName;
  Material* material = 0l;
  QString materialName;
  QHash<QString, FaceMaterial> faceMaterials;

  enum MaterialState {
    Ambient,
    Diffuse,
    Specular,
    Shininess
  } materialState;

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
      // Material block
      case 0xAFFF:
        // Nothing to do :)
        break;

      // Object block
      case 0x4000: {
        objectName = readString(modelFile);

        qDebug("BGE::Loader::Loader3DS::parse(): Parsing object '%s'", objectName.toUtf8().data());
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
        mesh->addVertices(objectName, vertices);

        if (!mesh->faces(objectName).isEmpty())
          mesh->calculateNormals(objectName);
        free(coordinates);

        qDebug("BGE::Loader::Loader3DS::parse(): Parsed %d vertices.", verticesNumber);
        break;
      }

      // Faces description
      case 0x4120: {
        bool calculateNormals = !vertices.isEmpty();

        quint16 facesNumber = *(quint16*) modelFile.read(2).data();
        size_t size = 4 * sizeof(quint16);
        quint16* vertexList = (quint16*) malloc(size);
        QVector<quint16> temp;
        temp.reserve(3);
        QList<Face> faces;
        for (quint16 i = 0; i < facesNumber; i++) {
          modelFile.read((char*) vertexList, size);
          temp.clear();
          temp << vertexList[0] << vertexList[1] << vertexList[2];
          faces << Face(Mesh::Triangles, temp);
        }
        free(vertexList);
        mesh->addFaces(objectName, faces);

        if (calculateNormals)
          mesh->calculateNormals(objectName);

        qDebug("BGE::Loader::Loader3DS::parse(): Parsed %d faces.", facesNumber);
        break;
      }

      // Faces material list
      case 0x4130: {
        QString materialName = readString(modelFile);
        quint16 facesNum = *(quint16*) modelFile.read(2).data();

        FaceMaterial mat = faceMaterials.value(objectName);
        for (quint16 i = 0; i < facesNum; i++) {
          quint16 faceIdx = *(quint16*) modelFile.read(2).data();
          mat.insert(faceIdx, materialName);
        }
        faceMaterials.insert(objectName, mat);

        qDebug("BGE::Loader::Loader3DS::parse(): Parsed %d material mappings for faces.", facesNum);
        break;
      }

      // Mapping coordinates list for each vertex
      case 0x4140: {
        quint16 verticesNumber = *(quint16*) modelFile.read(2).data();
        size_t size = 2* sizeof(float);
        float* raw = (float*) malloc(size);
        QVector<Vector2f> textureMaps;
        for (quint16 i = 0; i < verticesNumber; i++) {
          modelFile.read((char*) raw, size);
          textureMaps << Vector2f(raw);
        }
        free(raw);
        mesh->addTextureMaps(objectName, textureMaps);

        qDebug("BGE::Loader::Loader3DS::parse(): Parsed %d texture mappings for vertices.", verticesNumber);
        break;
      }

      // Material name
      case 0xA000: {
        materialName = readString(modelFile);
        material = new Material(materialName);
        mesh->addItem(material);
        materials.insert(materialName, material);

        qDebug("BGE::Storage::Loader:Loader3DS::parse(): Parsing material '%s'.", materialName.toUtf8().data());
        break;
      }

      // Material ambient color
      case 0xA010: {
        materialState = Ambient;

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsing material '%s' ambient color.", materialName.toUtf8().data());
        break;
      }

      // Material diffuse color
      case 0xA020 : {
        materialState = Diffuse;

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsing material '%s' diffuse color.", materialName.toUtf8().data());
        break;
      }

      // Material specular color
      case 0xA030 : {
        materialState = Specular;

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsing material '%s' specular color.", materialName.toUtf8().data());
        break;
      }

      // Material shininess
      case 0xA040 : {
        materialState = Shininess;

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsing material '%s' shininess.", materialName.toUtf8().data());
        break;
      }

      // Color (byte)
      case 0x0011: {
        quint8 red = *(quint8*) modelFile.read(1).data();
        quint8 green = *(quint8*) modelFile.read(1).data();
        quint8 blue = *(quint8*) modelFile.read(1).data();
        QColor color(red, green, blue);
        switch (materialState) {
          case Ambient:
            material->setAmbient(color);
            break;
          case Diffuse:
            material->setDiffuse(color);
            break;
          case Specular:
            material->setSpecular(color);
            break;
          default:
            break;
        }

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsed color (%d, %d, %d).", red, green, blue);
        break;
      }

      // Percent (int format)
      case 0x0030 : {
        quint16 percent = *(quint16*) modelFile.read(2).data();
        if (materialState != Shininess)
          break;
        material->setShininess(percent);

        qDebug("BGE::Storage::Loader::Loader3DS::parse(): Parsed procent (%d).", percent);
        break;
      }

      // A complete chunk skip
      default:
        modelFile.seek(modelFile.pos() + length - 6);
    }
  }

  // Add materials
  foreach (QString objectName, faceMaterials.keys())
    mesh->addFacesMaterials(objectName, faceMaterials.value(objectName));

  return mesh;
}

QString Loader3DS::readString(QFile& file) const
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
