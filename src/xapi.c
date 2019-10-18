#include "xapi.h"

void show_image(XImage* image) {
    assert(image);

    Display* display = NULL;
    display = XOpenDisplay(NULL);
    assert(display);

    int screen = DefaultScreen(display);
    Window window = 0;
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 5, WhitePixel(display, screen), BlackPixel(display, screen));
    assert(window);
    
    XSetStandardProperties(display, window, "show_image", "icon", None, NULL, 0, NULL);

    // MapNotify events
    XSelectInput(display, window, StructureNotifyMask);

    // Map the window (make it appear on the screen)
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, 0);
    XSetForeground(display, gc, WhitePixel(display, screen));

    while(1) {
        XEvent e;
        XNextEvent(display, &e);
        if (e.type == MapNotify) {
            break;
        }
    }

    XPutImage(display, window, gc, image, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // send display to the server
    XFlush(display);

    // wait until key is pressed
    getchar();

    // free resources
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void capture_screen(Display* display, XImage** image) {
    if ((image) && (*image)) {
        XDestroyImage(*image);
    }
    *image = XGetImage(display, RootWindow(display, DefaultScreen(display)), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, AllPlanes, ZPixmap);
}
