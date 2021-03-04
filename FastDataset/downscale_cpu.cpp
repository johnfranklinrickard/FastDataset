#include "downscale_cpu.h"

#include <cassert>


image_data<float> downscale::pixel_row_shift(
    const image_data<unsigned char> image,
    const std::vector<float> shift_values)
{
    const int downscale_factor = 4;
    const int width = image.width();
    const int height = image.height();
    assert(width % downscale_factor == 0 && height % downscale_factor == 0);

    // initialize empty img with fourth the size
    auto output = image_data<float>(width / downscale_factor, height / downscale_factor);

    const int shift_size = shift_values.size();

    for (int x = 0; x < width; x++)
    {
        const int out_x = x / downscale_factor;
        for (int y = 0; y < height; y++)
        {
            const int out_y = static_cast<int>(y / static_cast<float>(downscale_factor));
            const int shift_index = out_y % shift_size;

            // linear interpolation between neighboring pixels
            const float alpha = shift_values[shift_index];
            const auto p1 = image(x, y);
            const auto p2 = image(std::min(x + 1, width - 1), y);
            output(out_x, out_y) += (1 - alpha) * p1 + alpha * p2;
        }
    }

    // Normalize the values with the     input pixel count
    const float norm_factor = downscale_factor * downscale_factor;
    for (int x = 0; x < output.width(); x++)
    {
        for (int y = 0; y < output.height(); y++)
        {
            output(x, y) = output(x, y) / norm_factor;
        }
    }

    return output;
}
