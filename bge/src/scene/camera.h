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
#ifndef __BGE_SCENE_CAMERA_H
#define __BGE_SCENE_CAMERA_H

#include "scene/object.h"

#include <QtCore/QString>

namespace BGE {
namespace Scene {

/**
 * @short A viewing tool in the game
 *
 * To create it, use the canvas' method createCamera.
 *
 * Let's look at an example:
 * @code
 * // Create our camera
 * BGE::Scene::Camera* camera = Canvas::canvas()->createCamera("test");
 *
 * // And do some transformations to it
 * camera.move(vector);
 * camera.rotateY(angle);
 *
 * // And then activate it (It can be activated before transforms)
 * Canvas::canvas()->activateCamera("test");
 * // You could also use
 * // Canvas::canvas()->activateCamera(camera->name());
 * @endcode
 *
 * @see Canvas::createCamera
 * @see Canvas::activateCamera
 */
class Camera : public Object
{
  public:
    /**
     * Returns the name of the camera.
     */
    inline const QString& name() const
    {
      return m_name;
    }

  private:
    QString m_name;

    static quint32 m_serialNumber;

    // I do not allow creation of cameras outside the engine!
    Camera(const QString& name = QString());
    ~Camera() {}
    Camera();

  friend class BGE::Canvas;
};

}
}

#endif
