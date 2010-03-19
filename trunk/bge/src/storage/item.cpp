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
#include "item.h"

using namespace BGE;

bool Item::removeItem(const QString& name)
{
  if (!m_items.contains(name))
    return false;

  Item* item = m_items.value(name);
  delete item;
  m_items.remove(name);

  return true;
}

QString Item::path() const
{
  QString output = "/" + name();
  for (Item* node = m_parent; node->m_parent; node = node->m_parent)
    output.prepend("/" + node->name());

  return output;
}
