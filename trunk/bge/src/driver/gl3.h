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
#include <QtCore/QStringList>

class FBO;

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
    ~GL3();

    void bindFBO();
    void unbindFBO();
    void bind(Storage::Mesh* mesh);
    void bind(Storage::Texture* texture);
    void bind(const QHash<QString, Storage::Material*> &materials);
    void bind(Storage::ShaderProgram* shaderProgram);

    void unbind(Storage::Mesh* mesh);
    inline void unbind(const QHash<QString, Storage::Material*> &materials)
    {
      Q_UNUSED(materials);
      m_materials.clear();
    }
    inline void unbind(Storage::Texture* texture);
    void unbind(Storage::ShaderProgram* shaderProgram);

    void unload(Storage::Mesh* mesh);
    void unload(Storage::Texture* texture);
    void unload(Storage::ShaderProgram* shaderProgram);

    void setLight(Scene::Light* light);
    void resetLighting();

    void setTransformMatrix(const Transform3f& transform);

    void draw();

    void init();
    void clear();
    void setProjection(const Transform3f &projection);

    void shading();

    void registerStage(quint8 index, Rendering::Stage *stage);
    inline void unregisterStage(quint8 index)
    {
      m_stages.removeAt(index);
    }
    void replaceStage(quint8 index, Rendering::Stage *stage);
    inline Rendering::Stage *stage(quint8 index) const
    {
      return m_stages.at(index);
    }

    void pass(Rendering::Stage *stage);

  private:
    struct Plan {
      quint32 primitive;
      quint32 count;
      QString materialName;
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
    QHash<QString, Storage::Material*> m_materials;
    static const quint8 m_maxLights = 4;
    Storage::ShaderProgram *m_boundShader;
    Storage::Mesh *m_boundMesh;
    Storage::Material *m_boundingMaterial;
    quint32 m_renderedLights;
    FBO *m_fbo;
    FBO *m_renderer;
    quint32 m_quad;
    quint32 m_quadIdxs;
    QList<Rendering::Stage*> m_stages;
    bool m_shading;
    bool m_firstPass;
    quint32 m_renderTexture;
    QStringList m_globalUniforms;

    void load(Storage::Mesh* mesh);
    void load(Storage::ShaderProgram* shaderProgram);

  public:

    void bindAttribute(QString name, qint32 size, quint32 type, quint32 stride, quint32 offset);
    void unbindAttribute(QString name);

    void bindUniformAttribute(const QString& name, const QVector<float>& values);
    void bindUniformAttribute(const QString& name, const QList<Vector2f>& values);
    void bindUniformAttribute(const QString& name, const QList<Vector3f>& values);
    void bindUniformAttribute(const QString& name, const QList<Vector4f>& values);
    void bindUniformAttribute(const QString& name, const QList<Matrix2f>& values);
    void bindUniformAttribute(const QString& name, const QList<Matrix3f>& values);
    void bindUniformAttribute(const QString& name, const QList<Matrix4f>& values);

    inline void bindUniformAttribute(const QString &name, float value)
    {
      QVector<float> params;
      params << value;
      bindUniformAttribute(name, params);
    }
    inline void bindUniformAttribute(const QString &name, const Vector3f& value)
    {
      QList<Vector3f> params;
      params << value;
      bindUniformAttribute(name, params);
    }
    inline void bindUniformAttribute(const QString &name, const Vector4f& value)
    {
      QList<Vector4f> params;
      params << value;
      bindUniformAttribute(name, params);
    }
    inline void bindUniformAttribute(const QString &name, const Matrix4f& value)
    {
      QList<Matrix4f> params;
      params << value;
      bindUniformAttribute(name, params);
    }
    inline void bindUniformAttribute(const QString &name, const Matrix3f& value)
    {
      QList<Matrix3f> params;
      params << value;
      bindUniformAttribute(name, params);
    }
    void bindUniformAttribute(const QString& name, int value);

  private:

    void setMaterial(Storage::Material *material);

    static char** prepareShaderSource(const QString &source, qint32 &count, qint32 **length);

    void loadLights();
    void initFBO();
};

}
}

#endif
