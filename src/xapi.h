#ifndef X_API_H_
#define X_API_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <assert.h>
#include <stdio.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define BYTES_PER_PIXEL 4

void capture_screen(Display* display, XImage** image);

// this is very ineffective method of showing image on the screen which should be used only for debugging purposes.
void show_image(XImage* image);

#endif // X_API_H_
