#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#define BENIS_SIZE_BYTES 4

#include "benis_crypt.h"

namespace HideMyBenis {
    int encrypt(const char *picname, const char *outname, const char *filename, const char *pwd) {
        // load image
        unsigned char *picdata = NULL;
        int x, y, n;
        picdata = stbi_load(picname, &x, &y, &n, 3);
        if (picdata == NULL) {
            return -1;
        }

        // open file
        std::ifstream file;
        file.open(filename, std::ios::binary);
        if (!file.good()) {
            stbi_image_free(picdata);
            return -2;
        }

        // calculate filesize
        long long filesize = 0;
        file.seekg(0, file.end);
        filesize = file.tellg();
        file.seekg(0, file.beg);

        // test if filesize fits in the bytes encoding the filesize
        long long hide_max = 1;
        hide_max = hide_max << (BENIS_SIZE_BYTES * 8);
        if (filesize >= hide_max) {
            stbi_image_free(picdata);
            file.close();
            return -3;
        }
        // Size of hidden, encrypted data including nonce and salt
        size_t hidden_size = (size_t)filesize + BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES +
                             crypto_pwhash_scryptsalsa208sha256_SALTBYTES;
        // Test for overflow
        if(hidden_size <= filesize){
            stbi_image_free(picdata);
            file.close();
            return -3;
        }
        // Maximum of bytes that can be hidden in the image
        size_t pic_capacity = (size_t)(x * y * 3) / 8;
        // Check if hidden_size fits into the image
        if (pic_capacity < hidden_size) {
            stbi_image_free(picdata);
            file.close();
            return -3;
        }

        // Create Arrays for salt, nonce and the message that is to be encrypted
        size_t salt_and_nonce_size = crypto_secretbox_NONCEBYTES +
                                     crypto_pwhash_scryptsalsa208sha256_SALTBYTES;
        unsigned char *salt_and_nonce = new unsigned char[salt_and_nonce_size];
        size_t message_size = pic_capacity - salt_and_nonce_size;
        unsigned char *message = new unsigned char[pic_capacity - salt_and_nonce_size];
        // leave the beginning of the message empty, this will store the authentication
        // start with filling the message with the filesize of the hidden data
        for (int i = crypto_secretbox_MACBYTES; i < BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES; i++) {
            message[i] = (unsigned char) (UINT8_MAX & (filesize >> (8 * (BENIS_SIZE_BYTES - i + crypto_secretbox_MACBYTES - 1))));
        }
        // continue filling message with the actual data that is to be hidden
        for (int i = BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES;
             i < BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES + filesize; i++) {
            char c;
            file.get(c);
            message[i] = (unsigned char) c;
        }
        // fill the rest of the message with random data so that the whole Image
        // will be altered instead of just the part that is needed to store the data
        randombytes_buf(&message[BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES + filesize],
                        message_size - (BENIS_SIZE_BYTES + crypto_secretbox_MACBYTES + (size_t)filesize));
        // calculate random salt and nonce
        randombytes_buf(salt_and_nonce, salt_and_nonce_size);
        // create key via hashing with the salt out of the user's password
        unsigned char key[crypto_secretbox_KEYBYTES];
        if (crypto_pwhash_scryptsalsa208sha256
                    (key, sizeof key, pwd, strlen(pwd), salt_and_nonce,
                     crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE,
                     crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE) != 0) {
            stbi_image_free(picdata);
            file.close();
            delete[](salt_and_nonce);
            delete[](message);
            return -4;
        }

        // encrypt message with key and nonce
        crypto_secretbox_easy(message, &message[crypto_secretbox_MACBYTES], message_size - crypto_secretbox_MACBYTES,
                              &salt_and_nonce[crypto_pwhash_scryptsalsa208sha256_SALTBYTES], key);

        unsigned char *m_ptr = message;
        unsigned char *s_ptr = salt_and_nonce;
        unsigned char *d_ptr = picdata;
        // write plain salt and nonce into the image
        while (salt_and_nonce_size > 0) {
            unsigned char c = *s_ptr;
            s_ptr++;
            for (int i = 7; i >= 0; i--) {
                *d_ptr = (unsigned char)((((*d_ptr) >> 1) << 1) | ((c >> i) & 1));
                d_ptr++;
            }
            salt_and_nonce_size--;
        }
        // write encrypted message including authentication into image
        while (message_size > 0) {
            unsigned char c = *m_ptr;
            m_ptr++;
            for (int i = 7; i >= 0; i--) {
                *d_ptr = (unsigned char)((((*d_ptr) >> 1) << 1) | ((c >> i) & 1));
                d_ptr++;
            }
            message_size--;
        }
        // save the image and cleanup
        stbi_write_png(outname, x, y, 3, picdata, 3 * x);
        stbi_image_free(picdata);
        file.close();
        delete[](salt_and_nonce);
        delete[](message);
        return 0;
    }

    int decrypt(const char *picname, const char *filename, const char *pwd) {
        // load image
        unsigned char *picdata = NULL;
        int x, y, n;
        picdata = stbi_load(picname, &x, &y, &n, 3);
        if (picdata == NULL) {
            return -1;
        }

        // get capacity of image
        size_t pic_capacity = (size_t)(x * y * 3) / 8;

        // Create Arrays for salt, nonce and the message that is to be decrypted
        size_t salt_and_nonce_size = crypto_secretbox_NONCEBYTES +
                                     crypto_pwhash_scryptsalsa208sha256_SALTBYTES;
        unsigned char *salt_and_nonce = new unsigned char[salt_and_nonce_size];
        size_t message_size = pic_capacity - salt_and_nonce_size;
        unsigned char *message = new unsigned char[pic_capacity - salt_and_nonce_size];

        // Read salt, nonce and message from the image
        size_t s_size = salt_and_nonce_size;
        size_t m_size = message_size;
        unsigned char *s_ptr = salt_and_nonce;
        unsigned char *m_ptr = message;
        unsigned char *d_ptr = picdata;
        while (s_size > 0) {
            unsigned char c = 0;
            for (int i = 7; i >= 0; i--) {
                c = (unsigned char) (c | (*d_ptr & 1) << i);
                d_ptr++;
            }
            *s_ptr = c;
            s_ptr++;
            s_size--;
        }
        while (m_size > 0) {
            unsigned char c = 0;
            for (int i = 7; i >= 0; i--) {
                c = (unsigned char) (c | (*d_ptr & 1) << i);
                d_ptr++;
            }
            *m_ptr = c;
            m_ptr++;
            m_size--;
        }

        unsigned char key[crypto_secretbox_KEYBYTES];
        if (crypto_pwhash_scryptsalsa208sha256
                    (key, sizeof key, pwd, strlen(pwd), salt_and_nonce,
                     crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE,
                     crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE) != 0) {
            stbi_image_free(picdata);
            delete[](salt_and_nonce);
            delete[](message);
            return -4;
        }

        if (crypto_secretbox_open_easy(&message[crypto_secretbox_MACBYTES], message, message_size,
                                       &salt_and_nonce[crypto_pwhash_scryptsalsa208sha256_SALTBYTES], key) !=
            0) {
            stbi_image_free(picdata);
            delete[](salt_and_nonce);
            delete[](message);
            return -3;
        }

        std::ofstream file;
        file.open(filename, std::ios::binary);
        if (!file.good()) {
            stbi_image_free(picdata);
            delete[](salt_and_nonce);
            delete[](message);
            return -2;
        }

        m_ptr = &message[crypto_secretbox_MACBYTES];
        size_t actual_size = 0;
        for (int i = 0; i < BENIS_SIZE_BYTES; i++) {
            actual_size = actual_size | ((*m_ptr) << ((BENIS_SIZE_BYTES - i - 1) * 8));
            m_ptr++;
        }

        while (actual_size > 0) {
            file << *m_ptr;
            m_ptr++;
            actual_size--;
        }
        file.close();
        stbi_image_free(picdata);
        delete[](salt_and_nonce);
        delete[](message);
        return 0;
    }
}
