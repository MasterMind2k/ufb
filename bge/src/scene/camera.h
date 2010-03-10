#ifndef __BGE_SCENE_CAMERA_H
#define __BGE_SCENE_CAMERA_H

#include "scene/sceneobject.h"

#include <QtCore/QString>

namespace BGE
{
namespace Scene
{

class Camera : public BGE::Scene::SceneObject
{
  public:
    inline QString name() const
    {
      return m_name;
    }

    /// @TODO lookAt method

  private:
    QString m_name;

    static quint32 m_serialNumber;

    Camera(const QString& name = QString());
    ~Camera() {}

  friend class BGE::Canvas;
};

}
}

#endif
