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
#ifndef SHADER_P_H
#define SHADER_P_H

#include "canvas.h"

/* Defines for shader functions */
#define glCreateShader _glCreateShader
#define glShaderSource _glShaderSource
#define glCompileShader _glCompileShader
#define glDeleteShader _glDeleteShader
#define glDetachShader _glDetachShader

/* Defines for shader program functions */
#define glCreateProgram _glCreateProgram
#define glAttachShader _glAttachShader
#define glLinkProgram _glLinkProgram
#define glUseProgram _glUseProgram
#define glDeleteProgram _glDeleteProgram

/* Defines for getting info */
#define glGetShaderiv _glGetShaderiv
#define glGetProgramiv _glGetProgramiv
#define glGetShaderInfoLog _glGetShaderInfoLog
#define glGetProgramInfoLog _glGetProgramInfoLog

/* Defines for atrribute binding */
#define glGetAttribLocation _glGetAttribLocation
#define glVertexAttribPointer _glVertexAttribPointer
#define glEnableVertexAttribArray _glEnableVertexAttribArray
#define glDisableVertexAttribArray _glDisableVertexAttribArray

/* Uniform variables */
#define glGetUniformLocation _glGetUniformLocation
#define glUniform1i _glUniform1i
#define glUniform1fv _glUniform1fv
#define glUniform2fv _glUniform2fv
#define glUniform3fv _glUniform3fv
#define glUniform4fv _glUniform4fv
#define glUniformMatrix2fv _glUniformMatrix2fv
#define glUniformMatrix3fv _glUniformMatrix3fv
#define glUniformMatrix4fv _glUniformMatrix4fv

typedef GLuint (APIENTRY *glCreateShader_t) (GLenum type);
typedef void (APIENTRY *glShaderSource_t) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* length);
typedef void (APIENTRY *glCompileShader_t) (GLuint shader);
typedef void (APIENTRY *glDeleteShader_t) (GLuint shader);

typedef GLuint (APIENTRY *glCreateProgram_t) (void);
typedef void (APIENTRY *glAttachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glDetachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glLinkProgram_t) (GLuint program);
typedef void (APIENTRY *glUseProgram_t) (GLuint program);
typedef void (APIENTRY *glDeleteProgram_t) (GLuint program);

typedef void (APIENTRY *glGetShaderiv_t) (GLuint shader, GLenum type, int* param);
typedef void (APIENTRY *glGetProgramiv_t) (GLuint program, GLenum type, int* param);
typedef void (APIENTRY *glGetShaderInfoLog_t) (GLuint shader, int maxLen, int* len, char* log);
typedef void (APIENTRY *glGetProgramInfoLog_t) (GLuint program, int maxLen, int* len, char* log);

typedef GLint (APIENTRY *glGetAttribLocation_t) (GLuint program, const GLchar* name);
typedef void (APIENTRY *glVertexAttribPointer_t) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRY *glEnableVertexAttribArray_t) (GLuint index);
typedef void (APIENTRY *glDisableVertexAttribArray_t) (GLuint index);

typedef GLint (APIENTRY *glGetUniformLocation_t) (GLuint program, const char* name);
typedef void (APIENTRY *glUniform1i_t) (GLint location, GLint value);
typedef void (APIENTRY *glUniform1fv_t) (GLint location, GLsizei count, GLfloat *value);
typedef void (APIENTRY *glUniform2fv_t) (GLint location, GLsizei count, GLfloat *value);
typedef void (APIENTRY *glUniform3fv_t) (GLint location, GLsizei count, GLfloat *value);
typedef void (APIENTRY *glUniform4fv_t) (GLint location, GLsizei count, GLfloat *value);
typedef void (APIENTRY *glUniformMatrix2fv_t) (GLint location, GLsizei count, GLboolean transpose, GLfloat *value);
typedef void (APIENTRY *glUniformMatrix3fv_t) (GLint location, GLsizei count, GLboolean transpose, GLfloat *value);
typedef void (APIENTRY *glUniformMatrix4fv_t) (GLint location, GLsizei count, GLboolean transpose, GLfloat *value);

/* Shader */
glCreateShader_t _glCreateShader = 0l;
glShaderSource_t _glShaderSource = 0l;
glCompileShader_t _glCompileShader = 0l;
glDeleteShader_t _glDeleteShader = 0l;

/* Shader program */
glCreateProgram_t _glCreateProgram = 0l;
glAttachShader_t _glAttachShader = 0l;
glDetachShader_t _glDetachShader = 0l;
glLinkProgram_t _glLinkProgram = 0l;
glUseProgram_t _glUseProgram = 0l;
glDeleteProgram_t _glDeleteProgram = 0l;

/* Getter functions */
glGetShaderiv_t _glGetShaderiv = 0l;
glGetProgramiv_t _glGetProgramiv = 0l;
glGetShaderInfoLog_t _glGetShaderInfoLog = 0l;
glGetProgramInfoLog_t _glGetProgramInfoLog = 0l;

/* Binding functions */
glGetAttribLocation_t _glGetAttribLocation = 0l;
glVertexAttribPointer_t _glVertexAttribPointer = 0l;
glEnableVertexAttribArray_t _glEnableVertexAttribArray = 0l;
glDisableVertexAttribArray_t _glDisableVertexAttribArray = 0l;

/* Uniform binding functions */
glGetUniformLocation_t _glGetUniformLocation = 0l;
glUniform1i_t _glUniform1i = 0l;
glUniform1fv_t _glUniform1fv = 0l;
glUniform2fv_t _glUniform2fv = 0l;
glUniform3fv_t _glUniform3fv = 0l;
glUniform4fv_t _glUniform4fv = 0l;
glUniformMatrix2fv_t _glUniformMatrix2fv = 0l;
glUniformMatrix3fv_t _glUniformMatrix3fv = 0l;
glUniformMatrix4fv_t _glUniformMatrix4fv = 0l;

GLenum VERTEX_SHADER;
GLenum FRAGMENT_SHADER;

bool hasShaders = false;

using namespace BGE;

void getShaderFunctions()
{
  Canvas::canvas()->makeCurrent();
  const QGLContext* context = Canvas::canvas()->context();

  /* Shader */
  _glCreateShader = (glCreateShader_t) context->getProcAddress("glCreateShader");
  if (!_glCreateShader)
    return;
  _glShaderSource = (glShaderSource_t) context->getProcAddress("glShaderSource");
  _glCompileShader = (glCompileShader_t) context->getProcAddress("glCompileShader");
  _glDeleteShader = (glDeleteShader_t) context->getProcAddress("glDeleteShader");

  /* Shader program */
  _glCreateProgram = (glCreateProgram_t) context->getProcAddress("glCreateProgram");
  _glAttachShader =  (glAttachShader_t) context->getProcAddress("glAttachShader");
  _glDetachShader = (glDetachShader_t) context->getProcAddress("glDetachShader");
  _glLinkProgram = (glLinkProgram_t) context->getProcAddress("glLinkProgram");
  _glUseProgram = (glUseProgram_t) context->getProcAddress("glUseProgram");
  _glDeleteProgram = (glDeleteProgram_t) context->getProcAddress("glDeleteProgram");

  /* Getters */
  _glGetShaderiv = (glGetShaderiv_t) context->getProcAddress("glGetShaderiv");
  _glGetProgramiv = (glGetProgramiv_t) context->getProcAddress("glGetProgramiv");
  _glGetShaderInfoLog = (glGetShaderInfoLog_t) context->getProcAddress("glGetShaderInfoLog");
  _glGetProgramInfoLog = (glGetProgramInfoLog_t) context->getProcAddress("glGetProgramInfoLog");

  /* Binders */
  _glGetAttribLocation = (glGetAttribLocation_t) context->getProcAddress("glGetAttribLocation");
  _glVertexAttribPointer = (glVertexAttribPointer_t) context->getProcAddress("glVertexAttribPointer");
  _glEnableVertexAttribArray = (glEnableVertexAttribArray_t) context->getProcAddress("glEnableVertexAttribArray");
  _glDisableVertexAttribArray = (glDisableVertexAttribArray_t) context->getProcAddress("glDisableVertexAttribArray");

  /* Uniforms */
  _glGetUniformLocation = (glGetUniformLocation_t) context->getProcAddress("glGetUniformLocation");
  _glUniform1i = (glUniform1i_t) context->getProcAddress("glUniform1i");
  _glUniform1fv = (glUniform1fv_t) context->getProcAddress("glUniform1fv");
  _glUniform2fv = (glUniform2fv_t) context->getProcAddress("glUniform2fv");
  _glUniform3fv = (glUniform3fv_t) context->getProcAddress("glUniform3fv");
  _glUniform4fv = (glUniform4fv_t) context->getProcAddress("glUniform4fv");
  _glUniformMatrix2fv = (glUniformMatrix2fv_t) context->getProcAddress("glUniformMatrix2fv");
  _glUniformMatrix3fv = (glUniformMatrix3fv_t) context->getProcAddress("glUniformMatrix3fv");
  _glUniformMatrix4fv = (glUniformMatrix4fv_t) context->getProcAddress("glUniformMatrix4fv");

  VERTEX_SHADER = GL_VERTEX_SHADER;
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER;

  hasShaders = true;
}

#endif
