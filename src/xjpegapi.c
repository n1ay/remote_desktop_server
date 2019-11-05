#include "xjpegapi.h"

void compress_jpeg(char* data, unsigned char** buf, unsigned long* bufsize) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    unsigned char* buffer = NULL;
    unsigned long buffer_size = 0;
    jpeg_mem_dest(&cinfo, &buffer, &buffer_size);

    cinfo.image_width = WINDOW_WIDTH;
    cinfo.image_height = WINDOW_HEIGHT;
    cinfo.input_components = 4;
    cinfo.in_color_space = JCS_EXT_BGRA;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_COMPRESSION_QUALITY, 1);
    jpeg_start_compress(&cinfo, 1);

    JSAMPROW row_pointer;
    while(cinfo.next_scanline < cinfo.image_height) {
        row_pointer = (JSAMPROW) &data[cinfo.next_scanline * BYTES_PER_PIXEL * WINDOW_WIDTH];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    memset(*buf, 0, BYTES_PER_SCREEN);
    memcpy(*buf, buffer, buffer_size);
    *bufsize = buffer_size;

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    free(buffer);
}
