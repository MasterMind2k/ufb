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
#ifndef __BGE_LOADER_LOADER3DS_H
#define __BGE_LOADER_LOADER3DS_H

#include "storage/loader/abstractloader.h"

class QFile;

namespace BGE {
namespace Loader {

class Loader3DS : public AbstractLoader
{
  public:
  inline Loader3DS(const QString& filename) : AbstractLoader(filename) {}

  Item* load();

  private:
    QString readString(QFile& file) const;
};

}
}

#endif
