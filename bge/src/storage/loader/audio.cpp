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
#include "audio.h"

#include "QtCore/QFile"

#include "storage/audio.h"

using namespace BGE;
using namespace BGE::Storage::Loader;

Storage::Item *Audio::load()
{
  Storage::Audio *audio = new Storage::Audio(name());

  QFile audioFile(filename());
  if (!audioFile.open(QFile::ReadOnly)) {
    qWarning("BGE::Storage::Loader::Audio::load(): Cannot open audio file '%s'!", filename().toUtf8().data());
    return 0l;
  }

  audio->buffer()->setData(audioFile.readAll());

  audio->buffer()->close();
  audioFile.close();

  return audio;
}
