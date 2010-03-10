#include "camera.h"

#include <QtCore/QString>

using namespace BGE;
using namespace BGE::Scene;

quint32 Camera::m_serialNumber = 0;

Camera::Camera(const QString& name)
: SceneObject()
{
  if (name.isEmpty())
    m_name = "Camera_" + QString::number(m_serialNumber);
  else
    m_name = name;
}
