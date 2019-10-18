#include "xapi.h"

int main() {
    Display* display = NULL;
    display = XOpenDisplay(NULL);
    assert(display);

    XImage* image = NULL;
    capture_screen(display, &image);

    // show image only for debugging purposes
    //show_image(image);

    // free resources
    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
