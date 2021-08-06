/**
 * header file for completing tasks on the key 
 * this includes converting to 56 bits, shifting bits
 * and converting to 48 bits.
 * @author Evan Brooks
 * @version 23 Sep 2017
 */

#include <bitset>
#include <cassert>
#include <iostream>
#include <cassert>

using namespace std;
typedef unsigned int uint;

#ifndef key_box
#define key_box

const uint PC1[56] = {50, 43, 36, 29, 22, 15, 8, 1, 51, 44, 37, 30, 23, 16, 9, 2, 52,
                      45, 38, 31, 24, 17, 10, 3, 53, 46, 39, 32, 56, 49, 42, 35, 28, 21, 14, 7, 
                      55, 48, 41, 34, 27, 20, 13, 6, 54, 47, 40, 33, 26, 19, 12, 5, 25, 18, 11, 4}; 

/**const uint PC1[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 
    59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 
62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
*/
const uint PC2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
	
const uint shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

void shift_bits( bitset<28> & half_key, uint shift_num )
{
    bitset<2> temp; //holds bits to be wrapped
    temp[ 1 ] = half_key[ 27 ];
    if ( shift_num == 2 )
    {
        temp[ 0 ] = half_key[ 26 ];
    }
    
    half_key <<= shift_num; //shifts by number of bits for round
    //replaces ending numbers 
    if ( shift_num == 1 ) 
    {
        half_key[ 0 ] = temp[ 1 ];
    }
    else
    {
        half_key[ 0 ] = temp[ 0 ];
        half_key[ 1 ] = temp[ 1 ];
    }
}

void per_split( bitset<56> & key, bitset<28> & left, bitset<28> & right )
{
    bitset<56> perm_key;
    //takes 64 bit key and converts it to 56 bit key at beginnning
    for ( uint i = 0; i < 56; i++ )
    {
        perm_key[ 55 - i ] = key[ 55 - ( PC1[ i ] - 1 ) ];
    }
	
    //splits key into two sections
    for ( uint i = 0; i < 28; i++ )
    {
        right[ i ] = perm_key[ i ];
    }
	
    for ( uint i = 0; i < 28; i++ )
    {
        left[ i ] = perm_key[ i + 28 ];
    }
    
}

bitset<48> shift_per( bitset<28> & left, bitset<28> & right, uint round)
{
    //shifts bits each round
    shift_bits( left, shift[ round ] ); 
    shift_bits( right, shift[ round ] );
    uint index;
    bitset<48> key;
    bitset<56> sht;
    //takes halves and puts them back together
    for ( uint i = 0; i < 28; i++ )
    {
        sht[ i ] = right[ i ];
    }
    
    for ( uint i = 0; i < 28; i++ )
    {
        sht[ i + 28 ] = left[ i ];
    }
    
    //converts 56 bit key to 48 bits
    for ( uint i = 0; i < 48; i++ ) 
    {
        index = 55 - ( PC2[ i ] - 1 );
        key[ 47 - i] = sht[ index ];
    }
    return key;
}

#endif