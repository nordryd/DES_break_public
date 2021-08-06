 
typedef unsigned int uint;

#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset

using namespace std;

/**
* The final permutation of DES Encryption
* @author Spencer Berg
* @version 20170925
*/

bitset<32> & other_permutation(bitset<32> & yee)
{
	int initial_message_permutation[] =	   {16, 7, 20, 21, 29, 12, 28, 17,
						1, 15, 23, 26, 5, 18, 31, 10,
						2, 8, 24, 14, 32, 27, 3, 9,
						19, 13, 30, 6, 22, 11, 4, 25};
	
	bitset<32> whee(0x0);
	bitset<32> & woah = whee;
	int i;
	for (i = 0; i<32; i++)
	{
		woah[i] = yee[initial_message_permutation[i] - 1];
	}
	return woah;
}