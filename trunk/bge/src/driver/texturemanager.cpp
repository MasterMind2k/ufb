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
#include "texturemanager.h"

#include "canvas.h"

using namespace BGE;
using namespace Driver;

TextureManager *TextureManager::m_self = 0l;

quint8 TextureManager::bind(quint32 textureId)
{
  if (m_mapping.contains(textureId))
    return m_mapping.value(textureId);

  quint8 slot = 0;
  if (!m_availableSlots.isEmpty())
    slot = m_availableSlots.takeFirst();
  else
    slot = m_usedSlots++;

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glActiveTexture(GL_TEXTURE0);

  m_mapping.insert(textureId, slot);
  return slot;
}

void TextureManager::unbind(quint32 textureId)
{
  if (!m_mapping.contains(textureId))
    return;

  quint8 slot = m_mapping.value(textureId);
  if (slot - 1 == m_usedSlots)
    m_usedSlots--;
  else
    m_availableSlots << slot;

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);
  m_mapping.remove(textureId);
}

void TextureManager::unbind()
{
  foreach (quint8 slot, m_mapping.values()) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  glActiveTexture(GL_TEXTURE0);
  m_mapping.clear();
  m_usedSlots = 0;
  m_availableSlots.clear();
}

void TextureManager::copyRenderedToTexture(quint32 textureId)
{
  glActiveTexture(bind(textureId));

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, Canvas::canvas()->size().width(), Canvas::canvas()->size().height(), 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, 0, 0, Canvas::canvas()->size().width(), Canvas::canvas()->size().height(), 0);
  unbind(textureId);
}
