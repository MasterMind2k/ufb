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
#ifndef __BGE_STORAGE_MATERIAL_H
#define __BGE_STORAGE_MATERIAL_H

#include "storage/item.h"

#include <QtGui/QColor>

namespace BGE {
namespace Storage {

class Material : public Item
{
  public:
  inline Material(const QString& name) : Item(name)
    {
      /* Default values */
      m_ambient = QColor(0.2 * 255, 0.2 * 255, 0.2 * 255, 255);
      m_diffuse = QColor(0.8 * 255, 0.8 * 255, 0.8 * 255, 255);
      m_specular = m_emission = QColor(0, 0, 0, 0);
      m_shininess = 0;
      m_faceName = name;
    }

  inline Material(const QString& name, const QColor& ambient, const QColor& diffuse, const QColor& specular, const QColor& emission, float shininess) : Item(name)
    {
      m_ambient = ambient;
      m_diffuse = diffuse;
      m_specular = specular;
      m_emission = emission;
      m_shininess = shininess;
      m_faceName = name;
    }

    inline void setAmbient(const QColor& ambient)
    {
      m_ambient = ambient;
    }
    inline const QColor& ambient() const
    {
      return m_ambient;
    }
    inline const QColor& setDiffuse(const QColor& diffuse)
    {
      m_diffuse = diffuse;
    }
    inline const QColor& diffuse() const
    {
      return m_diffuse;
    }
    inline void setSpecular(const QColor& specular)
    {
      m_specular = specular;
    }
    inline const QColor& specular() const
    {
      return m_specular;
    }
    inline void setEmission(const QColor& emission)
    {
      m_emission = emission;
    }
    inline const QColor& emission() const
    {
      return m_emission;
    }
    inline void setShininess(qint32 shininess)
    {
      m_shininess = shininess;
    }
    inline qint32 shininess() const
    {
      return m_shininess;
    }

    inline void setFaceName(const QString &name)
    {
      m_faceName = name;
    }
    inline const QString &faceName() const
    {
      return m_faceName;
    }

  private:
    QColor m_ambient;
    QColor m_diffuse;
    QColor m_specular;
    QColor m_emission;
    qint32 m_shininess;
    QString m_faceName;
};

}
}

#endif
