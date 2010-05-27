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
#ifndef __BGE_DRIVER_ABSTRACTDRIVER_H
#define __BGE_DRIVER_ABSTRACTDRIVER_H

#include "global.h"

class QString;
class QMatrix4x4;

namespace BGE {

namespace Scene {
class Object;
class Light;
class ParticleEmitter;
}
namespace Rendering {
class Stage;
}

namespace Storage {
class Mesh;
class Material;
class Texture;
class Shader;
class ShaderProgram;
}

namespace Driver {

/**
 * Abstract driver.
 *
 * Implementations are GL1 and GL3.
 */
class AbstractDriver
{
  public:
    /**
     * It determens and selects the proper driver.
     *
     * Currently supported drivers are for OpenGL 3 and OpenGL 1.
     */
    static AbstractDriver* self();

    /**
     * Binds the FBO.
     */
    inline virtual void bindFBO() {}
    /**
     * Unbinds the FBO.
     */
    inline virtual void unbindFBO() {}
    /**
     * Binds a mesh.
     */
    virtual void bind(Storage::Mesh* mesh) = 0;
    /**
     * Binds materials. Key of the hash is the material name.
     * The material's name has to match object's material name, to be used.
     */
    virtual void bind(const QHash<QString, Storage::Material*> &materials) = 0;
    /**
     * Binds a texture.
     */
    virtual void bind(Storage::Texture* texture) = 0;
    /**
     * Binds a shader program.
     */
    virtual void bind(Storage::ShaderProgram* shaderProgram) = 0;

    /**
     * Unbinds a mesh.
     */
    virtual void unbind(Storage::Mesh* mesh) = 0;
    /**
     * Unbinds materials.
     */
    virtual void unbind(const QHash<QString, Storage::Material*> &materials) = 0;
    /**
     * Unbinds a texture.
     */
    virtual void unbind(Storage::Texture* texture) = 0;
    /**
     * Unbinds a shader program.
     */
    virtual void unbind(Storage::ShaderProgram* shaderProgram) = 0;

    /**
     * Unloads the mesh (removes it from the graphics card).
     */
    virtual void unload(Storage::Mesh* mesh) = 0;
    /**
     * Unloads the texture.
     */
    virtual void unload(Storage::Texture* texture) = 0;
    /**
     * Unloads the shader program.
     */
    virtual void unload(Storage::ShaderProgram* shaderProgram) = 0;

    /**
     * Sets the light
     */
    virtual void setLight(Scene::Light* light) = 0;
    /**
     * Resets (removes) lighting.
     */
    virtual void resetLighting() = 0;

    /**
     * Sets the model-view matrix.
     */
    virtual void setTransformMatrix(const Transform3f& transform) = 0;

    /**
     * Draws the binded mesh (with binded materials and texture).
     */
    virtual void draw() = 0;
    /**
     * Draws a particle emitter.
     */
    virtual void draw(Scene::ParticleEmitter *emitter) = 0;

    /**
     * Initializes the GL screen.
     */
    virtual void init() = 0;
    /**
     * Clears the screen.
     */
    virtual void clear() = 0;
    /**
     * Sets the projection.
     */
    virtual void setProjection(const Transform3f &projection) = 0;

    /**
     * Starts the shading stages.
     *
     * @note First stage should not be removed (unless you really know what
     *  are you doing)
     * @note Last stage should always be an Output stage.
     */
    virtual void shading() = 0;
    /**
     * Registers the shading stage.
     */
    virtual void registerStage(quint8 index, Rendering::Stage *stage) = 0;
    /**
     * Unregisters the shading stage.
     */
    virtual void unregisterStage(quint8 index) = 0;
    /**
     * Replaces a stage with a new one.
     */
    virtual void replaceStage(quint8 index, Rendering::Stage *stage) = 0;
    /**
     * Returns the stage with index.
     */
    virtual Rendering::Stage *stage(quint8 index) const = 0;

    /**
     * Disables lighting.
     */
    inline void disableLighting()
    {
      m_hasLighting = false;
    }
    /**
     * Enables lighting.
     */
    inline void enableLighting()
    {
      m_hasLighting = true;
    }
    /**
     * Returns the status of lighting.
     */
    inline bool hasLighting() const
    {
      return m_hasLighting;
    }

    /**
     * Enables or disables VSync property.
     */
    void toggleVSync(bool enable);

  protected:
    inline AbstractDriver() :m_hasLighting(true) {}

  private:
    static AbstractDriver* m_self;
    bool m_hasLighting;
};

}
}

#endif
