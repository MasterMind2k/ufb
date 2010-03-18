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

/* Need to add binding functions */

GLenum VERTEX_SHADER;
GLenum FRAGMENT_SHADER;

bool hasShaders = false;

using namespace BGE;

void getShaderFunctions()
{
  Canvas::canvas()->makeCurrent();
  _glCreateShader = (glCreateShader_t) Canvas::canvas()->context()->getProcAddress("glCreateShader");

  if (!_glCreateShader) {
    /* Shader */
    _glCreateShader = (glCreateShader_t) Canvas::canvas()->context()->getProcAddress("glCreateShaderObjectARB");
    if (!_glCreateShader)
      return;

    _glShaderSource = (glShaderSource_t) Canvas::canvas()->context()->getProcAddress("glShaderSourceARB");
    _glCompileShader = (glCompileShader_t) Canvas::canvas()->context()->getProcAddress("glCompileShaderARB");
    _glDeleteShader = (glDeleteShader_t) Canvas::canvas()->context()->getProcAddress("glDeleteObjectARB");

    /* Shader program */
    _glCreateProgram = (glCreateProgram_t) Canvas::canvas()->context()->getProcAddress("glCreateProgramObjectARB");
    _glAttachShader =  (glAttachShader_t) Canvas::canvas()->context()->getProcAddress("glAttachObjectARB");
    _glDetachShader = (glDetachShader_t) Canvas::canvas()->context()->getProcAddress("glDetachObjectARB");
    _glLinkProgram = (glLinkProgram_t) Canvas::canvas()->context()->getProcAddress("glLinkProgramARB");
    _glUseProgram = (glUseProgram_t) Canvas::canvas()->context()->getProcAddress("glUseProgramObjectARB");
    _glDeleteProgram = (glDeleteProgram_t) Canvas::canvas()->context()->getProcAddress("glDeleteObjectARB");

    VERTEX_SHADER = GL_VERTEX_SHADER_ARB;
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER_ARB;
  } else {
    /* Shader */
    _glShaderSource = (glShaderSource_t) Canvas::canvas()->context()->getProcAddress("glShaderSource");
    _glCompileShader = (glCompileShader_t) Canvas::canvas()->context()->getProcAddress("glCompileShader");
    _glDeleteShader = (glDeleteShader_t) Canvas::canvas()->context()->getProcAddress("glDeleteShader");

    /* Shader program */
    _glCreateProgram = (glCreateProgram_t) Canvas::canvas()->context()->getProcAddress("glCreateProgram");
    _glAttachShader =  (glAttachShader_t) Canvas::canvas()->context()->getProcAddress("glAttachShader");
    _glDetachShader = (glDetachShader_t) Canvas::canvas()->context()->getProcAddress("glDetachShader");
    _glLinkProgram = (glLinkProgram_t) Canvas::canvas()->context()->getProcAddress("glLinkProgram");
    _glUseProgram = (glUseProgram_t) Canvas::canvas()->context()->getProcAddress("glUseProgram");
    _glDeleteProgram = (glDeleteProgram_t) Canvas::canvas()->context()->getProcAddress("glDeleteProgram");

    VERTEX_SHADER = GL_VERTEX_SHADER;
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
  }

  hasShaders = true;
}

#endif
