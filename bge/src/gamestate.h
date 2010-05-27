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
#ifndef __BGE_GAMESTATE_H
#define __BGE_GAMESTATE_H

#include <QtCore/QHash>

class btDynamicsWorld;

namespace BGE {
class AbstractOverlay;
class AbstractController;
class Canvas;

namespace Scene {
class Object;
class Light;
class Camera;
class Partition;
}

/**
 * @short Defines the state of the game.
 *
 * You should change overlay and controller only in the constructor.
 * Later on, the Canvas gets all the control over them.
 *
 * @warning When the game state is loaded, the parameters may get out of sync!
 *
 * @see BGE::Canvas::pushGameState
 * @see BGE::Canvas::popGameState
 * @see BGE::Canvas::gameState
 */
class GameState
{
  public:
    GameState();

    /**
     * Deletes the scene and partition.
     *
     * @warning Do not delete the gamestate if it is inside Canvas's game stack!
     */
    virtual ~GameState();

  protected:
    inline virtual void load() {}
    inline virtual void unload() {}

    inline void setOverlay(AbstractOverlay *overlay)
    {
      m_overlay = overlay;
    }
    inline void setController(AbstractController *controller)
    {
      m_controller = controller;
    }

  private:
    Scene::Object *m_scene;
    Scene::Partition *m_partition;
    AbstractOverlay *m_overlay;
    AbstractController *m_controller;
    QHash<QString, Scene::Light*> m_lights;
    QHash<QString, Scene::Camera*> m_cameras;
    Scene::Camera *m_activeCamera;
    btDynamicsWorld *m_dynamicsWorld;

    friend class Canvas;
};

}

#endif
