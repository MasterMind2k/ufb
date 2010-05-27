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
#ifndef __BGE_GLOBAL_H
#define __BGE_GLOBAL_H

#include <QtCore/QList>
#include <QtCore/QPair>

#include <Eigen/Core>
#include <Eigen/Geometry>

#ifdef _WIN32
#include "3rdparty/gl.h"
#endif

/* Debugging */
#include <QtDebug>

USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Transform3f;
using Eigen::Transform3d;
using Eigen::Quaternionf;
using Eigen::AngleAxisf;
using Eigen::Scaling3f;
using Eigen::Hyperplane;
using Eigen::Rotation2Df;

typedef Hyperplane<float, 3> Plane;

typedef QList<Vector3f> VectorList;

inline bool operator<=(const Vector3f &v1, const Vector3f &v2)
{
  return v1.x() <= v2.x() && v1.y() <= v2.y() && v1.z() <= v2.z();
}

inline bool operator>=(const Vector3f &v1, const Vector3f &v2)
{
  return v1.x() >= v2.x() && v1.y() >= v2.y() && v1.z() >= v2.z();
}

inline bool operator<(const Vector3f &v1, const Vector3f &v2)
{
  return v1.x() < v2.x() && v1.y() < v2.y() && v1.z() < v2.z();
}

inline bool operator>(const Vector3f &v1, const Vector3f &v2)
{
  return v1.x() > v2.x() && v1.y() > v2.y() && v1.z() > v2.z();
}

inline bool operator==(const Vector3f &v1, const Vector3f &v2)
{
  return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
}

// Some documentation entries
/**
 * @short Main namespace of the engine.
 */
namespace BGE
{

enum Containment {
  FullyInside,
  PartialyInside,
  Outside
};

/**
 * @short All scene graph related classes are inside this namespace.
 */
namespace Scene
{
}

/**
 * @short Namespace containing rendering classes.
 * Classes from this namespace should be used only inside the
 * engine and specific render methods of the BGE::Scene classes.
 */
namespace Rendering
{
}

}

// For easier debugging
inline QDebug operator<<(QDebug dbg, const Vector3f &vector)
{
  dbg.nospace() << "Vector3f(" << vector.x() << ", " << vector.y() << ", " << vector.z() << ")";
  return dbg.space();
}

#endif
