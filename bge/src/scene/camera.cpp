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
#include "camera.h"

#include <QtCore/QString>

using namespace BGE;
using namespace BGE::Scene;

quint32 Camera::m_serialNumber = 0;

Camera::Camera(const QString& name)
: SceneObject()
{
  if (name.isEmpty())
    m_name = "Camera_" + QString::number(m_serialNumber);
  else
    m_name = name;
}
