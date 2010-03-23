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

#include "storagemanager.h"

#include <QtCore/QDir>
#include <QtCore/QStack>
#include <QtCore/QHash>

#include "storage/loader/loader3ds.h"
#include "storage/loader/textureloader.h"
#include "storage/loader/shaderloader.h"

#include "storage/item.h"
#include "storage/mesh.h"
#include "storage/texture.h"
#include "storage/shader.h"

using namespace BGE;
using namespace BGE::Storage;

StorageManager* StorageManager::m_self = 0l;

StorageManager::StorageManager()
{
  m_root = new Item("/");
}

StorageManager::~StorageManager()
{
  delete m_root;
}

void StorageManager::load()
{
  QStack<QFileInfo> dirs;
  dirs += QDir(":/bge_resources").entryInfoList().toVector();

  QStack<Item*> processedDirs;
  processedDirs.push(m_root);

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
      Loader::AbstractLoader* loader = 0l;
      QString filename = fileInfo.fileName();
      QString absoluteFilePath = fileInfo.absoluteFilePath();

      if (filename.endsWith(".3ds", Qt::CaseInsensitive)) {
        // Load 3ds
        qDebug("BGE::Storage::load(): Loading model '%s'", absoluteFilePath.toUtf8().data());
        loader = new Loader::Loader3DS(absoluteFilePath);
      } else if (filename.endsWith(".png", Qt::CaseInsensitive) || filename.endsWith(".jpg", Qt::CaseInsensitive)) {
        // Load texture
        qDebug("BGE::Storage::load(): Loading texture '%s'", absoluteFilePath.toUtf8().data());
        loader = new Loader::TextureLoader(absoluteFilePath);
      } else if (filename.endsWith(".vsm") || filename.endsWith(".fsm") || filename.endsWith(".sp")) {
        // Load shaders
        qDebug("BGE::Storage::load(): Loading shader '%s'", absoluteFilePath.toUtf8().data());
        loader = new Loader::ShaderLoader(absoluteFilePath);
      }

      if (loader) {
        parent->addItem(loader->load());
        delete loader;
      }
    }

    processedDirs.push(parent);
    if (fileInfo.isDir())
      processedDirs.push(item);
  }
}

Item* StorageManager::get(const QString &path) const
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

void StorageManager::set(Item* item, const QString& path)
{
  if (!path.startsWith("/"))
    return;

  QStringList splited = path.split("/");
  // Remove the dummy elements
  splited.removeFirst();
  if (path.endsWith("/"))
    splited.removeLast();

  Item* parent = m_root;
  Item* node = m_root;
  for (quint16 i = 0; i < splited.size(); i++) {
    QString part = splited.at(i);
    node = node->item(part);

    // The looked for item does not exist
    // let's create it
    if (!node && i < splited.size() - 1) {
      node = new Item(part);
      parent->addItem(node);
    }

    if (i == splited.size() - 1) {
      if (!node) {
        node = new Item(part);
        parent->addItem(node);
      }
      node->addItem(item);
    } else {
      parent = node;
    }
  }
}
