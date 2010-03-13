/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
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
#ifndef __BGE_LOADER_MODELLOADER_H
#define __BGE_LOADER_MODELLOADER_H

#include <QtCore/QString>

namespace BGE
{
class Mesh;
namespace Loader
{

class ModelLoader
{
  public:
    inline ModelLoader(const QString& filename)
    {
      m_filename = filename;
      m_mesh = 0l;
    }

    inline BGE::Mesh* mesh() const
    {
      return m_mesh;
    }

  protected:
    virtual void parse() = 0;

    QString m_filename;
    Mesh* m_mesh;
};

}

}

#endif
