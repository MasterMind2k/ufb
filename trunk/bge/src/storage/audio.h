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
#ifndef __BGE_STORAGE_AUDIO_H
#define __BGE_STORAGE_AUDIO_H

#include "storage/item.h"

#include <QtCore/QBuffer>

namespace BGE {
namespace Storage {

class Audio : public BGE::Storage::Item
{
  public:
    Audio(const QString &name) : Item(name)
    {
      m_buffer = new QBuffer;
    }
    inline ~Audio()
    {
      delete m_buffer;
    }

    inline QBuffer *buffer()
    {
      return m_buffer;
    }

  private:
    QBuffer *m_buffer;
};

}
}

#endif
