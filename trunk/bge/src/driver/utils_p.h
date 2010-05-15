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
#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtOpenGL/QGLContext>

/* Defines for utility functions */
#define glSwapInterval _glSwapInterval

typedef int (APIENTRY *glSwapInterval_t) (GLint enable);

glSwapInterval_t _glSwapInterval = 0l;

void getUtilFunctions()
{
  const QGLContext* context = QGLContext::currentContext();

  _glSwapInterval = (glSwapInterval_t) context->getProcAddress("glXSwapIntervalSGI");
  if (!_glSwapInterval)
    _glSwapInterval = (glSwapInterval_t) context->getProcAddress("wglSwapInterval");
}

#endif
