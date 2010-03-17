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

ShaderLoader::ShaderLoader(const QString& filename)
{
  m_filename = filename;
  m_shader = 0l;

  parse();
}

void ShaderLoader::parse()
{
  QFile file(m_filename);
  file.open(QFile::ReadOnly);

  qDebug("%s", file.fileName().toUtf8().data());

  m_shader = new Shader(file.fileName());
  m_shader->setShaderSource(QString::fromUtf8(file.readAll()), Shader::Vertex);
  file.close();
}
