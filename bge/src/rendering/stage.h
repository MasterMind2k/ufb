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
#ifndef __BGE_RENDERING_STAGE_H
#define __BGE_RENDERING_STAGE_H

#include <QtCore/QStringList>
#include <QtCore/QHash>

class FBO;

namespace BGE {
namespace Driver {
class GL3;
}
namespace Storage {
class ShaderProgram;
}

namespace Rendering {

/**
 * An abstract stage. To define your own, you have to reimplement render method.
 */
class Stage
{
  public:
    /**
     * Output methods.
     */
    enum Output {
      /** Stage is rendered to a texture. */
      Textures,
      /** Stage is rendered directly to the display. */
      Display
    };

    inline Stage()
    {
      m_renderOutput = Display;
      m_needLights = false;
      m_framebuffer = 0l;
    }

    /**
     * This method renders the stage.
     */
    virtual void render() = 0;

  protected:
    /**
     * Renders a pass.
     */
    void pass();

    /**
     * Sets the render output.
     */
    inline void setOutput(Output renderOutput)
    {
      m_renderOutput = renderOutput;
    }
    /**
     * Returns the selected rendering output.
     */
    inline Output renderOutput() const
    {
      return m_renderOutput;
    }

    /**
     * Bind the lighting to the shaders. One pass will be rendered multiple
     * times and its outputs will be additively blended.
     */
    inline void enableLights()
    {
      m_needLights = true;
    }
    /**
     * Doesn't bind the lights. The rendering pass will render only once.
     */
    inline void disableLights()
    {
      m_needLights = false;
    }

    /**
     * Adds a texture for fragment shader to use.
     */
    inline void addTexture(const QString &name)
    {
      m_textures << name;
    }

  private:
    FBO *m_framebuffer;
    Driver::GL3 *m_driver;
    Output m_renderOutput;
    QStringList m_textures;
    bool m_needLights;

    friend class BGE::Driver::GL3;
};

}
}

#endif
