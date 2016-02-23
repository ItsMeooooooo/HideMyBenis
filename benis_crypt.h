#ifndef HIDEMYBENIS_BENIS_CRYPT_H
#define HIDEMYBENIS_BENIS_CRYPT_H

#include "sodium.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <fstream>

namespace HideMyBenis {
/*
 * Encrypts the file located at 'filename' with 'pwd'
 * and hides it in the Image located at 'picname'.
 * The result is saved as 'outname'.
 * Returns:
 *  0: On success
 * -1: Error in image loading
 * -2: Error in loading file
 * -3: File too large
 * -4: Sodium out of memory
 */
    int encrypt(const char *picname, const char *outname, const char *filename, const char *pwd);

/*
 * Extracts the hidden data located in the image 'picname'
 * and decrypts it with 'pwd'.
 * The result is saved as 'filename'.
 * Returns:
 *  0: On success
 * -1: Error in image loading
 * -2: Error in saving file
 * -3: Authentication failed while encrypting
 * -4: Sodium out of memory
 */
    int decrypt(const char *picname, const char *filename, const char *pwd);
}
#endif //HIDEMYBENIS_BENIS_CRYPT_H