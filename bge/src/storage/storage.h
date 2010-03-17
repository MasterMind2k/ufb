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

#ifndef __BGE_STORAGE_H
#define __BGE_STORAGE_H

#include <QtCore/QString>

namespace BGE {
class Item;

class Storage
{
  public:
    /**
     * Instance "getter" method.
     */
    inline static Storage* self()
    {
      if (!m_self)
        m_self = new Storage;

      return m_self;
    }

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
    static Storage* m_self;
    Item* m_root;

    /* It's a singleton class */
    Storage();
};

}

#endif
