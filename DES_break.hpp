/**
 * Class to find the key used to encrypt a string with DES via brute
 * force. Uses multi-threading.
 * @author Jacob Overton
 * @version 27 October 2017
 */

#ifndef DESBREAKTHREAD_HPP
#define DESBREAKTHREAD_HPP

#include <cstdio>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <thread>
#include <bitset>
#include <string>

#include "des_cryptopp.hpp"
#include "file_eq.hpp"
#include <fstream>

#include "cryptopp/modes.h"
#include "cryptopp/des.h"
#include "cryptopp/filters.h"

#define BITS 64

using namespace std;
typedef unsigned int uint;
typedef unsigned long long ullong;


/**
 *  Increment the key, keeping in mind that every 8th bit is omitted
 * */
void increment_key( bitset<64> & key )
{
	bitset<56> short_key(0);
	for( int i = 0; i < 64; i++ )
	{
		if( i % 8 )//if not an 8th bit
		{
			short_key[i - (i/8) - 1 ] = key[ i ];
		}
	}
	short_key = bitset<56>(short_key.to_ullong() + 1 );
	//cout << short_key << endl;
	for( int i = 0; i< 63; i++ )
	{
		if( i % 8 )//if not an 8th bit
		{
			key[i] = short_key[ i - (i/8) - 1  ];
		}
	}
}

/** 
 * Convert a number, keeping in mind that every 8th bit is omitted
 * */
void convert_key_56( ullong & key_num )
{
	bitset<64> key( key_num );
	bitset<56> short_key(0);
	for( int i = 0; i < 64; i++ )
	{
		if( i % 8 )//if not an 8th bit
		{
			short_key[i - (i/8) - 1 ] = key[ i ];
		}
	}
	key_num = short_key.to_ullong();
}

void breakDES(void* caller, uint id, ullong min, ullong max, string& PLAINTEXT, string& CIPHERTEXT );
class DESBreakThread
{//
private:

uint NUM_THREADS;

uint STRLEN;

ullong MIN;
ullong MAX;
ullong KEYSLICE;
ullong KEYSLICEREM;
string PLAINTEXT;

string CIPHERTEXT;

bool foundKey;

public:
bitset<BITS> key;


DESBreakThread(uint _NUM_THREADS, ullong _MIN, ullong _MAX, string& _PLAINTEXT, string & _CIPHERTEXT)

{
    NUM_THREADS = _NUM_THREADS;
	MIN = _MIN;      
    MAX = _MAX;
    STRLEN = PLAINTEXT.length();
    PLAINTEXT = _PLAINTEXT;
	KEYSLICE = ((MAX - MIN) / NUM_THREADS );
	KEYSLICEREM = (MAX - MIN) % NUM_THREADS;
	CIPHERTEXT = _CIPHERTEXT;

	
	foundKey = false;
	key = bitset<BITS>(0);
}


bool get_found()
{
	return foundKey;
}
bool set_found( bool stat )
{
	foundKey = stat;
}

/*/*******************************************************************/
/*/*******************************************************************/
void crack() //MAIN METHOD
{
    thread t[NUM_THREADS];
    
    for(uint i = 0; i < NUM_THREADS; i++)
    {
		if(i == NUM_THREADS - 1)
		{
			t[i] = thread(breakDES, this, i + 1, MIN + (KEYSLICE * (ullong)i), MAX, ref(PLAINTEXT), ref(CIPHERTEXT));
		}
		else
		{
			t[i] = thread(breakDES, this, i + 1, MIN + (KEYSLICE * (ullong)i), MIN + (KEYSLICE * ((ullong)(i + 1))) - 1, ref(PLAINTEXT), ref(CIPHERTEXT));
		}
	}
	

	
	
	for(uint i = 0; i < NUM_THREADS; i++)
	{	
			t[i].join();
		
	}
/*
	for(uint i = 0; i < NUM_THREADS; i++)
	{
		if(&t[i] != nullptr)
		{
			t[i].detach();
		}
	}*/
}
/*/********************************************************************/
/*/********************************************************************/

bitset<BITS> get_key()
{
	return key;
}

ullong get_key_ullong()
{
	return key.to_ullong();
}
};//
#endif

void breakDES(void* caller, uint id, ullong min, ullong max, string& PLAINTEXT, string& CIPHERTEXT )
{ 
	uint STRLEN = PLAINTEXT.length();
	ullong currKey = min;
	bitset<BITS>currKeyBits( currKey );
//	printf("[THREAD NO.%d] Searching range [%llu,%llu]\n", id, min, max);
   	DESBreakThread * dbt = static_cast< DESBreakThread* >( caller );
	while(currKey <= max && !dbt->get_found())
	{
		//cout << currKeyBits.to_ullong() << endl;
//		printf("[THREAD NO.%d] currKey = %llu\n", id, currKey);

		string gen_plain = des_decrypt(CIPHERTEXT, currKeyBits).substr(0, STRLEN);
		
		if(PLAINTEXT == gen_plain )//str_sim(PLAINTEXT, gen_plain) >= .70)
		{
//			printf("[THREAD NO.%d] KEY FOUND -> %llu\n", id, currKey);
			dbt->key = currKeyBits;
			dbt->set_found(true);
			cout << dbt->get_found() << endl;
		}
		currKey++;
		increment_key(currKeyBits);
	}
}
