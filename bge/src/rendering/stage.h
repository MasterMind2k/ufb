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

class Stage
{
  public:
    enum Output {
      Textures,
      Display
    };

    inline Stage()
    {
      m_renderOutput = Display;
      m_needLights = false;
      m_framebuffer = 0l;
    }

    virtual void render() = 0;

  protected:
    /**
     * Creates the pass.
     */
    void pass();

    inline void setOutput(Output renderOutput)
    {
      m_renderOutput = renderOutput;
    }
    inline Output renderOutput() const
    {
      return m_renderOutput;
    }

    inline void enableLights()
    {
      m_needLights = true;
    }
    inline void disableLights()
    {
      m_needLights = false;
    }

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
