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
#include "light.h"

using namespace BGE;
using namespace BGE::Scene;

quint8 Light::m_serial = 0;
QColor Light::m_globalAmbient = QColor(0.2 * 255, 0.2 * 255, 0.2 * 255);

Light::Light(const QString &name)
 : Object()
{
  if (name.isEmpty())
    m_name = QString("Light_%0").arg(m_serial++);
  else
    m_name = name;

  m_ambient = Qt::black;
  m_diffuse = m_specular = Qt::white;

  m_isPositional = true;

  m_constantAttenuation = 1;
  m_linearAttenuation = m_quadraticAttenuation = 0;

  m_isSpot = false;
  m_spotExponent = 0;
  m_spotCutOff = 45;
}
