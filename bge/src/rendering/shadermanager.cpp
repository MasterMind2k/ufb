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
#include "storage/shaderprogram.h"

#include "rendering/shader_p.h"

using namespace BGE;
using namespace BGE::Rendering;

ShaderManager::ShaderManager()
{
  getShaderFunctions();
}

void ShaderManager::bindProgram(ShaderProgram* shaderProgram)
{
  if (!shaderProgram || shaderProgram->hasFailed())
    return;

  if (shaderProgram->bindId()) {
    shaderProgram->incrementReferenceCounter();
    return;
  }

  shaderProgram->bind(glCreateProgram());

  foreach (Shader* shader, shaderProgram->shaders()) {
    shader->incrementReferenceCounter();
    if (!shader->bindId()) {
      // Bind and compile shader
      switch (shader->type()) {
        case Shader::VertexShader:
          shader->bind(glCreateShader(VERTEX_SHADER));
          break;
        case Shader::FragmentShader:
          shader->bind(glCreateShader(FRAGMENT_SHADER));
          break;
        default:
          qWarning("BGE::Rendering::ShaderManager::bindObject(): Shader type not known!");
          break;
      }
      // Prepare input data
      qint32 count;
      qint32* length;
      char** lines = prepareSource(shader->shaderSource(), count, &length);

      // Load source
      glShaderSource(shader->bindId(), count, (const GLchar**) lines, length);
      // Compile shader
      glCompileShader(shader->bindId());

      // Check for errors
      int output;
      glGetShaderiv(shader->bindId(), GL_COMPILE_STATUS, &output);
      if (output == GL_FALSE) {
        glGetShaderiv(shader->bindId(), GL_INFO_LOG_LENGTH, &output);
        char *log = (char*) malloc(output * sizeof(char));
        glGetShaderInfoLog(shader->bindId(), output, 0l, log);
        qDebug("BGE::Rendering::ShaderManager::bindProgram(): Compile failed for shader '%s' with info log:\n%s", shader->path().toUtf8().data(), log);
        free(log);

        shaderProgram->setFailed(true);
        unbindProgram(shaderProgram);
        return;
      }
    }

    // Attach shader
    glAttachShader(shaderProgram->bindId(), shader->bindId());
  }

  // Link program
  glLinkProgram(shaderProgram->bindId());

  // Check for errors.
  int output;
  glGetProgramiv(shaderProgram->bindId(), GL_LINK_STATUS, &output);
  if (output == GL_FALSE) {
    glGetProgramiv(shaderProgram->bindId(), GL_INFO_LOG_LENGTH, &output);
    char *log = (char*) malloc(output * sizeof(char));
    glGetProgramInfoLog(shaderProgram->bindId(), output, 0l, log);
    qDebug("BGE::Rendering::ShaderManager::bindProgram(): Link failed for shader program '%s' with info log:\n%s", shaderProgram->path().toUtf8().data(), log);
    free(log);

    // Cleanup
    shaderProgram->setFailed(true);
    unbindProgram(shaderProgram);
  }
}

void ShaderManager::unbindProgram(ShaderProgram *shaderProgram)
{
  if (!shaderProgram || !shaderProgram->bindId())
    return;

  // Unbind all shaders
  if (!shaderProgram->decrementReferenceCounter()) {
    foreach (Shader* shader, shaderProgram->shaders()) {
      // Deattaches shader
      glDetachShader(shaderProgram->bindId(), shader->bindId());
      // Unbinds it
      if (!shader->decrementReferenceCounter()) {
        glDeleteShader(shader->bindId());
        shader->unbind();
      }
    }

    glDeleteProgram(shaderProgram->bindId());
    shaderProgram->unbind();
  }
}

void ShaderManager::useProgram(ShaderProgram* shaderProgram)
{
  if (!shaderProgram)
    return;

  if (shaderProgram->bindId())
    glUseProgram(shaderProgram->bindId());
}

void ShaderManager::unload()
{
  glUseProgram(0);
}

char** ShaderManager::prepareSource(const QString &source, qint32 &count, qint32 **length)
{
  QStringList lines = source.split('\n');
  count = lines.count();
  char** output = (char**) malloc(count * sizeof(char*));
  length[0] = (qint32*) malloc(count * sizeof(qint32));

  quint32 i = 0;
  foreach (QString line, lines) {
    length[0][i] = line.toUtf8().size();
    output[i] = (char*) malloc((length[0][i] + 1) * sizeof(char));
    memcpy(output[i], line.toUtf8().data(), length[0][i] + 1);
    qDebug("%s", output[i]);
    i++;
  }

  return output;
}
