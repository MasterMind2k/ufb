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
#include "shader.h"

#include <QtCore/QFile>
#include <QtCore/QRegExp>

#include "storage/shader.h"
#include "storage/shaderprogram.h"

using namespace BGE;
using namespace BGE::Storage::Loader;

QStringList parseSource(QString& source)
{
  QStringList modules;
  QRegExp matcher("\\s*use\\s(/.*);");
  matcher.setMinimal(true);
  int pos = 0;
  int length = 0;
  while ((pos = matcher.indexIn(source, pos + 1)) != -1) {
    modules << matcher.cap(1);
    length += matcher.matchedLength() + 1;
    pos += matcher.matchedLength();
  }

  source = source.mid(length + 1).simplified();

  return modules;
}

Storage::Item* Shader::load()
{
  QFile file(filename());
  file.open(QFile::ReadOnly);
  QString source = QString::fromUtf8(file.readAll());
  file.close();

  Storage::Shader* shader = new Storage::Shader(name());

  QRegExp typeMatcher(".*\\.(.+)$", Qt::CaseInsensitive, QRegExp::RegExp2);
  if (!typeMatcher.exactMatch(filename()))
    return 0l;
  QString extension = typeMatcher.cap(1).toLower();

  if (extension == "vsm") {
    // Load vertex shader
    shader->setShaderSource(source, Storage::Shader::VertexShader);
  } else if (extension == "fsm") {
    // Load fragment shader
    shader->setShaderSource(source, Storage::Shader::FragmentShader);
  } else if (extension == "sp") {
    // Parse and load shader program
    QString vertexSection = "[vertex]";
    QString fragmentSection = "[fragment]";
    int vertexPos = source.indexOf(vertexSection);
    int fragmentPos = source.indexOf(fragmentSection);
    QString vertexSource, fragmentSource;

    if (vertexPos == -1) {
      fragmentSource = source.mid(fragmentPos + fragmentSection.size());
    } else if (fragmentPos == -1) {
      vertexSource = source.mid(vertexPos + vertexSection.size());
    } else if (vertexPos < fragmentPos) {
      vertexSource = source.mid(vertexPos + vertexSection.size(), fragmentPos - (vertexPos + vertexSection.size()));
      fragmentSource = source.mid(fragmentPos + fragmentSection.size());
    } else if (fragmentPos < vertexPos) {
      fragmentSource = source.mid(fragmentPos + fragmentSection.size(), vertexPos - (fragmentPos + fragmentSection.size()));
      vertexSource = source.mid(vertexPos + vertexSection.size());
    }

    Storage::Shader* secondShader = 0l;
    if (!vertexSource.isEmpty() && !fragmentSource.isEmpty()) {
      shader->addDependencies(parseSource(vertexSource));
      shader->setShaderSource(vertexSource, Storage::Shader::VertexShader);
      secondShader = new Storage::Shader(name());
      secondShader->addDependencies(parseSource(fragmentSource));
      secondShader->setShaderSource(fragmentSource, Storage::Shader::FragmentShader);
    } else if (!vertexSource.isEmpty()) {
      shader->addDependencies(parseSource(vertexSource));
      shader->setShaderSource(vertexSource, Storage::Shader::VertexShader);
    } else if (!fragmentSource.isEmpty()) {
      shader->addDependencies(parseSource(fragmentSource));
      shader->setShaderSource(fragmentSource, Storage::Shader::FragmentShader);
    }

    Storage::ShaderProgram* shaderProgram = new Storage::ShaderProgram(name());
    shaderProgram->addShader(shader);
    shaderProgram->addShader(secondShader);

    return shaderProgram;
  }

  return shader;
}
