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
#ifndef __BGE_CANVAS_H
#define __BGE_CANVAS_H

#include "global.h"

#include <QtCore/QHash>
#include <QtCore/QDir>
#include <QtCore/QQueue>
#include <QtCore/QStack>
#include <QtCore/QMutex>

#include <QtOpenGL/QGLWidget>

class btDynamicsWorld;

namespace BGE {

class AbstractController;
class AbstractOverlay;
class Recorder;
class GameState;
namespace Scene
{
class Object;
class Camera;
class Light;
class Partition;
}
namespace Rendering
{
class Renderer;
class Stage;
}

/**
 * @short The "display" of the engine
 *
 * It has all general methods to control the engine. Through this class
 * you can create and add cameras, add objects to the scene and many more.
 *
 * @see canvas
 */
class Canvas : public QGLWidget
{
  Q_OBJECT
  public:
    static Vector3f SceneSize;
    /**
     * Adds an SceneObject to the scene.
     *
     * @see Scene::SceneObject
     */
    void addSceneObject(Scene::Object *object);
    /**
     * Delayed deletion of a scene object.
     */
    inline void deleteSceneObject(Scene::Object *object)
    {
      m_deletionQueue.enqueue(object);
    }
    /**
     * Returns the root node of the scene.
     */
    inline const Scene::Object *scene() const
    {
      return m_scene;
    }

    /**
     * Returns root spatial octree partition.
     */
    inline Scene::Partition *partition() const
    {
      return m_partition;
    }

    /**
     * Returns the instancce of the canvas.
     */
    static Canvas *canvas();

    /**
     * Sets the input controller. All keyboard and mouse events will
     * be directed to this controller.
     *
     * @see AbstractController
     */
    inline void setController(AbstractController *controller)
    {
      m_controller = controller;
    }

    /**
     * Sets the 2D painted overlay.
     *
     * @see AbstractOverlay
     */
    void setOverlay(AbstractOverlay *overlay);

    /**
     * Returns the status of bounding volumes.
     * Settable only through the command line arguments.
     */
    inline bool drawBoundingVolumes() const
    {
      return m_drawBoundingVolumes;
    }

    /**
     * Creates a camera with a name. The camera is _not_ added to the scene.
     *
     * @note If you do not specify a name, the camera will have a generic
     * name.
     *
     * @see Scene::Camera
     */
    Scene::Camera *createCamera(const QString &name);
    /**
     * Get a camera with a name. If it doesn't exist, then this method will
     * return null.
     *
     * @param name Name of the camera
     *
     * @return @c 0l There is no camera with the specified name
     *
     * @see createCamera
     */
    inline Scene::Camera *camera(const QString &name) const
    {
      return m_cameras.value(name);
    }
    /**
     * Returns the list of all cameras.
     */
    inline QList<Scene::Camera*> cameras() const
    {
      return m_cameras.values();
    }
    /**
     * Activates the specified camera. Returns false if the camera doesn't
     * exist.
     *
     * @return @c false Camera doesn't exist, and there were no changes
     * @return @c true Camera successfuly activated
     *
     * @see createCamera
     */
    bool activateCamera(const QString &name);
    /**
     * Removes a camera. Note that you cannot remove an active camera!
     *
     * @return @c false Camera was not removed, it wasn't found or it is active
     */
    bool removeCamera(const QString &name);
    /**
     * Returns an active camera.
     */
    inline Scene::Camera *activeCamera()
    {
      return m_activeCamera;
    }

    /**
     * Creates a light. It doesn't get inserted to the scene.
     * If you want to create a directional light, you don't
     * have to add it to the scene.
     */
    Scene::Light *createLight(const QString &name);
    /**
     * Removes the light.
     */
    bool removeLight(const QString &name);
    /**
     * Gets the light.
     */
    inline Scene::Light *light(const QString &name) const
    {
      return m_lights.value(name);
    }
    /**
     * Gets the list of all lights.
     */
    inline QList<Scene::Light*> lights() const
    {
      return m_lights.values();
    }

    /**
     * Sets if the FPS gets shown.
     */
    inline void setFPSShown(bool show)
    {
      m_isFPSShown = show;
    }
    /**
     * Returns the status of FPS visibility.
     */
    inline bool isFPSShown() const
    {
      return m_isFPSShown;
    }

    /**
     * Grabs the mouse.
     *
     * Take a look at Qt's documentation.
     */
    inline void grabMouse()
    {
      m_mouseGrabbed = true;
    }
    /**
     * Ungrabs the mouse.
     *
     * Take a look at Qt's documentation.
     */
    inline void ungrabMouse()
    {
      m_mouseGrabbed = false;
    }

    /**
     * Loads the resource. The resource file has to be a Qt's binary resource file.
     *
     * @note This method uses QResource class.
     */
    void loadResource(const QString &fileName = QString());

    /**
     * Replaces the occupiing stage.
     */
    void replaceStage(quint8 index, Rendering::Stage *stage);
    void unregisterStage(quint8 index);
    Rendering::Stage *stage(quint8 index) const;
    /**
     * Inserts the stage at that position.
     * @warning Last stage is rendering stage!
     */
    void registerStage(quint8 index, Rendering::Stage *stage);

    /**
     * Returns the Bullets dynamic world.
     */
    inline btDynamicsWorld *dynamicsWorld() const
    {
      return m_dynamicsWorld;
    }

    /**
     * Returns VSync property.
     */
    inline bool isVSyncEnabled() const
    {
      return m_vsync;
    }
    /**
     * Enables or disables VSync.
     */
    void toggleVSync(bool enable);

    /**
     * Pushes the GameState <i>state</i> to the stack. The Canvas <i>always</i> uses
     * the top game state as it's state!
     */
    void pushGameState(GameState *state);
    /**
     * Pops the top game state and returns it.
     */
    GameState *popGameState();
    /**
     * Returns the top (active) game state.
     *
     * @return @c 0l There is no game state.
     */
    inline GameState *gameState() const
    {
      if (m_states.isEmpty())
        return 0l;
      return m_states.top();
    }

  private:
    /* Really private stuff */
    Scene::Object *m_scene;
    Scene::Partition *m_partition;
    Rendering::Renderer *m_renderer;
    AbstractController *m_controller;
    AbstractOverlay *m_overlay;
    QTime *m_time;
    QTimer *m_timer;
    qint32 m_totalElapsed;
    QQueue<Scene::Object*> m_deletionQueue;
    QQueue<Scene::Object*> m_deferredAdd;
    QMutex m_renderLocker;
    bool m_mouseGrabbed;
    bool m_rendering;

    Recorder *m_recorder;
    quint32 m_timeSinceSnap;

    QHash<QString, Scene::Camera*> m_cameras;
    Scene::Camera* m_activeCamera;

    QHash<QString, Scene::Light*> m_lights;
    QStack<GameState*> m_states;

    bool m_vsync;
    bool m_isFPSShown;
    quint64 m_frames;
    qreal m_fps;

    bool m_drawBoundingVolumes;

    btDynamicsWorld *m_dynamicsWorld;

    static Canvas* m_self;

    // Private methods
    Canvas();
    void unloadState(GameState *state);
    void loadState(GameState *state);
    inline void setDynamicsWorld(btDynamicsWorld *dynamicsWorld)
    {
      m_dynamicsWorld = dynamicsWorld;
    }

    /* Reimplemented methods */
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void checkMouse(const QPoint &pos = QPoint());

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setupDynamicsWorld();

  private slots:
    void cleanup();
    void updateFPS();
};

}

#endif
