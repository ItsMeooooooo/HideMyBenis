#ifndef HIDEMYBENIS_BENIS_CRYPT_H
#define HIDEMYBENIS_BENIS_CRYPT_H

#include "stb_image.h"
#include "stb_image_write.h"

namespace HideMyBenis {
/**
 * \brief Encrypts a file and hides it inside an image.
 * \param picname Path to the image inside which to hide the data.
 * \param outname Path to where the modified image will be stored.
 * \param filename The file that shall be encrypted and hidden.
 * \param pwd The password for encryption.
 */
void encrypt(const char* picname, const char* filename, const char* pwd, const char* outname);

/**
 * \brief Extracts the hidding data inside an image processed by encrypt(...).
 * \param picname Path to the image containing hidden data.
 * \param filename Path to where to save the extracted data.
 * \param pwd The password for decryption.
 */
void decrypt(const char* picname, const char* pwd, const char* filename);
}
#endif //HIDEMYBENIS_BENIS_CRYPT_H
