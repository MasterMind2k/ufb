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
#ifndef CAMERAS_H
#define CAMERAS_H

#include <QtCore/QObject>

class QTimer;

namespace BGE {
namespace Scene {
class Camera;
}
}

class Cameras : public QObject
{
  Q_OBJECT
  public:
    explicit Cameras();

    inline void addCamera(BGE::Scene::Camera* camera)
    {
      m_cameras << camera;
    }

  private:
    QList<BGE::Scene::Camera*> m_cameras;
    quint32 m_currentCamera;

    QTimer* m_timer;

  private slots:
    void changeCamera();
};

#endif // CAMERAS_H
