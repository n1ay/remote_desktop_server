#include "xjpegapi.h"

int main() {
    Display* display = NULL;
    display = XOpenDisplay(NULL);
    assert(display);

    XImage* image = NULL;
    capture_screen(display, &image);

    // show image only for debugging purposes
    //show_image(image);
    
    unsigned char* buf = NULL;
    unsigned long bufsize = 0;
    compress_jpeg(image->data, &buf, &bufsize);

    // free resources
    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
