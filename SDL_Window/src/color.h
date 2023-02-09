#ifndef COLOR_H
#define COLOR_H
#include "utility.h"
vec3 write_color( vec3 pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    int ir = static_cast<int>(255.999 * clamp(r, 0.0, 0.999));
    int ig = static_cast<int>(255.999 * clamp(g, 0.0, 0.999));
    int ib = static_cast<int>(255.999 * clamp(b, 0.0, 0.999));

    return vec3(ir, ig, ib);
}

#endif