#pragma once
#include "image.h"
#include <iostream>

namespace HideMyBenis {
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
