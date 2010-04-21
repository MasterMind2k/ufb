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
#ifndef __BGE_SCENE_LIGHT_H
#define __BGE_SCENE_LIGHT_H

#include "scene/object.h"

#include <QtGui/QColor>

#include "global.h"

namespace BGE {
namespace Scene {

class Light : public BGE::Scene::Object
{
  public:
    Light(const QString& name = QString());

    inline void setAmbientColor(const QColor& ambientColor)
    {
      m_ambient = ambientColor;
    }
    inline const QColor& ambientColor() const
    {
      return m_ambient;
    }
    inline void setDiffuseColor(const QColor& diffuseColor)
    {
      m_diffuse = diffuseColor;
    }
    inline const QColor& diffuseColor() const
    {
      return m_diffuse;
    }
    inline void setSpecularColor(const QColor& specularColor)
    {
      m_specular = specularColor;
    }
    inline const QColor& specularColor() const
    {
      return m_specular;
    }

    inline void setPositional(bool isPositional)
    {
      m_isPositional = isPositional;
    }
    inline bool isPositional() const
    {
      return m_isPositional;
    }

    inline void setConstantAttenuation(float constantAttenuation)
    {
      m_constantAttenuation = constantAttenuation;
    }
    inline float constantAttenuation() const
    {
      return m_constantAttenuation;
    }
    inline void setLinearAttenuation(float linearAttenuation)
    {
      m_linearAttenuation = linearAttenuation;
    }
    inline float linearAttenuation() const
    {
      return m_linearAttenuation;
    }
    inline void setQuadraticAttenuation(float quadraticAttenuation)
    {
      m_quadraticAttenuation = quadraticAttenuation;
    }
    inline float quadraticAttenuation() const
    {
      return m_quadraticAttenuation;
    }

    inline void setSpot(bool isSpot)
    {
      m_isSpot = isSpot;
    }
    inline bool isSpot() const
    {
      return m_isSpot;
    }
    inline void setSpotExponent(float exponent)
    {
      if (exponent < 0)
        exponent = 0;
      else if (exponent > 128)
        exponent = 128;
      m_spotExponent = exponent;
    }
    inline float spotExponent() const
    {
      return m_spotExponent;
    }
    inline void setSpotCutOff(float angle)
    {
      if (angle > 90)
        angle = 90;
      if (angle < 0)
        angle = 0;
      m_spotCutOff = angle;
    }
    inline float spotCutOff() const
    {
      return m_spotCutOff;
    }

    inline static const QColor &globalAmbient()
    {
      return m_globalAmbient;
    }
    inline static void setGlobalAmbient(const QColor &globalAmbient)
    {
      m_globalAmbient = globalAmbient;
    }

  private:
    /* General */
    QString m_name;
    static quint8 m_serial;

    /* Light colors */
    QColor m_ambient;
    QColor m_diffuse;
    QColor m_specular;
    static QColor m_globalAmbient;

    bool m_isPositional;

    /* Attenuation */
    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;

    /* Spot */
    bool m_isSpot;
    float m_spotExponent;
    float m_spotCutOff;
};

}
}

#endif