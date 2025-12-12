#pragma once

#include <string>

namespace dsc {
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
		static std::string feistel(std::string, std::string);
		static std::string des(std::string, std::string);
		static std::string aes(std::string, std::string);
		static std::string rc4(std::string, std::string);
		static std::string rc5(std::string, std::string);
		static std::string rc6(std::string, std::string);
		static std::string blowfish(std::string, std::string);			

		static uint64_t modularExponentiation(uint64_t, uint64_t, uint64_t);

		static uint64_t generatePrimeNumber(uint64_t = 2, uint64_t = -1);
		static uint64_t isPrime(uint64_t);
		static uint64_t isPrimeBig(uint64_t);

	};

}

