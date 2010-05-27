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
#ifndef __BGE_SCENE_BOUNDINGVOLUME_H
#define __BGE_SCENE_BOUNDINGVOLUME_H

#include "global.h"

namespace BGE {
namespace Scene {
class Object;
class Partition;

/**
 * Bounding volume class. It contains radius and bounding box.
 */
class BoundingVolume
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    /**
     * Creates a 0-sized bounding volume.
     */
    inline BoundingVolume()
    {
      m_corners.reserve(8);
      m_transformedCorners.reserve(8);
      m_radius = 0;
      m_size = Vector3f::Zero();
      m_center = m_size;
      m_transform.setIdentity();
      m_isCacheValid = false;
      calculateCorners();
    }
    /**
     * Creates a bounding volume from defined sphere radius and bounding box
     * with min and max.
     */
    inline BoundingVolume(float radius, const Vector3f &min, const Vector3f &max)
    {
      m_corners.reserve(8);
      m_transformedCorners.reserve(8);
      m_radius = radius;
      m_size = min.cwise().abs() + max;
      m_center = (min + max) / 2.0;
      m_transform.setIdentity();
      m_isCacheValid = false;
      calculateCorners();
    }
    /**
     * Creates a bounding volume from defined center and size.
     */
    inline BoundingVolume(const Vector3f &center, const Vector3f &size)
    {
      m_corners.reserve(8);
      m_transformedCorners.reserve(8);
      m_center = center;
      m_transform.setIdentity();
      setSize(size);
    }

    /**
     * Returns the size of a bounding box in a vector.
     *
     * x - width
     * y - height
     * z - depth
     */
    inline const Vector3f &size() const
    {
      return m_size;
    }

    /**
     * Returns the radius of a bounding sphere.
     */
    inline const float radius() const
    {
      return m_radius;
    }

    /**
     * Returns corners of the bounding box.
     */
    inline const QVector<Vector3f> &corners() const
    {
      return m_corners;
    }
    /**
     * Returns the min corner. It is transformed.
     *
     * @see transformedCorners
     */
    inline const Vector3f &min() const
    {
      return transformedCorners().last();
    }
    /**
     * Returns the max corner. It is transformed.
     *
     * @see transformedCorners
     */
    inline const Vector3f &max() const
    {
      return transformedCorners().first();
    }

    /**
     * Returns the center of the bounding volume.
     *
     * @see transformedCenter
     */
    inline const Vector3f &center() const
    {
      return m_center;
    }

    /**
     * Returns the transformed center.
     *
     * @see center
     */
    inline const Vector3f &transformedCenter() const
    {
      if (!m_isCacheValid) {
        m_transformedCenter = m_transform * m_center;
        calculateTransformedCorners();
      }
      return m_transformedCenter;
    }
    /**
     * Returns the transformed corners of the bounding box.
     *
     * @see corners
     */
    inline const QVector<Vector3f> &transformedCorners() const
    {
      if (!m_isCacheValid) {
        m_transformedCenter = m_transform * m_center;
        calculateTransformedCorners();
      }

      return m_transformedCorners;
    }

    /**
     * Is it bigger of the specified bounding volume?
     */
    inline bool isBigger(const BoundingVolume *boundingVolume) const
    {
      return boundingVolume->m_radius > m_radius;
    }
    /**
     * Is the bounding volume inside this one?
     */
    bool isInside(const BoundingVolume *boundingVolume) const;
    /**
     * Is the specified pointer inside this bounding volume?
     */
    bool isInside(const Vector3f &point) const;

  private:
    float m_radius;
    Vector3f m_size;
    Vector3f m_center;
    Transform3f m_transform;
    mutable Vector3f m_transformedCenter;
    QVector<Vector3f> m_corners;
    mutable QVector<Vector3f> m_transformedCorners;
    mutable bool m_isCacheValid;

    // Calculates corners
    void calculateCorners();
    // Transforms the corners
    void calculateTransformedCorners() const;
    // Calculates the radius
    void calculateRadius();

    // Should be accessed only from BGE::Scene::Object and BGE::Scene::Partition
    inline void setTransform(const Transform3f &transform)
    {
      m_transform = transform;
      m_isCacheValid = false;
    }
    inline void setSize(const Vector3f &size)
    {
      m_size = size;
      m_isCacheValid = false;
      calculateCorners();
      calculateRadius();
    }

    friend class BGE::Scene::Object;
    friend class BGE::Scene::Partition;
};

}
}

#endif
