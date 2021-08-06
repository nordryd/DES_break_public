#include <bitset>
#include <iostream>
#include "sbox.hpp"
#include <string>

using namespace std;

int main()
{
  bitset< 48 > baz( "011000010001011110111010100001100110010100100111" );
  bitset< 32 > foobar = substitution_box( baz );
  cout <<  foobar << " " <<  foobar.to_ulong()
   << endl;
  return 0;
}