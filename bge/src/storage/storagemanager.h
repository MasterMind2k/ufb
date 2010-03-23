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

#ifndef __BGE_STORAGE_STORAGEMANAGER_H
#define __BGE_STORAGE_STORAGEMANAGER_H

#include <QtCore/QString>

namespace BGE {
namespace Storage {
class Item;

class StorageManager
{
  public:
    /**
     * Instance "getter" method.
     */
    inline static StorageManager* self()
    {
      if (!m_self)
        m_self = new StorageManager;

      return m_self;
    }

    ~StorageManager();

    /**
     * Loads the data from the :/bge_resources directory.
     */
    void load();

    /**
     * Gets an item.
     */
    Item* get(const QString& path) const;
    /**
     * \overload
     */
    template <class T>
    inline T get(const QString& path) const
    {
      return static_cast<T> (get(path));
    }

  private:
    static StorageManager* m_self;
    Item* m_root;

    /* It's a singleton class */
    StorageManager();
};

}
}

#endif