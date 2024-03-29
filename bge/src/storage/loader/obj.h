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
#ifndef __BGE_STORAGE_LOADER_OBJLOADER_H
#define __BGE_STORAGE_LOADER_OBJLOADER_H

#include "storage/loader/abstractloader.h"

#include "global.h"

namespace BGE {
namespace Storage {
namespace Loader {

/**
 * Obj model loader.
 *
 * It does not load materials.
 *
 * It works properly only when faces are convex. The loader triangulates all
 * faces before loadig.
 */
class Obj : public AbstractLoader
{
  public:
    inline Obj(const QString &filename) : AbstractLoader(filename) {}

    Item *load();

  private:
    QList<Vector3f> triangulate(const QList<Vector3f> &vertices);
};

}
}
}

#endif
