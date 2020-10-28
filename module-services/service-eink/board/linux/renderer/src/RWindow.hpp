// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

/*
 * RWindow.hpp
 *
 *  Created on: 24 kwi 2019
 *      Author: robert
 */

#ifndef RENDERER_RWINDOW_HPP_
#define RENDERER_RWINDOW_HPP_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdk/gdk.h> // for GdkEventKey
#include <map>       // for map
#include <cstdint>   // for uint32_t, int8_t

#include "RArea.hpp" // for RArea
#include "module-bsp/bsp/keyboard/key_codes.hpp"
#include "gtkmm/window.h" // for Window

class RWindow : public Gtk::Window
{
  protected:
    RArea drawArea;
    // pointer to shared memory with gui renderer buffer passed from main.
    char *shmMemPtr;
    // local buffer
    char *rgbMemPtr;
    // dimensions of the shared memory buffer
    int width, height;
    // map that holds pair of key, time
    //	pair is inserted into map when there is key event of GDK_KEY_PRESS
    // it is removed when there is GDK_KEY_RELEASE event
    std::map<uint32_t, uint32_t> keys;

    std::map<int8_t, uint32_t> keyMap;
    std::map<int8_t, uint32_t> batteryKeyMap;
    // named pipe file descriptor
    int fifoFd;
    // named pipe file descriptor
    int fifoFdBatt;
    void keyMapInit(void);

  public:
    RWindow(char *shmMemory, int fifoKbd, int fifoBatt, int w, int h);
    virtual ~RWindow();
    bool onKeyPress(GdkEventKey *event);
    bool onKeyRelease(GdkEventKey *event);

    void updateDrawBuffer();
};

#endif /* RENDERER_RWINDOW_HPP_ */
