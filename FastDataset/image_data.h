#pragma once

#include <vector>
#include <string>
#include <cassert>
#include "loadpng/lodepng.h"

/// <summary>
/// Contains greyscale image data and size information.
/// </summary>
/// <typeparam name="T">The held data type</typeparam>
template<class T>
class image_data
{
public:

	/// <summary>
	/// Constructs an empty image in memory with the given sizes
	/// </summary>
	image_data(const int width, const int height)
	{
		m_width = width;
		m_height = height;
		m_data = std::vector<T>(width * height, T());
	}

	/// <summary>
	/// Loads an image into memory from the given file name.
	/// </summary>
	/// <param name="filename"></param>
	image_data(const std::string& filename)
	{
		std::vector<unsigned char> image;
		unsigned int width;
		unsigned int height;
		unsigned int error = lodepng::decode(
			image,
			width,
			height,
			filename,
			LodePNGColorType::LCT_GREY);

		assert(error == 0);

		m_width = width;
		m_height = height;
		if (std::is_same_v<T, unsigned int>)
			m_data = image;
		else
			m_data = std::vector<T>(image.begin(), image.end());
	}

	/// <summary>
	/// Converts image data from one underlying data type to another
	/// </summary>
	template<class U> image_data(const image_data<U>& other)
	{
		static_assert(std::is_convertible<U, T>::value, "U has to be t-convertible")
		m_width = other.m_width;
		m_height = other.m_height;
		m_data = std::vector<T>(other.m_data.begin(), other.m_data.end());
	}

	const int width() const { return m_width; }

	const int height() const { return m_height; }

	/// <summary>
	/// Saves the image as grayscale with unsigned char pixel values.
	/// </summary>
	void save_as(const std::string& name) const
	{
		unsigned int error = lodepng::encode(
			name,
			std::vector<unsigned char>(m_data.begin(), m_data.end()),
			static_cast<unsigned int>(m_width),
			static_cast<unsigned int>(m_height),
			LodePNGColorType::LCT_GREY);

		assert(error == 0);
	}

	/// <summary>
	/// Gains access to the pixel value at a given index
	/// </summary>
	T& operator()(const int x, const int y)
	{
		assert(x < m_width&& y < m_height);
		return m_data[y * m_width + x];
	}

	/// <summary>
	/// Returns the pixel value at a given index
	/// </summary>
	const T& operator()(const int x, const int y) const
	{
		assert(x < m_width&& y < m_height);
		return m_data[y * m_width + x];
	}

	static_assert(std::is_arithmetic<T>::value, "Type T has to be an arithmetic type");

private:
	int m_width;
	int m_height;
	std::vector<T> m_data;

};

template<>
void image_data<unsigned char>::save_as(const std::string& name) const
{
	unsigned int error = lodepng::encode(
		name,
		m_data,
		static_cast<unsigned int>(m_width),
		static_cast<unsigned int>(m_height),
		LodePNGColorType::LCT_GREY);

	assert(error == 0);
}