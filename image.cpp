#include "image.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"
#include "stb_image_write.h"

namespace HideMyBenis {
Image::Image(std::string filename)
{
	data = stbi_load(filename.c_str(), &width, &height, nullptr, 3);
	if(!data)
	{
		throw std::runtime_error("ERROR in Image::Image(\"" + filename + "\"): Could not load image!");
	}
}

Image::~Image()
{
	stbi_image_free(data);
}

void Image::writeToFile(std::string filename) const
{
	if(!stbi_write_png(filename.c_str(), width, height, 3, data, 3 * width))
	{
		throw std::runtime_error("ERROR in Image::writeToFile(\"" + filename + "\"): Could not write image!");
	}
}
}
