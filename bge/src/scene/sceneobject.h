/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
 *   Copyright (C) 2010 by Matevž Pesek    <be inserted>                   *
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
#ifndef __BGE_SCENE_SCENEOBJECT_H
#define __BGE_SCENE_SCENEOBJECT_H

#include <QtCore/QList>

#include "global.h"

namespace BGE
{
class Canvas;

namespace Rendering
{
class Renderer;
}

namespace Scene
{

class SceneObject
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    SceneObject();

    void move(Vector3f direction);
    void rotateX(qreal angle);
    void rotateY(qreal angle);
    void rotateZ(qreal angle);

    Transform3f transform() const;
    inline Transform3f globalTransform() const
    {
      return m_globalTransform;
    }
    Vector3f globalPosition() const;
    Vector3f position() const;
    Quaternionf globalOrientation() const;
    Quaternionf orientation() const;

    inline void addChild(SceneObject* child)
    {
      m_childs << child;
      child->setParent(this);
    }
    inline void removeChild(SceneObject* child)
    {
      m_childs.removeOne(child);
    }
    inline SceneObject* child(int index) const
    {
      return m_childs.at(index);
    }
    inline void setParent(SceneObject* parent)
    {
      m_parent = parent;
    }
    inline SceneObject* parent() const
    {
      return m_parent;
    }

    inline bool isTransformModified() const
    {
      return m_transformModified;
    }

  protected:
    virtual inline void render(Rendering::Renderer* renderer) {}
    virtual inline void calculateTransforms() {}

  private:
    void prepareTransforms(Rendering::Renderer* renderer);

    Transform3f m_transform;
    Transform3f m_globalTransform;
    Vector3f m_globalPosition;
    Vector3f m_position;
    Quaternionf m_globalOrientation;
    Quaternionf m_orientation;
    QList<SceneObject*> m_childs;
    SceneObject* m_parent;
    bool m_transformModified;

    /* Be very careful in these classes!!! */
    friend class BGE::Canvas;
    friend class BGE::Rendering::Renderer;
};

}
}

#endif
