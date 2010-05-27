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
#ifndef __BGE_STORAGE_LOADER_AUDIO_H
#define __BGE_STORAGE_LOADER_AUDIO_H

#include "storage/loader/abstractloader.h"

namespace BGE {
namespace Storage {
namespace Loader {

/**
 * An audio loader.
 *
 * @note Engine currently does not support audio playback.
 */
class Audio : public BGE::Storage::Loader::AbstractLoader
{
  public:
    inline Audio(const QString &fileName) : AbstractLoader(fileName) {}

    Item *load();
};

}
}
}

#endif
