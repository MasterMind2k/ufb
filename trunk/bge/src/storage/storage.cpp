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

#include "storage.h"

#include <QtCore/QDir>
#include <QtCore/QStack>
#include <QtCore/QHash>

#include "storage/item.h"

#include <QtDebug>

using namespace BGE;

Storage* Storage::m_self = 0l;

Storage::Storage()
{
  m_root = new Item("/");
}

void Storage::load()
{
  QStack<QFileInfo> dirs;
  dirs += QDir(":/bge_resources").entryInfoList().toVector();

  QStack<Item*> processedDirs;
  processedDirs.push(m_root);

  QHash<QString, Item*> dirItems;

  while (!dirs.isEmpty()) {
    QFileInfo fileInfo = dirs.pop();
    Item* parent;
    Item* item;
    do {
      parent = processedDirs.pop();
    } while(parent->name() != fileInfo.dir().dirName() && parent->name() != "/");

    if (fileInfo.isDir()) {
      item = new Item(fileInfo.fileName());
      parent->addItem(item);
      dirs += QDir(fileInfo.absoluteFilePath()).entryInfoList().toVector();
    } else {
      /// @TODO not yet implemented!
      item = new Item(fileInfo.fileName());
      parent->addItem(item);
    }

    processedDirs.push(parent);
    if (fileInfo.isDir())
      processedDirs.push(item);
  }
}
