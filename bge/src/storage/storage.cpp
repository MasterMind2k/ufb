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

#include "storage/loader/loader3ds.h"
#include "storage/loader/textureloader.h"

#include "storage/item.h"
#include "storage/mesh.h"
#include "storage/texture.h"

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
      if (fileInfo.fileName().endsWith(".3ds", Qt::CaseInsensitive)) {
        // Load 3ds
        qDebug("BGE::Storage::load(): Loading file '%s'", fileInfo.absoluteFilePath().toUtf8().data());
        Loader::Loader3DS *loader = new Loader::Loader3DS(fileInfo.absoluteFilePath());
        parent->addItem(loader->mesh());
        delete loader;
      } else if (fileInfo.fileName().endsWith(".png", Qt::CaseInsensitive) || fileInfo.fileName().endsWith(".jpg", Qt::CaseInsensitive)) {
        // Load texture
        qDebug("BGE::Storage::load(): Loading file '%s'", fileInfo.absoluteFilePath().toUtf8().data());
        Loader::TextureLoader *loader = new Loader::TextureLoader(fileInfo.absoluteFilePath());
        parent->addItem(loader->texture());
        delete loader;
      }
    }

    processedDirs.push(parent);
    if (fileInfo.isDir())
      processedDirs.push(item);
  }
}

Item* Storage::get(const QString &path) const
{
  if (!path.startsWith("/") || path.endsWith("/"))
    return 0l;

  QStringList splited = path.split("/");
  // Remove the dummy element
  splited.removeAt(0);

  Item* node = m_root;
  for (quint16 i = 0; i < splited.size(); i++) {
    QString part = splited.at(i);
    node = node->item(part);
    // The looked for item does not exist
    if (!node)
      return 0l;
    if (node->isDir() && i == splited.size() - 1)
      return 0l;

    if (!node->isDir() && i == splited.size() - 1)
      return node;
  }
}
