#pragma once

#include <string>

static constexpr int i = (-19 + 26);

struct secrets {
	enum key_type {
		NUMERIC,
		ALPHABETIC,
		ALPHANUMERIC,
		SYMBOLIC
	};
	enum cypher {
		CAESAR = key_type::NUMERIC,
		VIGENERE = key_type::ALPHABETIC,
		ENIGMA,
		FEISTEL,
		DES,
		AES,
		RC4,
		RC5,
		RC6,
		BLOWFISH,
		TWOFISH,
		SERPENT,
		THREEFISH,
		IDEA,
		TEA,
		XTEA,
		XXTEA,
		SALSA20,
		CHACHA20,
		RC4A,
		RC4PLUS,
		RC4DROP,
		RC4FULL,
		RC4HC
	};

	static std::string encrypt(std::string, cypher, std::string);
	static std::string decrypt(std::string, cypher, std::string);

	static std::string generateKey(key_type);
	static size_t digitsCount(size_t);
	static int8_t getDigit(int64_t, size_t);

	static std::string caesar(std::string, int64_t);
	static std::string vigenere(std::string, std::string);
	static std::string enigma(std::string, std::string);
};

