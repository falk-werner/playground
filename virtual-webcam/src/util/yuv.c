#include "yuv.h"
#include "image.h"

static uint8_t
clamp(double value)
{
    if (0 > value) { return 0; }
    if (255 < value) { return 255; }
    return (uint8_t) value;
}

void 
rgb_to_yuv(struct pixel const * rgb, struct yuv * yuv)
{
    double r = rgb->r;
    double g = rgb->g;
    double b = rgb->b;

    double y = ( 0.299000 * r) + ( 0.587000 * g) + ( 0.114000 * b);
    double u = (-0.168736 * r) + (-0.331264 * g) + ( 0.500000 * b) + 128;
    double v = ( 0.500000 * r) + (-0.418688 * g) + (-0.081312 * b) + 128;

    yuv->y = clamp(y);
    yuv->u = clamp(u);
    yuv->v = clamp(v);
}
