#pragma once

#include <vector>
#include "image_data.h"

namespace downscale {
	image_data<float> pixel_row_shift(
		const image_data<unsigned char> image,
		const std::vector<float> shift_values
	);
}
