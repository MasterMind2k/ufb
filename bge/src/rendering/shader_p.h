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

/* Shader */
GLuint (*glCreateShader)(GLenum type) = 0l;
void (*glShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) = 0l;
void (*glCompileShader)(GLuint shader) = 0l;
void (*glDeleteShader)(GLuint shader) = 0l;
void (*glDetachShader)(GLuint shader) = 0l;

/* Shader program */
GLuint (*glCreateProgram)(void) = 0l;
void (*glAttachShader)(GLuint program, GLuint shader) = 0l;
void (*glLinkProgram)(GLuint program) = 0l;
void (*glUseProgram)(GLuint program) = 0l;

/* Need to add binding functions */

/* Typedefs for casting :D */
typedef GLuint (APIENTRY *_glCreateShader) (GLenum);
typedef void (APIENTRY *_glShaderSource) (GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (APIENTRY *_glCompileShader) (GLuint);
typedef void (APIENTRY *_glDeleteShader) (GLuint);
typedef void (APIENTRY *_glDetachShader) (GLuint);

typedef GLuint (APIENTRY *_glCreateProgram) (void);
typedef void (APIENTRY *_glAtachShader) (GLuint, GLuint);
typedef void (APIENTRY *_glLinkProgram) (GLuint);
typedef void (APIENTRY *_glUseProgram) (GLuint);

GLenum VERTEX_SHADER;
GLenum FRAGMENT_SHADER;

using namespace BGE;

void getShaderFunctions()
{
  // @TODO some tests :D
  Canvas::canvas()->makeCurrent();
  /* Shader */
  glCreateShader = (_glCreateShader) Canvas::canvas()->context()->getProcAddress("glCreateShaderObjectARB");
  glShaderSource = (_glShaderSource) Canvas::canvas()->context()->getProcAddress("glShaderSourceARB");
  glCompileShader = (_glCompileShader) Canvas::canvas()->context()->getProcAddress("glCompileShaderARB");

  /* Shader program */
  glCreateProgram = (_glCreateProgram) Canvas::canvas()->context()->getProcAddress("glCreateProgramObjectARB");
  glAttachShader =  (_glAtachShader) Canvas::canvas()->context()->getProcAddress("glAttachObjectARB");
  glLinkProgram = (_glLinkProgram) Canvas::canvas()->context()->getProcAddress("glLinkProgramARB");
  glUseProgram = (_glUseProgram) Canvas::canvas()->context()->getProcAddress("glUseProgramObjectARB");

  VERTEX_SHADER = GL_VERTEX_SHADER_ARB;
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER_ARB;
}

#endif
