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

#include <QtCore/QList>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <Eigen/SVD>

USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Transform3f;
using Eigen::Quaternionf;
using Eigen::AngleAxisf;
using Eigen::Scaling3f;

typedef QList<Vector3f> VectorList;

// Some documentation entries
/**
 * @short Main namespace of the engine.
 */
namespace BGE
{

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

#endif
