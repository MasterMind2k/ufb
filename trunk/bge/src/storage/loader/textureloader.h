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
#ifndef __BGE_LOADER_TEXTURELOADER_H
#define __BGE_LOADER_TEXTURELOADER_H

#include <QtCore/QString>

namespace BGE {
class Texture;
namespace Loader {

class TextureLoader
{
  public:
    TextureLoader(const QString& filename);

    inline Texture* texture() const
    {
      return m_texture;
    }

  private:
    Texture* m_texture;
};

}
}

#endif
