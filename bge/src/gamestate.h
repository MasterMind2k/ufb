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
 * You should access and change game state's parameters only in the constructor.
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

  protected:
    virtual void load() = 0;
    virtual void unload() = 0;

  private:
    Scene::Object *m_scene;
    Scene::Partition *m_partition;
    AbstractOverlay *m_overlay;
    AbstractController *m_controller;
    QHash<QString, Scene::Light*> m_lights;
    QHash<QString, Scene::Camera*> m_cameras;
    Scene::Camera *m_activeCamera;

    friend class Canvas;
};

}

#endif
