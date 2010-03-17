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
#include "shadermanager.h"

#include <cstdlib>
#include <cstring>

#include "canvas.h"

#include "scene/object.h"

#include "storage/shader.h"

#include "rendering/shader_p.h"

using namespace BGE;
using namespace Rendering;

ShaderManager::ShaderManager()
{
  getShaderFunctions();
}

void ShaderManager::bindObject(Scene::Object* object)
{
  if (object->shaderProgramId())
    return;

  object->setShaderProgramId(glCreateProgram());

  foreach (Shader* shader, object->shaders()) {
    if (shader->bindId()) {
      glAttachShader(object->shaderProgramId(), shader->bindId());
      continue;
    }

    switch (shader->type()) {
      case Shader::Vertex:
        shader->bind(glCreateShader(VERTEX_SHADER));
        break;
      case Shader::Fragment:
        shader->bind(glCreateShader(FRAGMENT_SHADER));
        break;
      default:
        qWarning("BGE::Rendering::ShaderManager::bindObject(): Shader type not known!");
        break;
    }
    qint32 count;
    qint32* length;
    char** lines = prepareSource(shader->shaderSource(), count, &length);
    glShaderSource(shader->bindId(), count, (const GLchar**) lines, length);
    glCompileShader(shader->bindId());
    glAttachShader(object->shaderProgramId(), shader->bindId());
  }

  glLinkProgram(object->shaderProgramId());
}

void ShaderManager::useShaderProgram(Scene::Object *object)
{
  if (object->shaderProgramId())
    glUseProgram(object->shaderProgramId());
}

char** ShaderManager::prepareSource(const QString &source, qint32 &count, qint32 **length)
{
  QStringList lines = source.split('\n');
  count = lines.count();
  char** output = (char**) malloc(count * sizeof(char*));
  length = (qint32**) malloc(sizeof(qint32));
  length[0] = (qint32*) malloc(count * sizeof(qint32));

  quint32 i = 0;
  foreach (QString line, lines) {
    length[0][i] = line.toUtf8().size();
    output[i] = (char*) malloc(length[0][i] * sizeof(char));
    memcpy(output[i], line.toUtf8().data(), length[0][i++]);
  }

  return output;
}
