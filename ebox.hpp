/**
 * E-box yo, just steal the function from below. (Killer rhymes)
 * @author Jake Francis
 * @version 21 Sep 2017
 */

#include <bitset>
#include <iostream>
#include <string>

using namespace std;
typedef unsigned int uint;

/**
 *    
 * Takes in a 32-bits and returns the expanded 48-bits
  * @param input 32 bits
  * @return the 48 bit expansion
  */
bitset< 48 > expansion_box( bitset< 32 > & input )
{ 
  bitset< 48 > output;
  output[ 0 ] = input[ 31 ]; output[ 1 ] = input[ 0 ];
  output[ 2 ] = input[ 1 ]; output[ 3 ] = input[ 2 ];
  output[ 4 ] = input[ 3 ]; output[ 5 ] = input[ 4 ];
  output[ 6 ] = input[ 3 ]; output[ 7 ] = input[ 4 ];
  output[ 8 ] = input[ 5 ]; output[ 9 ] = input[ 6 ];
  output[ 10 ] = input[ 7 ]; output[ 11 ] = input[ 8 ];
  output[ 12 ] = input[ 7 ]; output[ 13 ] = input[ 8 ];
  output[ 14 ] = input[ 9 ]; output[ 15 ] = input[ 10 ];
  output[ 16 ] = input[ 11 ]; output[ 17 ] = input[ 12 ];
  output[ 18 ] = input[ 11 ]; output[ 19 ] = input[ 12 ];
  output[ 20 ] = input[ 13 ]; output[ 21 ] = input[ 14 ];
  output[ 22 ] = input[ 15 ]; output[ 23 ] = input[ 16 ];
  output[ 24 ] = input[ 15 ]; output[ 25 ] = input[ 16 ];
  output[ 26 ] = input[ 17 ]; output[ 27 ] = input[ 18 ];
  output[ 28 ] = input[ 19 ]; output[ 29 ] = input[ 20 ];
  output[ 30 ] = input[ 19 ]; output[ 31 ] = input[ 20 ];
  output[ 32 ] = input[ 21 ]; output[ 33 ] = input[ 22 ];
  output[ 34 ] = input[ 23 ]; output[ 35 ] = input[ 24 ];
  output[ 36 ] = input[ 23 ]; output[ 37 ] = input[ 24 ];
  output[ 38 ] = input[ 25 ]; output[ 39 ] = input[ 26 ];
  output[ 40 ] = input[ 27 ]; output[ 41 ] = input[ 28 ];
  output[ 42 ] = input[ 27 ]; output[ 43 ] = input[ 28 ];
  output[ 44 ] = input[ 29 ]; output[ 45 ] = input[ 30 ];
  output[ 46 ] = input[ 31 ]; output[ 47 ] = input[ 0 ];
  return output;
}
