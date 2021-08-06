/**
 * Contains functions for handling splitting and concatenating text
 * @author Caleb A. Jones
 * @version 30 Sep. 2017
 */

#include <bitset>

void split_text( bitset< 64 > & whole, 
  bitset< 32 > & left, bitset< 32 > & right )
{
  for( uint i = 0; i < 32; i++ )
  {
    right[ i ] = whole[ i ];
    left[ i ] = whole[ i + 32 ];
  }
}

void concat_text( bitset< 64 > & whole, 
  bitset< 32 > & left, bitset< 32 > & right )
{
  for( uint i = 0; i < 32; i++ )
  {
    whole[ i ] = right[ i ];
    whole[ i + 32 ] = left[ i ];
  }
}