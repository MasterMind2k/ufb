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
#include "storage/shaderprogram.h"

#include <QtCore/QQueue>
#include <QtCore/QSet>

#include "driver/abstractdriver.h"

#include "storage/storagemanager.h"
#include "storage/shader.h"

using namespace BGE;
using namespace BGE::Storage;

bool ShaderProgram::prepareProgram()
{
  QQueue<Shader*> queue;
  QSet<Shader*> shaderSet;
  QList<Shader*> shaders;

  queue += m_shaders;
  shaderSet = m_shaders.toSet();

  while (!queue.isEmpty()) {
    Shader* shader = queue.dequeue();

    foreach (QString import, shader->dependencies()) {
      Shader* shaderDependancy = StorageManager::self()->get<Shader*>(import);
      if (!shaderDependancy) {
        qWarning("BGE::ShaderProgram::prepareProgram(): Shader program '%s' cannot be imported. Shader module '%s' is missing.", name().toUtf8().data(), import.toUtf8().data());
        return false;
      }

      if (!shaderSet.contains(shaderDependancy)) {
        shaders << shaderDependancy;
        shaderSet << shaderDependancy;
        queue.enqueue(shaderDependancy);
      }
    }
  }

  m_shaders += shaders;

  return true;
}

void ShaderProgram::bind()
{
  Driver::AbstractDriver::self()->bind(this);
}

void ShaderProgram::unload()
{
  Driver::AbstractDriver::self()->unload(this);
}
