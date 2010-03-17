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
#include "textureloader.h"

#include "storage/texture.h"

using namespace BGE;
using namespace BGE::Loader;

Item* TextureLoader::load()
{
  QImage image(filename());
  Texture* texture = 0l;
  if (!image.isNull()) {
    texture = new Texture(name());
    texture->setTexture(image);
  }

  return texture;
}
