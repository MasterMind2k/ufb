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
#include "abstractloader.h"

#include <QtCore/QRegExp>

using namespace BGE;
using namespace BGE::Loader;

QString AbstractLoader::name() const
{
  QRegExp nameMatcher("^.+/(.+)\\..+$");
  nameMatcher.setPatternSyntax(QRegExp::RegExp2);
  nameMatcher.exactMatch(m_filename);
  return nameMatcher.cap(1);
}
