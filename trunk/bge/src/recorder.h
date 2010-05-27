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
#ifndef __BGE_RECORDER_H
#define __BGE_RECORDER_H

#include <QtCore/QThread>
#include <QtCore/QDir>
#include <QtCore/QQueue>
#include <QtCore/QMutex>

#include <QtGui/QImage>

namespace BGE {

/**
 * A recorder class. It saves images from framebuffer to designated path.
 */
class Recorder : public QThread
{
  Q_OBJECT
  public:
    explicit Recorder(QObject *parent = 0)
    {
      m_stop = false;
    }

    inline void enqueueImage(const QImage &image)
    {
      m_mutex.lock();
      m_queue.enqueue(image);
      m_mutex.unlock();
    }
    /**
     * Stops the thread loop
     */
    inline void stop()
    {
      m_stop = true;
    }
    /**
     * Sets the save output.
     */
    inline void setOutput(const QDir &output)
    {
      m_outputDir = output;
    }

  private:
    QDir m_outputDir;
    QQueue<QImage> m_queue;
    bool m_stop;
    QMutex m_mutex;

    void run();

};

}

#endif
