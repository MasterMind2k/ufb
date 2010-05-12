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
#ifndef FBO_P_H
#define FBO_P_H

#include "canvas.h"

/* Defines for FBO */
#define glGenFramebuffers _glGenFramebuffers
#define glDeleteFramebuffers _glDeleteFramebuffers
#define glBindFramebuffer _glBindFramebuffer
#define glGenRenderbuffers _glGenRenderbuffers
#define glDeleteRenderbuffers _glDeleteRenderbuffers
#define glBindRenderbuffer _glBindRenderbuffer
#define glRenderbufferStorage _glRenderbufferStorage
#define glFramebufferRenderbuffer _glFramebufferRenderbuffer
#define glFramebufferTexture2D _glFramebufferTexture2D
#define glCheckFramebufferStatus _glCheckFramebufferStatus
#define glDrawBuffers _glDrawBuffers

// Added for vsync settings
#define glSwapInterval _glSwapInterval

typedef void (APIENTRY *glGenFramebuffers_t) (GLsizei n, const GLuint *);
typedef void (APIENTRY *glDeleteFramebuffers_t) (GLsizei, const GLuint *);
typedef void (APIENTRY *glBindFramebuffer_t) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY *glGenRenderbuffers_t) (GLsizei, GLuint *);
typedef void (APIENTRY *glDeleteRenderbuffers_t) (GLsizei, const GLuint *);
typedef void (APIENTRY *glBindRenderbuffer_t) (GLenum, GLuint);
typedef void (APIENTRY *glRenderbufferStorage_t) (GLenum, GLenum, GLsizei, GLsizei);
typedef void (APIENTRY *glFramebufferRenderbuffer_t) (GLenum, GLenum, GLenum, GLuint);
typedef void (APIENTRY *glFramebufferTexture2D_t) (GLenum, GLenum, GLenum, GLuint, GLint);
typedef GLenum (APIENTRY *glCheckFramebufferStatus_t) (GLenum);
typedef void (APIENTRY *glDrawBuffers_t) (GLsizei, const GLenum *);
typedef int (APIENTRY *glSwapInterval_t) (GLint enable);

/* FBOs */
glGenFramebuffers_t _glGenFramebuffers = 0l;
glDeleteFramebuffers_t _glDeleteFramebuffers = 0l;
glBindFramebuffer_t _glBindFramebuffer = 0l;
glGenRenderbuffers_t _glGenRenderbuffers = 0l;
glDeleteRenderbuffers_t _glDeleteRenderbuffers = 0l;
glBindRenderbuffer_t _glBindRenderbuffer = 0l;
glRenderbufferStorage_t _glRenderbufferStorage = 0l;
glFramebufferRenderbuffer_t _glFramebufferRenderbuffer = 0l;
glFramebufferTexture2D_t _glFramebufferTexture2D = 0l;
glCheckFramebufferStatus_t _glCheckFramebufferStatus = 0l;
glDrawBuffers_t _glDrawBuffers = 0l;
glSwapInterval_t _glSwapInterval = 0l;

bool hasFBOs = false;

using namespace BGE;

void getFBOFunctions()
{
  Canvas::canvas()->makeCurrent();
  const QGLContext* context = Canvas::canvas()->context();

  _glSwapInterval = (glSwapInterval_t) context->getProcAddress("glXSwapIntervalSGI");
  if (!_glSwapInterval)
    _glSwapInterval = (glSwapInterval_t) context->getProcAddress("wglSwapInterval");

  _glGenFramebuffers = (glGenFramebuffers_t) context->getProcAddress("glGenFramebuffers");
  if (!_glGenFramebuffers)
    return;

  _glDeleteFramebuffers = (glDeleteFramebuffers_t) context->getProcAddress("glDeleteFramebuffers");
  _glBindFramebuffer = (glBindFramebuffer_t) context->getProcAddress("glBindFramebuffer");
  _glGenRenderbuffers = (glGenRenderbuffers_t) context->getProcAddress("glGenRenderbuffers");
  _glDeleteRenderbuffers = (glDeleteRenderbuffers_t) context->getProcAddress("glDeleteRenderbuffers");
  _glBindRenderbuffer = (glBindRenderbuffer_t) context->getProcAddress("glBindRenderbuffer");
  _glRenderbufferStorage = (glRenderbufferStorage_t) context->getProcAddress("glRenderbufferStorage");
  _glFramebufferRenderbuffer = (glFramebufferRenderbuffer_t) context->getProcAddress("glFramebufferRenderbuffer");
  _glFramebufferTexture2D = (glFramebufferTexture2D_t) context->getProcAddress("glFramebufferTexture2D");
  _glCheckFramebufferStatus = (glCheckFramebufferStatus_t) context->getProcAddress("glCheckFramebufferStatus");
  _glDrawBuffers = (glDrawBuffers_t) context->getProcAddress("glDrawBuffers");

  hasFBOs = true;
}

#endif
