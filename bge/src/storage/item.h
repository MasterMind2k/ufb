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
#ifndef __BGE_STORAGE_ITEM_H
#define __BGE_STORAGE_ITEM_H

#include <QtCore/QString>
#include <QtCore/QHash>

#include "global.h"

namespace BGE {
namespace Storage {

/**
 * An abstract storage item.
 */
class Item
{
  public:
    inline Item(const QString &name)
    {
      m_name = name;
      m_bindId = 0;
      m_refCount = 0;
      m_parent = 0l;
    }

    virtual inline ~Item()
    {
      foreach (Item* item, m_items) {
        item->unload();
        delete item;
      }
    }

    /**
     * Name of the item.
     */
    inline const QString &name() const
    {
      return m_name;
    }

    /**
     * Adds an item.
     *
     * @see item
     * @see removeItem
     */
    inline void addItem(Item *item)
    {
      if (!item)
        return;
      m_items.insert(item->name(), item);
      item->m_parent = this;
    }
    /**
     * Removes an item with a name.
     *
     * @see addItem
     */
    bool removeItem(const QString &name);
    /**
     * Gets an item by name.
     *
     * @see addItem
     * @see removeItem
     */
    inline Item* item(const QString &name) const
    {
      return m_items.value(name, 0l);
    }
    /**
     * Gets all it's items.
     */
    inline QList<Item*> items() const
    {
      return m_items.values();
    }

    /**
     * Returns the item's parent.
     */
    inline Item *parent() const
    {
      return m_parent;
    }

    /**
     * Sets the bind id.
     */
    inline void setBindId(quint32 bindId)
    {
      m_bindId = bindId;
    }
    inline virtual void bind() {}
    inline virtual void unbind() {}
    /**
     * Gets the bind id.
     */
    inline quint32 bindId() const
    {
      return m_bindId;
    }

    inline virtual void unload() {}

    /**
     * Returns the path.
     */
    QString path() const;

  private:
    QString m_name;
    QHash<QString, Item*> m_items;
    Item* m_parent;
    quint32 m_bindId;
    quint32 m_refCount;

    /* Invalid constructor */
    inline Item()
    {
      qFatal("BGE::Item: Invalid constructor invoked!");
    }
};

}
}

#endif
