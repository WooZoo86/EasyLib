#ifndef EASY_LIB_CRYPTO_INTERFACE_HEADER_
#define	EASY_LIB_CRYPTO_INTERFACE_HEADER_

#pragma once

#define	ELIB_EXPORT
#ifdef ELIB_EXPORT
#ifdef __cplusplus
#define	ELIB_API	extern "C" __declspec(dllexport)
#else
#define	ELIB_API	__declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define	ELIB_API	extern "C" __declspec(dllimport)
#else
#define	ELIB_API	__declspec(dllimport)
#endif
#endif // ELIB_EXPORT


/********************************** Crypto ****************************************/
//Rijndael
ELIB_API int __stdcall RijndaelSetupEncrypt(unsigned long *rk, const unsigned char *key, int keybits);
ELIB_API int __stdcall RijndaelSetupDecrypt(unsigned long *rk, const unsigned char *key, int keybits);
ELIB_API void __stdcall RijndaelEncrypt(const unsigned long *rk, int nrounds,
	const unsigned char plaintext[16], unsigned char ciphertext[16]);
ELIB_API void __stdcall RijndaelDecrypt(const unsigned long *rk, int nrounds,
	const unsigned char ciphertext[16], unsigned char plaintext[16]);



#endif//EASY_LIB_CRYPTO_INTERFACE_HEADER_