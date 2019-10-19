#ifndef _X_JPEG_API_H_
#define _X_JPEG_API_H_

#include "xapi.h"

#include <jpeglib.h>

#define JPEG_COMPRESSION_QUALITY 75

void compress_jpeg(char* data);

#endif // _X_JPEG_API_H_
