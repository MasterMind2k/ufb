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

#include "buffermanager.h"

#include "vbomanager.h"

using namespace BGE;
using namespace BGE::Rendering;

BufferManager* BufferManager::init()
{
  VBOManager* output = new VBOManager;
  if (!output->isAvailable())
    qFatal("BGE::Rendering::BufferManager::init(): VBO is not present and there is no fallback!");

  return output;
}
