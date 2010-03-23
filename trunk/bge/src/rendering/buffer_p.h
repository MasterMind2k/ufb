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

#ifndef BUFFER_P_H
#define BUFFER_P_H

#include "canvas.h"


/* Defines for Buffer */
#define glGenBuffers _glGenBuffers
#define glBindBuffer _glBindBuffer
#define glBufferData _glBufferData
#define glBufferSubData _glBufferSubData
#define glMapBuffer _glMapBuffer
#define glUnmapBuffer _glUnmapBuffer
#define glGetBufferParameteriv _glGetBufferParameteriv
#define glDeleteBuffers _glDeleteBuffers

typedef void (APIENTRY *glGenBuffers_t) (GLsizei n, GLuint* buffers);
typedef void (APIENTRY *glBindBuffer_t) (GLenum target, GLuint buffer);
typedef void (APIENTRY *glBufferData_t) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void (APIENTRY *glBufferSubData_t) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void* (APIENTRY *glMapBuffer_t) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY *glUnmapBuffer_t) (GLenum target);
typedef void (APIENTRY *glGetBufferParameteriv_t) (GLenum target, GLenum value, GLint* data);
typedef void (APIENTRY *glDeleteBuffers_t) (GLsizei n, GLuint* buffers);

/* Buffers */
glGenBuffers_t _glGenBuffers = 0l;
glBindBuffer_t _glBindBuffer = 0l;
glBufferData_t _glBufferData = 0l;
glBufferSubData_t _glBufferSubData = 0l;
glMapBuffer_t _glMapBuffer = 0l;
glUnmapBuffer_t _glUnmapBuffer = 0l;
glGetBufferParameteriv_t _glGetBufferParameteriv = 0l;
glDeleteBuffers_t _glDeleteBuffers = 0l;

bool hasBuffers = false;

using namespace BGE;

void getBufferFunctions()
{
  Canvas::canvas()->makeCurrent();
  const QGLContext* context = Canvas::canvas()->context();

  _glGenBuffers = (glGenBuffers_t) context->getProcAddress("glGenBuffers");
  if (!_glGenBuffers)
    return;

  _glBindBuffer = (glBindBuffer_t) context->getProcAddress("glBindBuffer");
  _glBufferData = (glBufferData_t) context->getProcAddress("glBufferData");
  _glBufferSubData = (glBufferSubData_t) context->getProcAddress("glBufferSubData");
  _glMapBuffer = (glMapBuffer_t) context->getProcAddress("glMapBuffer");
  _glUnmapBuffer = (glUnmapBuffer_t) context->getProcAddress("glUnmapBuffer");
  _glGetBufferParameteriv = (glGetBufferParameteriv_t) context->getProcAddress("glGetBufferParameteriv");
  _glDeleteBuffers = (glDeleteBuffers_t) context->getProcAddress("glDeleteBuffers");

  hasBuffers = true;
}


#endif
