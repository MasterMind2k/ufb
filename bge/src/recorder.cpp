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
#include "recorder.h"

using namespace BGE;

void Recorder::run()
{
  quint64 i = 0;
  m_mutex.lock();
  while (!m_stop || !m_queue.isEmpty()) {
    if (m_queue.isEmpty()) {
      m_mutex.unlock();
      usleep(10);
      m_mutex.lock();
      continue;
    }

    QImage img = m_queue.dequeue();
    m_mutex.unlock();
    img.save(m_outputDir.absoluteFilePath("frame%0.tiff").arg(i++), "tiff");
    m_mutex.lock();
  }
}
