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
#ifndef __BGE_SCENE_SCENEOBJECT_H
#define __BGE_SCENE_SCENEOBJECT_H

#include <QtCore/QList>
#include <QtCore/QMap>

#include "global.h"

#include "storage/material.h"

namespace BGE {
class Canvas;

namespace Storage {
class Mesh;
class Texture;
}

namespace Scene
{
class Partition;
class BoundingVolume;

/**
 *@short Main Scene graph object representation
 *
 * It contains the basic transform commands.
 * It should contain also a texture and material settings for an object.
 */
class Object
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Object();

    virtual ~Object();

    /**
     * @overload
     */
    inline void move(qreal dx, qreal dy, qreal dz)
    {
      move(Vector3f(dx, dy, dz));
    }

    /**
     * Moves the object into the direction.
     *
     * @param direction A vector comprises of dx, dy and dz
     */
    void move(const Vector3f& direction);

    /**
     * Scales the object using the coeficients vector.
     */
    void scale(const Scaling3f& scale);
    /**
     * Sets the new scale
     */
    void setScale(const Scaling3f& scale);
    /**
     * \overload
     */
    inline void setScale(float scale)
    {
      setScale(Scaling3f(scale, scale, scale));
    }
    /**
     * \overload
     */
    inline void scale(float sx, float sy, float sz)
    {
      scale(Scaling3f(sx, sz, sy));
    }
    /**
     * \overload
     */
    inline void scale(float s)
    {
      scale(Scaling3f(s, s, s));
    }
    inline Vector3f scaled() const
    {
      return m_scale * Vector3f(1.0, 1.0, 1.0);
    }

    /**
     * Rotates the object by rotation.
     *
     * @see rotateX
     * @see rotateY
     * @see rotateZ
     */
    void rotate(const AngleAxisf& rotation);
    /**
     * Wrapper method for convenience.
     *
     * @param angle Angle in degress
     *
     * @see rotate
     */
    inline void rotateX(qreal angle)
    {
      AngleAxisf rotation(angle * M_PI / 180, Vector3f::UnitX());
      rotate(rotation);
    }
    /**
     * Wrapper method for convenience.
     *
     * @param angle Angle in degress
     *
     * @see rotate
     */
    inline void rotateY(qreal angle)
    {
      AngleAxisf rotation(angle * M_PI / 180, Vector3f::UnitY());
      rotate(rotation);
    }
    /**
     * Wrapper method for convenience.
     *
     * @param angle Angle in degress
     *
     * @see rotate
     */
    inline void rotateZ(qreal angle)
    {
      AngleAxisf rotation(angle * M_PI / 180, Vector3f::UnitZ());
      rotate(rotation);
    }

    /**
     * Returns the local transformation matrix. Relative to the parent.
     *
     * @see position
     * @see orientation
     */
    inline const Transform3f& transform() const
    {
      return m_transform;
    }
    /**
     * Returns the global transformation matrix.
     *
     * @see globalPosition
     * @see globalOrientation
     */
    inline const Transform3f& globalTransform() const
    {
      return m_globalTransform;
    }

    /**
     * Returns the local position. Relative to the parent.
     *
     * @see globalPosition
     */
    inline const Vector3f& position() const
    {
      return m_position;
    }
    inline void setPosition(float x, float y, float z)
    {
      setPosition(Vector3f(x, y, z));
    }
    inline void setPosition(const Vector3f &pos)
    {
      m_position = pos;
      m_transformModified = true;
    }
    /**
     * Returns the global position.
     *
     * @see position
     */
    inline const Vector3f& globalPosition() const
    {
      return m_globalPosition;
    }

    /**
     * Returns the local orientation. Relative to the parent.
     *
     * @see globalOrientation
     */
    inline const Quaternionf& orientation() const
    {
      return m_orientation;
    }
    /**
     * Sets orientation.
     */
    inline void setOrientation(const Quaternionf& orientation)
    {
      m_orientation = orientation.normalized();
      m_transformModified = true;
    }
    inline void setOrientation(float w, float x, float y, float z)
    {
      setOrientation(Quaternionf(w, x, y, z));
    }
    /**
     * Returns the global orientation.
     *
     * @see orientation
     */
    inline const Quaternionf& globalOrientation() const
    {
      return m_globalOrientation;
    }

    /**
     * Adds a child.
     *
     * @see removeChild
     */
    void addChild(Object* child);
    /**
     * Removes a child.
     *
     * @see addChild
     */
    void removeChild(Object* child);
    /**
     * Returns a child.
     *
     * @see childrenNum
     */
    inline Object* child(int index) const
    {
      return m_children.at(index);
    }
    /**
     * Returns the number of children.
     */
    inline int childrenNum() const
    {
      return m_children.size();
    }
    /**
     * Returns a list of children.
     *
     * @note It returns a read only reference
     */
    inline const QList<Object*>& children() const
    {
      return m_children;
    }

    /**
     * Sets the parent.
     *
     * @see parent
     */
    inline void setParent(Object* parent)
    {
      m_parent = parent;
      if (m_parent) {
        m_globalPosition = m_parent->globalPosition() + m_position;
        m_globalOrientation = (m_parent->globalOrientation() * m_orientation).normalized();
      }
    }
    /**
     * Gets the parent.
     *
     * @see setParent
     */
    inline Object* parent() const
    {
      return m_parent;
    }

    /**
     * True if transformations have to be recalculated.
     */
    inline bool isTransformModified() const
    {
      return m_transformModified;
    }

    /**
     * Sets the mesh.
     */
    void setMesh(Storage::Mesh* mesh);
    /**
     * Gets the mesh.
     */
    inline Storage::Mesh* mesh() const
    {
      return m_mesh;
    }

    /**
     * Adds a material.
     */
    inline void addMaterial(Storage::Material *material)
    {
      m_materials.insert(material->faceName(), material);
    }
    /**
     * Gets the materials.
     */
    inline const QHash<QString, Storage::Material*> &materials() const
    {
      return m_materials;
    }

    /**
     * Sets the texture.
     */
    inline void setTexture(Storage::Texture *texture)
    {
      m_texture = texture;
    }
    /**
     * Gets the texture.
     */
    inline Storage::Texture *texture() const
    {
      return m_texture;
    }

    /**
     * Sets the orientation so the object looks at the center of the
     * specified object.
     */
    void lookAt(Object *object);

    /**
     * Sets the object, to be looked at.
     *
     * @see lookAt
     */
    inline void observe(Object *object)
    {
      m_observed = object;
    }
    /**
     * Gets the observed object.
     */
    inline Object *observed() const
    {
      return m_observed;
    }

    /**
     * Creates a Object from a submesh and add it as a child to this object.
     */
    Object *objectify(const QString& objectName);

    /**
     * Loads all materials that are defined in the model.
     */
    void loadMaterialsFromMesh();

    /**
     * Gets the bounding volume.
     */
    inline const BoundingVolume *boundingVolume() const
    {
      return m_boundingVolume;
    }

    /**
     * Returns if this object is a renderable object.
     */
    inline bool isRenderable() const
    {
      return m_isRenderable;
    }

    /**
     * If you call this method, you wil disable culling of this object. Useful for
     * skyboxes.
     * It also disables lighting.
     */
    inline void disableCulling()
    {
      m_isCulled = false;
      m_ignoreLights = true;
    }
    /**
     * Returns culling status.
     */
    inline bool isCulled() const
    {
      return m_isCulled;
    }

    /**
     * Disables lighting for this object.
     */
    inline void disableLighting()
    {
      m_ignoreLights = true;
    }
    /**
     * Has disabled lighting?
     */
    inline bool hasDisabledLighting() const
    {
      return m_ignoreLights;
    }

    /**
     * Returns the octree partition this object belonfs to.
     */
    inline const Partition *partition() const
    {
      return m_partition;
    }

    /**
     * Name of the object.
     */
    inline const QString &name() const
    {
      return m_name;
    }

    /**
     * Deattaches from parent and adjust it's local position to match global
     * positions.
     *
     * @warning Object is parentless after this operation!
     */
    void deattach();

  protected:
    /**
     * This method gets called _before_ transform matrices get updated. Reimplement
     * it to add your own transform calculations.
     */
    virtual inline void calculateTransforms(qint32 timeDiff)
    {
      Q_UNUSED(timeDiff);
    }

    /**
     * This method gets called _after_ transform matrices get updated.
     */
    virtual inline void postTransformCalculations(qint32 timeDiff)
    {
      Q_UNUSED(timeDiff);
    }

    /**
     * Sets the renderable status of the object.
     */
    inline void setRenderable(bool isRenderable)
    {
      m_isRenderable = isRenderable;
    }

    /**
     * Sets the bounding volume.
     */
    inline void setBoundingVolume(BoundingVolume *boundingVolume)
    {
      m_boundingVolume = boundingVolume;
    }

    /**
     * This method gets called when a collision occurs. For this to happen,
     * you have to set user pointers to the Bullet dynamic bodies.
     *
     * Also note that, both objects (that collide) have to have user pointer
     * defined.
     *
     * Use name to distinguish between objects.
     *
     * @see setName
     * @see name
     */
    virtual inline void collision(Object *object) {}

    /**
     * Sets the name.
     */
    inline void setName(const QString &name)
    {
      m_name = name;
    }

  private:
    Transform3f m_transform;
    Transform3f m_globalTransform;
    Vector3f m_globalPosition;
    Vector3f m_position;
    Scaling3f m_scale;
    Quaternionf m_globalOrientation;
    Quaternionf m_orientation;
    BoundingVolume *m_boundingVolume;
    QList<Object*> m_children;
    Object *m_parent;
    bool m_transformModified;
    bool m_isRenderable;
    bool m_isCulled;
    bool m_ignoreLights;
    QString m_name;

    Storage::Mesh* m_mesh;
    QHash<QString, Storage::Material*> m_materials;
    Storage::Texture* m_texture;

    Object* m_observed;

    Partition *m_partition;

    void prepareTransforms(qint32 timeDiff);
    void setPartition(Partition *partition);

    /* Be very careful in these classes!!! */
    friend class BGE::Canvas;
    friend class BGE::Scene::Partition;
};

}
}

#endif
