#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <bitset>
#include <sstream>

#include "cryptopp/modes.h"
#include "cryptopp/des.h"
#include "cryptopp/filters.h"
#include <cmath>
typedef unsigned long long ullong;
using namespace std;
string des_encrypt( string & plaintext, byte key[] )
{
    std:string ciphertext;

    CryptoPP::DES::Encryption desEncryption(key, CryptoPP::DES::DEFAULT_KEYLENGTH);
    CryptoPP::ECB_Mode_ExternalCipher::Encryption ecbEncryption( desEncryption);

    CryptoPP::StreamTransformationFilter stfEncryptor(ecbEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();

    return ciphertext;
}

string des_encrypt( string & plaintext, bitset<64> & key_bits)
{
    std:string ciphertext;
    cout << key_bits << endl;
    byte key[CryptoPP::DES::DEFAULT_KEYLENGTH]; 
    for( int i = 7; i >= 0 ; i-- )
    {
		int q = (128 * key_bits[ (i + 1) * 8 - 1 ] + 64 * key_bits[ (i + 1) * 8 - 2 ] +
			32 * key_bits[ (i + 1) * 8 - 3 ] + 16 * key_bits[ (i + 1) * 8 - 4 ] +
			8 * key_bits[ (i + 1) * 8 - 5 ] + 4 * key_bits[ (i + 1) * 8 - 6 ] +
			2 * key_bits[ (i + 1) * 8 - 7 ] + 1 * key_bits[ (i + 1) * 8 - 8 ]);
		key[i] = q;		
	}
	// memset( key, key_hex, CryptoPP::DES::DEFAULT_KEYLENGTH);
    
    CryptoPP::DES::Encryption desEncryption(key, CryptoPP::DES::DEFAULT_KEYLENGTH);
    CryptoPP::ECB_Mode_ExternalCipher::Encryption ecbEncryption( desEncryption);

    CryptoPP::StreamTransformationFilter stfEncryptor(ecbEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();
    
    return ciphertext;
}

string des_decrypt( string & ciphertext, byte key[] )
{
    std:string decryptedtext;

    CryptoPP::DES::Decryption desDecryption(key, CryptoPP::DES::DEFAULT_KEYLENGTH);
    CryptoPP::ECB_Mode_ExternalCipher::Decryption ecbDecryption( desDecryption );

    CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink( decryptedtext ), 
		CryptoPP::StreamTransformationFilter::NO_PADDING  );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    return decryptedtext;
}

string des_decrypt( string & ciphertext, bitset<64> & key_bits )
{
    std:string decryptedtext;

    ullong key_hex;
    stringstream ss;
    ss << hex << key_bits.to_ulong() << endl;
    ss >> key_hex;
   
    byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
    
    for( int i = 7; i >= 0 ; i-- )
    {
		int q = (128 * key_bits[ (i + 1) * 8 - 1 ] + 64 * key_bits[ (i + 1) * 8 - 2 ] +
			32 * key_bits[ (i + 1) * 8 - 3 ] + 16 * key_bits[ (i + 1) * 8 - 4 ] +
			8 * key_bits[ (i + 1) * 8 - 5 ] + 4 * key_bits[ (i + 1) * 8 - 6 ] +
			2 * key_bits[ (i + 1) * 8 - 7 ] + 1 * key_bits[ (i + 1) * 8 - 8 ]);
		key[i] = q;		
	}
    CryptoPP::DES::Decryption desDecryption(key, CryptoPP::DES::DEFAULT_KEYLENGTH);
    CryptoPP::ECB_Mode_ExternalCipher::Decryption ecbDecryption( desDecryption );

    CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink( decryptedtext ),	
		CryptoPP::StreamTransformationFilter::NO_PADDING );
  

    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();
	
    return decryptedtext;
}
