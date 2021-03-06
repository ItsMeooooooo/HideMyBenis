#pragma once
#include <string>
#include <stdexcept>

namespace HideMyBenis {
class StbLoadException : public std::runtime_error{
public:
    explicit StbLoadException(const std::string& fileName) : std::runtime_error("ERROR \"" + fileName + "\":\n Could not load image!"){}
};

class StbWriteException : public std::runtime_error{
public:
    explicit StbWriteException(const std::string& fileName) : std::runtime_error("ERROR \"" + fileName + "\":\n Could not write image!"){}
};

class Image
{
public:
	explicit Image(const std::string& filename);
	Image(const Image& other) = delete;
	Image(Image&& other) = delete;

	Image& operator=(const Image& other) = delete;
	Image& operator=(Image&& other) = delete;

	~Image();

	unsigned char* get() const { return data; }

	void writeToFile(const std::string& filename) const;

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int numPixels() const { return width * height; }
	int numChars() const { return width * height * channels; }
	int getChannels() const { return channels; }
private:
	unsigned char* data;
	int width;
	int height;
	int channels = 3;
};
}
