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

class Item
{
  public:
    /**
     * A default constructor.
     */
    inline Item(const QString& name)
    {
      m_name = name;
      m_isDir = false;
      m_bindId = 0;
      m_refCounter = 0;
      m_parent = 0l;
    }

    /**
     * A default destructor. Make sure you call it in your implementation.
     */
    virtual inline ~Item()
    {
      foreach (Item* item, m_items) {
        item->unload();
        delete item;
      }
    }

    /**
     * Self explanatory.
     *
     * @return @c true Item contains sub items
     */
    inline bool isDir() const
    {
      return m_isDir;
    }

    /**
     * Name of the item.
     */
    inline const QString& name() const
    {
      return m_name;
    }

    /**
     * Adds an item.
     *
     * @see item
     * @see removeItem
     */
    inline void addItem(Item* item)
    {
      if (!item)
        return;
      m_items.insert(item->name(), item);
      m_isDir = true;
      item->m_parent = this;
    }
    /**
     * Removes an item with a name.
     *
     * @see addItem
     */
    bool removeItem(const QString& name);
    /**
     * Gets an item by name.
     *
     * @see addItem
     * @see removeItem
     */
    inline Item* item(const QString& name) const
    {
      return m_items.value(name, 0l);
    }

    inline Item* parent() const
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
    /**
     * Gets the bind id.
     */
    inline quint32 bindId() const
    {
      return m_bindId;
    }
    /**
     * Unbinds (sets bind id to 0) the item.
     */
    inline void unbind()
    {
      m_bindId = 0;
    }

    inline virtual void unload() {}

    /**
     * Increments the reference counter.
     */
    inline quint32 incrementReferenceCounter()
    {
      return ++m_refCounter;
    }
    /**
     * Decrements the reference counter.
     */
    inline quint32 decrementReferenceCounter()
    {
      if (!m_refCounter)
        return 0;

      return --m_refCounter;
    }

    /**
     * Returns the path.
     */
    QString path() const;

  private:
    QString m_name;
    bool m_isDir;
    QHash<QString, Item*> m_items;
    Item* m_parent;
    quint32 m_bindId;
    quint32 m_refCounter;

    /* Invalid constructor */
    inline Item()
    {
      qFatal("BGE::Item: Invalid constructor invoked!");
    }
};

}
}

#endif
