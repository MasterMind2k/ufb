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
class Canvas;
namespace Scene {

/**
 * A light source. You can create this object only by calling createLight
 * in Canvas.
 *
 * @see Canvas::createLight
 */
class Light : public BGE::Scene::Object
{
  public:
    /**
     * Name of the light.
     */
    inline const QString &name() const
    {
      return m_name;
    }

    /**
     * Sets the ambient color.
     */
    inline void setAmbientColor(const QColor &ambientColor)
    {
      m_ambient = ambientColor;
    }
    /**
     * Gets the ambient color.
     */
    inline const QColor &ambientColor() const
    {
      return m_ambient;
    }
    /**
     * Sets the diffuse color.
     */
    inline void setDiffuseColor(const QColor &diffuseColor)
    {
      m_diffuse = diffuseColor;
    }
    /**
     * Gets the diffuse color.
     */
    inline const QColor &diffuseColor() const
    {
      return m_diffuse;
    }
    /**
     * Sets the specular color.
     */
    inline void setSpecularColor(const QColor &specularColor)
    {
      m_specular = specularColor;
    }
    /**
     * Gets the specular color.
     */
    inline const QColor &specularColor() const
    {
      return m_specular;
    }

    /**
     * Sets if the light is positional or directional.
     */
    inline void setPositional(bool isPositional)
    {
      m_isPositional = isPositional;
    }
    /**
     * Returns if the light is positional or not.
     */
    inline bool isPositional() const
    {
      return m_isPositional;
    }

    /**
     * Sets the constant part of attenuation equation.
     */
    inline void setConstantAttenuation(float constantAttenuation)
    {
      m_constantAttenuation = constantAttenuation;
    }
    /**
     * Returns the constant attenuation.
     */
    inline float constantAttenuation() const
    {
      return m_constantAttenuation;
    }
    /**
     * Sets the linear attenuation factor.
     */
    inline void setLinearAttenuation(float linearAttenuation)
    {
      m_linearAttenuation = linearAttenuation;
    }
    /**
     * Returns the linear attenuation factor.
     */
    inline float linearAttenuation() const
    {
      return m_linearAttenuation;
    }
    /**
     * Sets the quadratic attenuation factor.
     */
    inline void setQuadraticAttenuation(float quadraticAttenuation)
    {
      m_quadraticAttenuation = quadraticAttenuation;
    }
    /**
     * Gets the quadratic attenuation factor.
     */
    inline float quadraticAttenuation() const
    {
      return m_quadraticAttenuation;
    }

    /**
     * Sets the spot property. Spotlight are special case of positional lights.
     */
    inline void setSpot(bool isSpot)
    {
      m_isSpot = isSpot;
    }
    /**
     * Returns if this is a spotlight.
     */
    inline bool isSpot() const
    {
      return m_isSpot;
    }
    /**
     * Sets the amount of light that is distributed.
     */
    inline void setSpotExponent(float exponent)
    {
      if (exponent < 0)
        exponent = 0;
      else if (exponent > 128)
        exponent = 128;
      m_spotExponent = exponent;
    }
    /**
     * Returns the spot distribution.
     */
    inline float spotExponent() const
    {
      return m_spotExponent;
    }
    /**
     * Sets the angle (in degrees) that the spotlight has an effect.
     * Ranges from 0° to 90°.
     *
     * @note This is half the angle.
     */
    inline void setSpotCutOff(float angle)
    {
      if (angle > 90)
        angle = 90;
      if (angle < 0)
        angle = 0;
      m_spotCutOff = angle;
    }
    /**
     * Returns the spotlight cutoff angle.
     */
    inline float spotCutOff() const
    {
      return m_spotCutOff;
    }

    /**
     * Returns the global ambient.
     */
    inline static const QColor &globalAmbient()
    {
      return m_globalAmbient;
    }
    /**
     * Sets the global ambient.
     */
    inline static void setGlobalAmbient(const QColor &globalAmbient)
    {
      m_globalAmbient = globalAmbient;
    }

  private:
    Light(const QString &name = QString());

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

    friend class BGE::Canvas;
};

}
}

#endif
