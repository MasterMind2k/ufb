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
#ifndef __BGE_DRIVER_GL3_H
#define __BGE_DRIVER_GL3_H

#include "driver/abstractdriver.h"

#include <QtCore/QHash>
#include <QtCore/QVector>

namespace BGE {
namespace Storage {
class Material;
}
namespace Driver {

class GL3 : public AbstractDriver
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    GL3();

    void bind(Storage::Mesh* mesh);
    void bind(Storage::Texture* texture);
    void bind(const QHash<QString, Storage::Material*> &materials) {}
    void bind(Storage::ShaderProgram* shaderProgram);

    void unbind(Storage::Mesh* mesh);
    void unbind(const QHash<QString, Storage::Material*> &materials) {}
    inline void unbind(Storage::Texture* texture) {/* Ignore */}
    void unbind(Storage::ShaderProgram* shaderProgram);

    void unload(Storage::Mesh* mesh);
    void unload(Storage::Texture* texture);
    void unload(Storage::ShaderProgram* shaderProgram);

    void setLight(Scene::Light* light);
    void resetLighting();

    void setTransformMatrix(const Transform3f& transform);

    void draw(Scene::Object* object);

    void init();
    void clear();
    void setProjection(const QMatrix4x4& transform);

  private:
    struct Plan {
      quint32 primitive;
      quint32 count;
      quint32 offset;
    };
    struct Light {
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      Vector4f position;
      // Colors
      Vector4f ambient;
      Vector4f diffuse;
      Vector4f specular;
      // Attenuations
      float constant;
      float linear;
      float quadratic;
      // Spot properties
      float spot_cutoff;
      float spot_exponent;
      Vector3f spot_direction;
    };
    QHash<quint32, quint32> m_indices;
    QHash<quint32, QList<Plan> > m_plans;
    Matrix4f m_projectionMatrix;
    Transform3f m_transform;
    Matrix3f m_normalMatrix;
    QList<Light> m_lights;

    void load(Storage::Mesh* mesh);
    void load(Storage::ShaderProgram* shaderProgram);

    static void bindAttribute(Storage::ShaderProgram* shaderProgram, QString name, qint32 size, quint32 type, quint32 stride, quint32 offset);
    static void unbindAttribute(Storage::ShaderProgram* shaderProgram, QString name);

    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QVector<float>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Vector2f>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Vector3f>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Vector4f>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Matrix2f>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Matrix3f>& values);
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, const QList<Matrix4f>& values);

    inline static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString &name, float value)
    {
      QVector<float> params;
      params << value;
      bindUniformAttribute(shaderProgram, name, params);
    }
    inline static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString &name, const Vector3f& value)
    {
      QList<Vector3f> params;
      params << value;
      bindUniformAttribute(shaderProgram, name, params);
    }
    inline static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString &name, const Vector4f& value)
    {
      QList<Vector4f> params;
      params << value;
      bindUniformAttribute(shaderProgram, name, params);
    }
    inline static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString &name, const Matrix4f& value)
    {
      QList<Matrix4f> params;
      params << value;
      bindUniformAttribute(shaderProgram, name, params);
    }
    inline static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString &name, const Matrix3f& value)
    {
      QList<Matrix3f> params;
      params << value;
      bindUniformAttribute(shaderProgram, name, params);
    }
    static void bindUniformAttribute(Storage::ShaderProgram* shaderProgram, const QString& name, int value);

    static void setMaterial(Storage::Material *material, Storage::ShaderProgram* shaderProgram);

    static char** prepareShaderSource(const QString &source, qint32 &count, qint32 **length);

    void loadLights(Storage::ShaderProgram *shaderProgram);
};

}
}

#endif