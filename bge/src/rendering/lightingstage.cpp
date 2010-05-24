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
#include "lightingstage.h"

#include "storage/manager.h"
#include "storage/shaderprogram.h"

using namespace BGE;
using namespace BGE::Rendering;

void LightingStage::render()
{
  Storage::Manager::self()->get("/shaders/LightingStage")->bind();

  enableLights();
  pass();
  disableLights();

  Storage::Manager::self()->get("/shaders/LightingStage")->unbind();
}
