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
#ifndef __BGE_TEXTURE_H
#define __BGE_TEXTURE_H

#include "storage/item.h"

#include <QtGui/QImage>

namespace BGE
{

class Texture : public BGE::Item
{
  public:
    inline Texture(const QString& name) : Item(name)
    {
      m_bindId = 0;
    }

    /**
     * Bind the texture.
     */
    inline void bind(quint32 bindId)
    {
      m_bindId = bindId;
    }
    /**
     * Gets the texture id.
     */
    inline quint32 bindId() const
    {
      return m_bindId;
    }

    /**
     * Sets a texture image.
     */
    inline void setTexture(const QImage& image)
    {
      m_texture = image;
    }
    /**
     * Gets a texture image.
     */
    inline const QImage& texture() const
    {
      return m_texture;
    }

  private:
    QImage m_texture;

    quint32 m_bindId;
};

}

#endif
