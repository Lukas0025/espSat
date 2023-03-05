#include "image.h"
#include "debug.h"
//#include "jpeg.inl"

namespace image {

    /**
     * Malloc 
     */
    void *_malloc(size_t size) {
        void * res = malloc(size);
        if(res) {
            return res;
        }

        // check if SPIRAM is enabled and is allocatable
        #if (CONFIG_SPIRAM_SUPPORT && (CONFIG_SPIRAM_USE_CAPS_ALLOC || CONFIG_SPIRAM_USE_MALLOC))
            return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        #endif
            return NULL;
    }

    unsigned espcam2jpegencPixT(pixformat_t pix) {
        if (pix == PIXFORMAT_GRAYSCALE) return JPEG_PIXEL_GRAYSCALE;
        if (pix == PIXFORMAT_RGB565)    return JPEG_PIXEL_RGB565;
        if (pix == PIXFORMAT_RGB888)    return JPEG_PIXEL_RGB888;
        
        ERROR_PRINT("unknown pixel format");
        return JPEG_PIXEL_GRAYSCALE;
    }

    image_t jpg(image_t src, uint8_t quality) {
        image_t    out;
        JPEG       jpgEncoder;
        JPEGENCODE jpe;
        
        //todo: allocate proper buffer for holding JPEG data
        //this should be enough for CIF frame size
        int jpg_buf_len = 128*1024;


        uint8_t * jpg_buf = (uint8_t *)_malloc(jpg_buf_len);
        
        if(jpg_buf == NULL) {
            ERROR_PRINT("fail to allocate JPG out buffer");
            return out;
        }

        auto status = jpgEncoder.open(jpg_buf, jpg_buf_len);
        
        if (status != JPEG_SUCCESS) {
            ERROR_PRINT("fail to open JPG encoder");
            free(jpg_buf);
            return out;
        }

        unsigned iBytePP = 1;
        if (src.fromat == PIXFORMAT_RGB565) iBytePP = 2;
        if (src.fromat == PIXFORMAT_RGB888) iBytePP = 3;

        int iPitch = iBytePP * src.width;

        status = jpgEncoder.encodeBegin(&jpe, src.width, src.height, espcam2jpegencPixT(src.fromat), JPEG_SUBSAMPLE_444, quality);

        unsigned iMCUCount = ((src.width + jpe.cx-1)/ jpe.cx) * ((src.height + jpe.cy-1) / jpe.cy);

        for (unsigned i = 0; i < iMCUCount && status == JPEG_SUCCESS; i++) {
            status = jpgEncoder.addMCU(&jpe, &src.buf[(jpe.x * iBytePP) + (jpe.y * iPitch)], iPitch);
        }
        
        if (status != JPEG_SUCCESS) {
            ERROR_PRINT("fail to encode image to jpeg");
            free(jpg_buf);
            return out;
        }
        
        out.buf    = jpg_buf;
        out.len    = jpgEncoder.close();
        out.fromat = PIXFORMAT_JPEG;
        out.height = src.height;
        out.width  = src.width;

        return out;
    }

    void release(image_t img) {
        free(img.buf);
    }
}
