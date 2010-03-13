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

#include <QtCore/QStringList>

#include "storage/texture.h"

using namespace BGE;
using namespace Loader;

TextureLoader::TextureLoader(const QString &filename)
{
  QImage image(filename);
  if (image.isNull()) {
    m_texture = 0l;
  } else {
    QString name = filename.split("/").last().split(".").first();
    m_texture = new Texture(name);
    m_texture->setTexture(image);
  }
}
