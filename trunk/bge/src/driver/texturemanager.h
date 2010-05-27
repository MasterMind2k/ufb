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
#ifndef __BGE_DRIVER_TEXTUREMANAGER_H
#define __BGE_DRIVER_TEXTUREMANAGER_H

#include <QtCore/QHash>

namespace BGE {
namespace Driver {

/**
 * A helper class for texture managment.
 */
class TextureManager
{
  public:
    static inline TextureManager *self()
    {
      if (!m_self)
        m_self = new TextureManager;

      return m_self;
    }

    /**
     * Binds the texture and returns it's slot.
     *
     * @return Slot of binded texture
     */
    quint8 bind(quint32 textureId);
    /**
     * Unbinds the texture.
     */
    void unbind(quint32 textureId);
    /**
     * Unbinds all textures.
     */
    void unbind();
    /**
     * Returns texture's slot.
     */
    inline qint16 slot(quint32 textureId) const
    {
      return m_mapping.value(textureId, -1);
    }

  private:
    static TextureManager *m_self;
    // Maps texture id and slot
    QHash<quint16, qint8> m_mapping;
    QList<quint8> m_availableSlots;
    quint16 m_usedSlots;

    // A forbidden constructor :D
    TextureManager();
};

}
}

#endif
