#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xos.h>
#include<stdio.h>
#include<assert.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main() {
    Display* display = NULL;
    display = XOpenDisplay(NULL);
    assert(display);

    XImage* image = NULL;
    image = XGetImage(display, RootWindow(display, DefaultScreen(display)), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, AllPlanes, ZPixmap);
    assert(image);

    int screen = DefaultScreen(display);
    Window window = 0;
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 5, WhitePixel(display, screen), BlackPixel(display, screen));
    assert(window);
    
    XSetStandardProperties(display, window, "Test window", "icon", None, NULL, 0, NULL);

    //MapNotify events
    XSelectInput(display, window, StructureNotifyMask);

    //Map the window (make it appear on the screen)
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

    printf("Hello World");
    getchar();
    return 0;
}
