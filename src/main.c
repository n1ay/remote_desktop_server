#include "xjpegapi.h"
#include "socketserver.h"
#include <pthread.h>

int main() {
    Display* display = NULL;
    display = XOpenDisplay(NULL);
    assert(display);

    XImage* image = NULL;
    capture_screen(display, &image);

    // show image only for debugging purposes
    //show_image(image);

    unsigned char* buf = malloc(BYTES_PER_SCREEN);
    unsigned long bufsize = 0;
    compress_jpeg(image->data, &buf, &bufsize);

    SocketServerAttr socksrv_attr;
    init_socksrv_attr(&socksrv_attr, "5678", &buf, &bufsize);
    pthread_t socket_server_thread;
    pthread_create(&socket_server_thread, NULL, socket_server_start, (void*) &socksrv_attr);

    while(1) {
        capture_screen(display, &image);
        compress_jpeg(image->data, &buf, &bufsize);
    }

    // free resources
    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
