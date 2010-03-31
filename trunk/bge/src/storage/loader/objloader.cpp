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
#include "objloader.h"

#include <QtCore/QFile>

#include "storage/mesh.h"

using namespace BGE;
using namespace BGE::Storage;
using namespace BGE::Storage::Loader;

Item *ObjLoader::load()
{
  enum Stage {
    Vertices,
    UVMapping,
    Normals,
    Faces
  } stage = Vertices;

  VectorList vertices;
  QList<Vector2f> uvMaps;
  VectorList normals;

  VectorList sortedVertices;
  QList<Vector2f> sortedUvs;
  VectorList sortedNormals;
  QList<Face> faces;

  QFile modelFile(filename());
  if (!modelFile.open(QFile::ReadOnly)) {
    qWarning("BGE::Storage::Loader::ObjLoader::load(): Cannot open '%s'!", filename().toUtf8().data());
    return 0l;
  }

  QRegExp comment("^\\s*#.*");
  QRegExp emptyLine("^\\s*");
  QRegExp vertex("^\\s*v\\s*(-?\\d+\\.?\\d*)\\s(-?\\d+\\.?\\d*)\\s(-?\\d+\\.?\\d*).*", Qt::CaseInsensitive, QRegExp::RegExp2);
  QRegExp uvMap("^\\s*vt\\s*(-?\\d+\\.?\\d*)\\s(-?\\d+\\.?\\d*).*", Qt::CaseInsensitive, QRegExp::RegExp2);
  QRegExp normal("^\\s*vn\\s*(-?\\d+\\.?\\d*)\\s(-?\\d+\\.?\\d*)\\s(-?\\d+\\.?\\d*).*", Qt::CaseInsensitive, QRegExp::RegExp2);
  QRegExp face("^\\s*f\\s(.*)");
  QRegExp faceComponent("(\\d+)/(\\d+)/(\\d+)");

  qDebug("BGE::Storage::Loader::ObjLoader::load(): Parsing file '%s'.", filename().toUtf8().data());
  while (!modelFile.atEnd()) {
    QString line = modelFile.readLine();
    line.remove('\n');
    if (comment.exactMatch(line) || emptyLine.exactMatch(line))
      continue;

    switch (stage) {
      case Vertices: {
        if (vertex.exactMatch(line)) {
          vertices << Vector3f(vertex.cap(1).toFloat(), vertex.cap(2).toFloat(), vertex.cap(3).toFloat());
          break;
        } else {
          qDebug("BGE::Storage::Loader::ObjLoader::load(): Parsed %d vertices.", vertices.size());
          stage = UVMapping;
        }
      }

      case UVMapping: {
        if (uvMap.exactMatch(line)) {
          uvMaps << Vector2f(uvMap.cap(1).toFloat(), uvMap.cap(2).toFloat());
          break;
        } else {
          qDebug("BGE::Storage::Loader::ObjLoader::load(): Parsed %d uv mappings.", uvMaps.size());
          stage = Normals;
        }
      }

      case Normals: {
        if (normal.exactMatch(line)) {
          normals << Vector3f(normal.cap(1).toFloat(), normal.cap(2).toFloat(), normal.cap(3).toFloat()).normalized();
          break;
        } else {
          qDebug("BGE::Storage::Loader::ObjLoader::load(): Parsed %d normals.", normals.size());
          stage = Faces;
        }
      }

      case Faces: {
        if (face.exactMatch(line)) {
          Face facePair;
          facePair.first = Mesh::Polygons;
          foreach (QString faceComponents, face.cap(1).split(QRegExp("\\s+"))) {
            if (faceComponent.exactMatch(faceComponents)) {
              Vector3f vertex = vertices.at(faceComponent.cap(1).toUShort() - 1);
              Vector2f uvMap = uvMaps.at(faceComponent.cap(2).toUShort() - 1);
              Vector3f normal = normals.at(faceComponent.cap(3).toUShort() - 1);

              // Check for existing triple
              bool found = false;
              for (quint16 i = 0; i < sortedVertices.size(); i++) {
                if (sortedVertices.at(i) == vertex && sortedUvs.at(i) == uvMap && sortedNormals.at(i) == normal) {
                  found = true;
                  facePair.second << i;
                  break;
                }
              }

              if (!found) {
                sortedVertices << vertex;
                sortedUvs << uvMap;
                sortedNormals << normal;
                facePair.second << sortedVertices.size() - 1;
              }
            }
          }
          faces << facePair;
        } else {
          continue;
        }
        break;
      }
    }
  }

  qDebug("BGE::Storage::Loader::ObjLoader::load(): Parsed %d faces. Resulting in %d vertices, %d uvMappings, %d normals.", faces.size(), sortedVertices.size(), sortedUvs.size(), sortedNormals.size());

  Mesh *mesh = new Mesh(name());
  mesh->addVertices("mesh", sortedVertices);
  mesh->addTextureMaps("mesh", sortedUvs.toVector());
  mesh->addNormals("mesh", sortedNormals.toVector());
  mesh->addFaces("mesh", faces);
  return mesh;
}
