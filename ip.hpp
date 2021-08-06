
/**
 * A header file for a two functions that scramble a bitset of 64 bits for the
 * initial and final permutations.
 * @author Jacob Overton
 * @version 27 Sep 2017
 */

#include <bitset>
#include <cassert>
#include <iostream>
#include <cassert>

using namespace std;
typedef unsigned int uint;

#ifndef IP_HPP
#define IP_HPP

const int ipbox[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                        60, 52, 44, 36, 28, 20, 12, 4,
                        62, 54, 46, 38, 30, 22, 14, 6,
                        64, 56, 48, 40, 32, 24, 16, 8,
                        57, 49, 41, 33, 25, 17, 9, 1,
                        59, 51, 43, 35, 27, 19, 11, 3,
                        61, 53, 45, 37, 29, 21, 13, 5,
                        63, 55, 47, 39, 31, 23, 15, 7 };

const int ipboxinv[64] { 40, 8, 48, 16, 56, 24, 64, 32,
                         39, 7, 47, 15, 55, 23, 63, 31,
                         38, 6, 46, 14, 54, 22, 62, 30,
                         37, 5, 45, 13, 53, 21, 61, 29,
                         36, 4, 44, 12, 52, 20, 60, 28,
                         35, 3, 43, 11, 51, 19, 59, 27,
                         34, 2, 42, 10, 50, 18, 58, 26,
                         33, 1, 41, 9, 49, 17, 57, 25 };

/**
 * Takes in a set of 64 bits and scrambles them based on a
 * pre-defined table.
 * @param set of 64 bits
 * @return the s-box substitution from the hard-coded arrays
 */
bitset< 64 > ip( bitset< 64 >& input )
{
    bitset< 64 > ipbits = input;

    for (int i = 63; i >= 0; i--)
    {
        ipbits[i] = input[ipbox[i] - 1];
    }

    return ipbits;
}

/**
 * Takes in a set of 64 bits and scrambles them based on a
 * pre-defined table.
 * @param set of 64 bits
 * @return the s-box substitution from the hard-coded arrays
 */
bitset< 64 > ip_inv( bitset< 64 >& input )
{
    bitset< 64 > ipbits;

    for (int i = 63; i >= 0; i--)
    {
        ipbits[i] = input[ipboxinv[i] - 1];
    }

    return ipbits;
}

#endif