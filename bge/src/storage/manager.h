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

/**
 * A singleton class for managing storage items.
 */
class Manager
{
  public:
    /**
     * Instance "getter" method.
     */
    inline static Manager* self()
    {
      if (!m_self)
        m_self = new Manager;

      return m_self;
    }

    ~Manager();

    /**
     * Loads the data from the :/bge_resources directory.
     *
     * @note Removes all previously loaded resources
     */
    void load();

    /**
     * Gets an item.
     */
    Item* get(const QString &path) const;
    /**
     * @overload
     *
     * It casts the item.
     */
    template <class T>
    inline T get(const QString &path) const
    {
      return static_cast<T> (get(path));
    }

    /**
     * Sets an item.
     *
     * @warning Use this method only when resources are loaded!
     */
    void set(Item *item, const QString &path);

  private:
    static Manager* m_self;
    Item* m_root;

    /* It's a singleton class */
    Manager();
};

}
}

#endif
