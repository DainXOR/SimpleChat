#include "secrets.hpp"

#include <iostream>
#include <string>

namespace dsc {
	std::string secrets::encrypt(std::string message,
								 cypher encryptionType,
								 std::string key = "") {
		if (key == "") {
			key = generateKey(static_cast<key_type>(encryptionType));
		}

		switch (encryptionType) {
			case secrets::CAESAR:
				message = caesar(message, std::stoi(key));
				break;
			case secrets::VIGENERE:
				message = vigenere(message, key);
				break;
			case secrets::ENIGMA:
				break;
			case secrets::FEISTEL:
				break;
			case secrets::DES:
				break;
			case secrets::AES:
				break;
			case secrets::RC4:
				break;
			case secrets::RC5:
				break;
			case secrets::RC6:
				break;
			case secrets::BLOWFISH:
				break;
			case secrets::TWOFISH:
				break;
			case secrets::SERPENT:
				break;
			case secrets::THREEFISH:
				break;
			case secrets::IDEA:
				break;
			case secrets::TEA:
				break;
			case secrets::XTEA:
				break;
			case secrets::XXTEA:
				break;
			case secrets::SALSA20:
				break;
			case secrets::CHACHA20:
				break;
			case secrets::RC4A:
				break;
			case secrets::RC4PLUS:
				break;
			case secrets::RC4DROP:
				break;
			case secrets::RC4FULL:
				break;
			case secrets::RC4HC:
				break;
			default:
				break;
		}

		return message;
	}
	std::string secrets::decrypt(std::string message,
								 cypher encryptionType,
								 std::string key) {

		switch (encryptionType) {
			case secrets::CAESAR:
				message = caesar(message, -std::stoi(key));
				break;
			case secrets::VIGENERE:
				message = vigenere(message, key);
				break;
			case secrets::ENIGMA:
				break;
			case secrets::FEISTEL:
				break;
			case secrets::DES:
				break;
			case secrets::AES:
				break;
			case secrets::RC4:
				break;
			case secrets::RC5:
				break;
			case secrets::RC6:
				break;
			case secrets::BLOWFISH:
				break;
			case secrets::TWOFISH:
				break;
			case secrets::SERPENT:
				break;
			case secrets::THREEFISH:
				break;
			case secrets::IDEA:
				break;
			case secrets::TEA:
				break;
			case secrets::XTEA:
				break;
			case secrets::XXTEA:
				break;
			case secrets::SALSA20:
				break;
			case secrets::CHACHA20:
				break;
			case secrets::RC4A:
				break;
			case secrets::RC4PLUS:
				break;
			case secrets::RC4DROP:
				break;
			case secrets::RC4FULL:
				break;
			case secrets::RC4HC:
				break;
			default:
				break;
		}

		return message;
	}

	std::string secrets::generateKey(key_type keyType) {
		switch (keyType) {
			case secrets::NUMERIC:
				break;
			case secrets::ALPHABETIC:
				break;
			case secrets::ALPHANUMERIC:
				break;
			case secrets::SYMBOLIC:
				break;
			default:
				break;
		}
		return "";
	}

	size_t secrets::digitsCount(size_t number) {
		size_t digits = 0;

		while (number > 0) {
			number /= 10;
			digits++;
		}
		return digits;
	}

	int8_t secrets::getDigit(int64_t number, size_t digit) {
		for (size_t i = 0; i < digit; i++) {
			number /= 10;
		}
		number -= (number - (number % 10));

		return static_cast<int8_t>(number);
	}

	std::string secrets::caesar(std::string message, int64_t key) {
		for (size_t i = 0; i < message.size(); i++) {
			message[i] = uint8_t(message[i] + key);
		}

		return message;
	}

	std::string secrets::vigenere(std::string message, std::string key) {
		for (size_t i = 0; i < message.size(); i++) {
			message[i] = uint8_t(message[i] + key[i % key.size()]);
		}

		return message;
	}

	std::string secrets::enigma(std::string message, std::string key) {
		// Implement Enigma


		return std::string();
	}

}
