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
#include "shaderloader.h"

#include <QtCore/QFile>

#include "storage/shader.h"

using namespace BGE;
using namespace BGE::Loader;

Item* ShaderLoader::load()
{
  QFile file(filename());
  file.open(QFile::ReadOnly);
  QString source = QString::fromUtf8(file.readAll());
  file.close();

  Shader* shader = new Shader(name());

  if (filename().endsWith(".vsp", Qt::CaseInsensitive)) {
    // Load vertex shader program
    shader->setShaderSource(source, Shader::Vertex);
  } else if (filename().endsWith(".fsp", Qt::CaseInsensitive)) {
    // Load vertex shader program
    shader->setShaderSource(source, Shader::Fragment);
  }
  // @TODO loading shader modules / creating shader programs...

  return shader;
}
