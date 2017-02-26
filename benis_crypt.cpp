#include "benis_crypt.h"
#include "sodium.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

constexpr size_t FILE_SIZE_BYTES = 4;
constexpr size_t SALT_AND_NONCE_SIZE = crypto_secretbox_NONCEBYTES + crypto_pwhash_scryptsalsa208sha256_SALTBYTES;

namespace HideMyBenis {
void encrypt(Image& img, std::istream& input, const std::string& pwd, bool fastmode)
{
	auto opslimit = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_SENSITIVE;
	auto memlimit = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_SENSITIVE;
	if(fastmode)
	{
		opslimit = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE;
		memlimit = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE;
	}
	unsigned char* picdata = img.get();

	// calculate size
	unsigned long long offset = input.tellg();
	input.seekg(0, input.end);
	unsigned long long insize = unsigned long long(input.tellg()) - offset;
	input.seekg(offset, input.beg);

	// test if size fits in the bytes encoding the size
	unsigned long long hide_max = 1;
	hide_max = hide_max << (FILE_SIZE_BYTES * 8);
	if(insize >= hide_max)
	{
		throw std::runtime_error("ERROR in encrypt: Input data too large!");
	}
	// Size of hidden, encrypted data including nonce and salt
	size_t inputsize = static_cast<size_t>(insize);
	size_t hidden_size = inputsize + FILE_SIZE_BYTES + crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES +
			crypto_pwhash_scryptsalsa208sha256_SALTBYTES;
	// Test for overflow
	if(hidden_size <= inputsize)
	{
		throw std::runtime_error("ERROR in encrypt: Input data too large!");
	}
	// Maximum of bytes that can be hidden in the image
	size_t pic_capacity = img.numChars() / 8;
	// Check if hidden_size fits into the image
	if(pic_capacity < hidden_size)
	{
		throw std::runtime_error("ERROR in encrypt: Input data too large! pic_capacity: " + std::to_string(pic_capacity) + " hidden_size: " + std::to_string(hidden_size));
	}

	// Create Arrays for salt, nonce and the message that is to be encrypted
	unsigned char salt_and_nonce[SALT_AND_NONCE_SIZE];
	size_t message_size = pic_capacity - SALT_AND_NONCE_SIZE;
	std::unique_ptr<unsigned char[]> message(new unsigned char[pic_capacity - SALT_AND_NONCE_SIZE]);
	// leave the beginning of the message empty, this will store the authentication
	// start with filling the message with the filesize of the hidden data
	for(int i = crypto_secretbox_MACBYTES; i < FILE_SIZE_BYTES + crypto_secretbox_MACBYTES; i++) { message[i] = static_cast<unsigned char>(0xff & (inputsize >> (8 * (FILE_SIZE_BYTES - i + crypto_secretbox_MACBYTES - 1)))); }
	// continue filling message with the actual data that is to be hidden
	for(size_t i = FILE_SIZE_BYTES + crypto_secretbox_MACBYTES;
	    i < FILE_SIZE_BYTES + crypto_secretbox_MACBYTES + inputsize; i++)
	{
		char c;
		input.get(c);
		message[i] = static_cast<unsigned char>(c);
	}
	// fill the rest of the message with random data so that the whole Image
	// will be altered instead of just the part that is needed to store the data
	randombytes_buf(&message[FILE_SIZE_BYTES + crypto_secretbox_MACBYTES + inputsize],
	                message_size - (FILE_SIZE_BYTES + crypto_secretbox_MACBYTES + inputsize));
	// calculate random salt and nonce
	randombytes_buf(salt_and_nonce, SALT_AND_NONCE_SIZE);
	// create key via hashing with the salt out of the user's password
	unsigned char key[crypto_secretbox_KEYBYTES];
	if(crypto_pwhash_scryptsalsa208sha256(key, sizeof key, pwd.c_str(), pwd.size(), salt_and_nonce, opslimit, static_cast<size_t>(memlimit)) != 0)
	{
		throw std::runtime_error("ERROR in encrypt: libsodium went out of memory!");
	}

	// encrypt message with key and nonce
	crypto_secretbox_easy(message.get(), &message[crypto_secretbox_MACBYTES], message_size - crypto_secretbox_MACBYTES,
	                      &salt_and_nonce[crypto_pwhash_scryptsalsa208sha256_SALTBYTES], key);

	unsigned char* m_ptr = message.get();
	unsigned char* s_ptr = salt_and_nonce;
	unsigned char* d_ptr = picdata;
	// write plain salt and nonce into the image
	for(size_t j = SALT_AND_NONCE_SIZE; j > 0; j--)
	{
		unsigned char c = *s_ptr;
		s_ptr++;
		for(int i = 7; i >= 0; i--)
		{
			*d_ptr = static_cast<unsigned char>((((*d_ptr) >> 1) << 1) | ((c >> i) & 1));
			d_ptr++;
		}
	}
	// write encrypted message including authentication into image
	for(size_t j = message_size; j > 0; j--)
	{
		unsigned char c = *m_ptr;
		m_ptr++;
		for(int i = 7; i >= 0; i--)
		{
			*d_ptr = static_cast<unsigned char>((((*d_ptr) >> 1) << 1) | ((c >> i) & 1));
			d_ptr++;
		}
	}
}

void decrypt(const Image& img, const std::string& pwd, std::ostream& output, bool fastmode)
{
	auto opslimit = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_SENSITIVE;
	auto memlimit = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_SENSITIVE;
	if(fastmode)
	{
		opslimit = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE;
		memlimit = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE;
	}
	unsigned char* picdata = img.get();

	// get capacity of image
	size_t pic_capacity = img.numChars() / 8;

	// Create Arrays for salt, nonce and the message that is to be decrypted
	unsigned char salt_and_nonce[SALT_AND_NONCE_SIZE];
	size_t message_size = pic_capacity - SALT_AND_NONCE_SIZE;
	std::unique_ptr<unsigned char[]> message(new unsigned char[pic_capacity - SALT_AND_NONCE_SIZE]);

	// Read salt, nonce and message from the image
	unsigned char* s_ptr = salt_and_nonce;
	unsigned char* m_ptr = message.get();
	unsigned char* d_ptr = picdata;
	for(size_t j = SALT_AND_NONCE_SIZE; j > 0; j--)
	{
		unsigned char c = 0;
		for(int i = 7; i >= 0; i--)
		{
			c = static_cast<unsigned char>(c | (*d_ptr & 1) << i);
			d_ptr++;
		}
		*s_ptr = c;
		s_ptr++;
	}
	for(size_t j = message_size; j > 0; j--)
	{
		unsigned char c = 0;
		for(int i = 7; i >= 0; i--)
		{
			c = static_cast<unsigned char>(c | (*d_ptr & 1) << i);
			d_ptr++;
		}
		*m_ptr = c;
		m_ptr++;
	}

	unsigned char key[crypto_secretbox_KEYBYTES];
	if(crypto_pwhash_scryptsalsa208sha256(key, sizeof key, pwd.c_str(), pwd.size(), salt_and_nonce, opslimit, static_cast<size_t>(memlimit)) != 0)
	{
		throw std::runtime_error("ERROR in decrypt: libsodium went out of memory!");
	}

	if(crypto_secretbox_open_easy(&message[crypto_secretbox_MACBYTES], message.get(), message_size,
	                              &salt_and_nonce[crypto_pwhash_scryptsalsa208sha256_SALTBYTES], key) !=
		0)
	{
		throw std::runtime_error("ERROR in decrypt: Authentication failed!\nWrong password, corrupted data or nothing hidden in this image.");
	}

	m_ptr = &message[crypto_secretbox_MACBYTES];
	size_t actual_size = 0;
	for(int i = 0; i < FILE_SIZE_BYTES; i++)
	{
		actual_size = actual_size | ((*m_ptr) << ((FILE_SIZE_BYTES - i - 1) * 8));
		m_ptr++;
	}

	for(size_t i = actual_size; i > 0; i--)
	{
		output << *m_ptr;
		m_ptr++;
	}
}
}
