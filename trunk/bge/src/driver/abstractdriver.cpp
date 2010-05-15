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
#include "abstractdriver.h"

#include <QtCore/QCoreApplication>

#include <QtOpenGL/QGLFormat>

#include "gl1.h"
#include "gl3.h"

#include "utils_p.h"

using namespace BGE;
using namespace BGE::Driver;

AbstractDriver* AbstractDriver::m_self = 0l;

AbstractDriver* AbstractDriver::self()
{
  getUtilFunctions();
  if (!m_self) {
    bool useGL1 = QCoreApplication::instance()->arguments().contains("GL11");
    if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_0 && !useGL1) {
      m_self = new GL3;
    } else {
      m_self = new GL1;
      qWarning("BGE::Driver::AbstractDriver::self(): Running in compatibility mode (OpenGL 1.1)!");
    }
  }

  return m_self;
}

void AbstractDriver::toggleVSync(bool enable)
{
  if (glSwapInterval) {
    qDebug() << "Setting" << enable;
    glSwapInterval(enable ? 1 : 0);
  }
}
