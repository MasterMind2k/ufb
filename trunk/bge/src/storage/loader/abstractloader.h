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
#ifndef __BGE_STORAGE_LOADER_ABSTRACTLOADER_H
#define __BGE_STORAGE_LOADER_ABSTRACTLOADER_H

#include <QtCore/QString>

namespace BGE {
namespace Storage {
class Item;
namespace Loader {

/**
 * An abstract loader to subclass, to create your own loader.
 *
 * A game cannot create it's own loader.
 */
class AbstractLoader
{
  public:
    inline AbstractLoader(const QString &filename)
    {
      m_filename = filename;
    }

    /**
     * Loads the data and create a representing item in the storage API.
     */
    virtual Item *load() = 0;

  protected:
    /**
     * Returns the full file name with the path. Can be used as an argument
     * for QFile & friends.
     */
    inline const QString &filename() const
    {
      return m_filename;
    }
    /**
     * Returns the name of the file, without the path.
     */
    QString name() const;

  private:
    QString m_filename;
};

}
}
}

#endif
