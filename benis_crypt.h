#pragma once
#include "image.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace HideMyBenis {
class SodiumError: public std::runtime_error{
public:
    explicit SodiumError() : std::runtime_error("ERROR: libsodium went out of memory!"){}
};

class AuthError: public std::runtime_error{
public:
    explicit AuthError() : std::runtime_error("ERROR: Authentication failed!\nWrong password, corrupted data or nothing hidden in this image."){}
};

class SizeError: public std::runtime_error{
public:
    explicit SizeError() : std::runtime_error("ERROR: Input data too large!"){}
    SizeError(size_t pic_capacity, size_t hidden_size) : std::runtime_error("ERROR: Input data too large!\nCapacity: " + std::to_string(pic_capacity) + " Bytes\nSize of Data: " + std::to_string(hidden_size) + " Bytes"){}
};
/**
 * \brief Encrypts a file and hides it inside an image.
 * \param img Image inside which to hide the data.
 * \param input The input that shall be encrypted and hidden inside img. This will assume input to be in a valid state. This function will start reading at the current position. std::ios::binary recommended. std::cin will NOT work.
 * \param pwd The password for encryption.
 * \param fastmode Whether to use faster password-hashing. Has to be decrypted with the same value.
 */
void encrypt(Image& img, std::istream& input, const std::string& pwd, bool fastmode);

/**
 * \brief Extracts the hidding data inside an image processed by encrypt(...).
 * \param img The Image to extract data from.
 * \param output The output stream. Will be assumed to be in a valid state. std::ios::binary recommended.
 * \param pwd The password for decryption.
 * \param fastmode Whether to use faster password-hashing. Has to have been encrypted with the same value.
 */
void decrypt(const Image& img, const std::string& pwd, std::ostream& output, bool fastmode);
}
