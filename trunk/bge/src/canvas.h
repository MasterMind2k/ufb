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

#include <QtCore/QHash>
#include <QtCore/QDir>

#include <QtOpenGL/QGLWidget>

namespace BGE {

class AbstractController;
class AbstractOverlay;
class Recorder;
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
    /**
     * Adds an SceneObject to the scene.
     *
     * @see Scene::SceneObject
     */
    void addSceneObject(Scene::Object* object);
    /**
     * Returns the root node of the scene.
     */
    inline const Scene::Object *scene() const
    {
      return m_scene;
    }

    /**
     * Returns root cube.
     */
    inline Scene::Partition *partition() const
    {
      return m_partition;
    }

    /**
     * Returns the instancce of the canvas.
     */
    static Canvas* canvas();

    /**
     * Sets the input controller. All keyboard and mouse events will
     * be directed to this controller.
     *
     * @see AbstractController
     */
    inline void setController(AbstractController* controller)
    {
      m_controller = controller;
    }

    /**
     * Sets the 2D painted overlay.
     *
     * @see AbstractOverlay
     */
    void setOverlay(AbstractOverlay* overlay);

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
    Scene::Camera* createCamera(const QString& name = QString());
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
    inline Scene::Camera* camera(const QString& name) const
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
    bool activateCamera(const QString& name);
    /**
     * Removes a camera. Note that you cannot remove an active camera!
     *
     * @return @c false Camera was not removed, it wasn't found or it is active
     */
    bool removeCamera(const QString& name);
    /**
     * Returns an active camera.
     */
    inline Scene::Camera* activeCamera()
    {
      return m_activeCamera;
    }

    /**
     * Creates a light. It doesn't get inserted to the scene.
     * If you want to create a directional light, you don't
     * have to add it to the scene.
     */
    Scene::Light* createLight(const QString& name = QString());
    /**
     * Removes the light.
     */
    bool removeLight(const QString& name);
    /**
     * Gets the light.
     */
    inline Scene::Light* light(const QString& name) const
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

    inline void setFPSShown(bool show)
    {
      m_isFPSShown = show;
    }
    inline bool isFPSShown() const
    {
      return m_isFPSShown;
    }

    /**
     * Loads the resource. The resource file has to be a Qt's binary resource file.
     *
     * @note This method uses QResource class.
     */
    void loadResource(const QString& fileName = QString());

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

    Recorder *m_recorder;
    quint32 m_timeSinceSnap;

    QHash<QString, Scene::Camera*> m_cameras;
    Scene::Camera* m_activeCamera;

    QHash<QString, Scene::Light*> m_lights;

    bool m_isFPSShown;
    quint64 m_frames;
    qreal m_fps;

    bool m_drawBoundingVolumes;

    static Canvas* m_self;

    Canvas();


    /* Reimplemented methods */
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

  private slots:
    void cleanup();
    void updateFPS();
};

}

#endif
