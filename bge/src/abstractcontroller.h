/***************************************************************************
 *   Copyright (C) 2010 by Gregor Kališnik <gregor@unimatrix-one.org>      *
 *   Copyright (C) 2010 by Matej Jakop     <matej@jakop.si>                *
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
#ifndef __BGE_ABSTRACTCONTROLLER_H
#define __BGE_ABSTRACTCONTROLLER_H

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>

namespace BGE
{

/**
 * @short Input handling class.
 *
 * Use this class to implement your input handling through the canvas.
 *
 * @see Canvas::setController
 */
class AbstractController
{
  public:
    /**
     * This method gets called when a user presses a key.
     */
    virtual void keyPressed(QKeyEvent* event) {}
    /**
     * This method gets called wen user moves a mouse.
     */
    virtual void mouseMoved(QMouseEvent* event) {}
    /**
     * This method gets called wen user presses a mouse button.
     */
    virtual void mouseButtonPressed(QMouseEvent* event) {}
};

}

#endif
