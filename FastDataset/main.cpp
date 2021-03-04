#include "loadpng\lodepng.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>

#include "image_data.h"
#include "downscale_cpu.h"
#include <iostream>


std::vector<float> random_shifts()
{
    return std::vector<float>{
        0.45827556, 0.81700253, 0.10976018, 0.94226881, 0.92507905,
            0.3281929, 0.76806125, 0.71006357, 0.92269042, 0.03214076,
            0.11784005, 0.77842902, 0.60397358, 0.10120204, 0.2834368,
            0.14302333, 0.81090329, 0.14976199, 0.8049117, 0.84405617,
            0.25479715, 0.8276375, 0.71067842, 0.80527478, 0.76914734,
            0.02411469, 0.16829006, 0.24266901, 0.23532346, 0.82618664,
            0.87076253, 0.29077999, 0.53627999, 0.08863232, 0.17803077,
            0.04235153, 0.73004863, 0.12771995, 0.50726152, 0.28726838,
            0.76762234, 0.14399337, 0.9058409, 0.15567541, 0.93813085,
            0.97081135, 0.98268429, 0.69171163, 0.20419215, 0.57377654,
            0.97479026, 0.53501836, 0.71777861, 0.16413508, 0.37370553,
            0.11900185, 0.47896497, 0.75426357, 0.9888634, 0.54197847,
            0.41962811, 0.66753586, 0.0359308, 0.1575505};
}


image_data<unsigned char> extract_snippet(image_data<unsigned char>& image)
{
    const int snippet_size = 256; // uniform snippet size

    int start_x = std::rand() % (image.width() - snippet_size);
    int start_y = std::rand() % (image.height() - snippet_size);

    image_data<unsigned char> snippet(snippet_size, snippet_size);

    for (int x = 0; x < snippet_size; x++)
    {
        for (int y = 0; y < snippet_size; y++)
        {
            snippet(x, y) = image(start_x + x, start_y + y);
        }
    }

    return snippet;
}

std::string pad_int(int number)
{
    auto num = std::to_string(number);
    return std::string(4 - num.size(), '0').append(num);
}

std::string get_file_name(int index)
{
    std::string div2k = "../../DIV2K Dataset/DIV2K_train_HR/";
    return div2k.append(pad_int(index)).append(".png");
}

const int snippet_count = 15;

void iteration(int num)
{
    auto hd_img = image_data<unsigned char>(get_file_name(num));

    for (int i = 0; i < snippet_count; i++)
    {
        int index = num * snippet_count + i;
        auto padded = pad_int(index) + ".png";

        auto original = extract_snippet(hd_img);
        original.save_as("original-" + padded);

        auto control_shift = std::vector<float>{ 0.f, 0.f };
        auto control = downscale::pixel_row_shift(original, control_shift);
        control.save_as("control-" + padded);

        auto half_shift = std::vector<float>{ 0.f, 0.5f };
        auto half = downscale::pixel_row_shift(original, half_shift);
        half.save_as("half-" + padded);

        auto quarter_shift = std::vector<float>{ 0.f, 0.25f, 0.5f, 0.75f };
        auto quarter = downscale::pixel_row_shift(original, quarter_shift);
        quarter.save_as("quarter-" + padded);

        auto random = downscale::pixel_row_shift(original, random_shifts());
        random.save_as("random-" + padded);
    }

    std::cout << num << std::endl;
}


int main()
{
    std::srand(std::time(nullptr));

    auto start = std::chrono::high_resolution_clock::now();

    for (int num = 1; num < 101; num++)
    {
        iteration(num);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << time.count() << std::endl;
}
