#include "ebox.hpp"
#include "init.hpp"
#include "ip.hpp"
#include "key_box.hpp"
#include "sbox.hpp"
#include "second_permutation.hpp"
#include "split_and_concat.hpp"

#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<chrono>

#ifndef DES_OBJ_HPP
#define DES_OBJ_HPP

using namespace std;
using namespace std::chrono; 

/**
 * The driver for the DES algorithm. Takes in command line parameters
 * including whether you want to encrypt/decrypt, the number of rounds
 * (1 to 16), the input filename, the output filename, and the key, and then
 * either encrypts or decrypts.
 * @authors Caleb Jones, Spencer Berg, Evan Brooks, Jacob Overton,
 *    Jake Francis
 * @version 30 Sep. 2017
 */
class des
{
	private: 
		bitset<56> key;
		int num_rounds;

	public:

des( bitset<56> & _key, int _rounds )
 {
	key = _key;
	num_rounds = _rounds;
 }
 
void encrypt( string & inputFile, string & outputFile)
{      
  ifstream in_file;
  in_file.open(  inputFile, ios::binary );
  ofstream out_stream;
  out_stream.open(outputFile, std::ios_base::app);
      
  char charblock[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint chars_read;
  while( chars_read = in_file.read( charblock, 8  ).gcount() )
  {
    for( int i = 8 - chars_read; i > 0; i-- )
     {
       charblock[ 8 - i ] = 0;
     }
    
    bitset< 8 > in_bits[ 8 ];
    for( int i = 0; i < 8; i++ )
    {
      in_bits[ i ] = bitset< 8 >( charblock[ i ] );
    } 
     bitset< 64 > bit_block( in_bits[ 0 ].to_string() + in_bits[ 1 ].to_string()
        + in_bits[ 2 ].to_string() + in_bits[ 3 ].to_string() + 
        in_bits[ 4 ].to_string() + in_bits[ 5 ].to_string() +
         in_bits[ 6 ].to_string() + in_bits[ 7 ].to_string() );

      //initial permutation
      bitset< 64 > permuted_bit_block = ip( bit_block );
      
      //Splitting the key
      bitset< 28 > left_key( 0 );
      bitset< 28 > right_key( 0 );
      per_split( key, left_key, right_key );
    
      //Splitting the plaintext into left and right chunks
      bitset< 32 > left_text( 0 );
      bitset< 32 > right_text( 0 );
      split_text( permuted_bit_block, left_text, right_text );
      
      for( uint r = 0; r < num_rounds; r++ )
      {
        //Permute & contract key to 48 bits
        bitset< 48 > shifted_key = shift_per( left_key, right_key, r );
        
        //Expand the right chunk
        bitset< 48 > expanded_right = expansion_box( right_text );
      
        //XOR permuted, contracted key with expanded right
        bitset< 48 > xored_right = expanded_right ^ shifted_key;

        //Put the result in the S-box
        bitset< 32 > substituted_right_text = substitution_box( xored_right );
       
        //Do the final permutation
        bitset< 32 > permuted_right = other_permutation( 
          substituted_right_text );
    
        //XOR dis with the left
        bitset< 32 > final_right = permuted_right ^ left_text;  

        left_text = right_text;
        right_text = final_right;      

      }
      
      //32 bit swap
      bitset< 32 > temp = right_text;
      right_text = left_text;
      left_text = temp;
      
      //Inverse Permutation
      bitset< 64 > cipher_bits( 0 );
      concat_text( cipher_bits, left_text, right_text );
      cipher_bits = ip_inv( cipher_bits );
      
      //Output the ciphertext
      bitset< 8 > out_bits[ 8 ];
      for( int i = 7; i >= 0; i-- )
      {
        for( uint j = 0; j < 8; j++ )
        {
          out_bits[ i ][ j ] = cipher_bits[ i * 8 + j ];
        }
       // if( chars_read > ( 8 - i ) ) //If char is not null
          out_stream << char( out_bits[ i ].to_ulong() );
      }
    }
     out_stream.close();
  }

string encrypt( string & plaintext)
{      
  stringstream in_file;
  in_file << plaintext;

  string ciphertext = "";
  char charblock[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint chars_read;
  while( chars_read = in_file.read( charblock, 8  ).gcount() )
  {
    for( int i = 8 - chars_read; i > 0; i-- )
     {
       charblock[ 8 - i ] = 0;
     }
    
    bitset< 8 > in_bits[ 8 ];
    for( int i = 0; i < 8; i++ )
    {
      in_bits[ i ] = bitset< 8 >( charblock[ i ] );
    } 
     bitset< 64 > bit_block( in_bits[ 0 ].to_string() + in_bits[ 1 ].to_string()
        + in_bits[ 2 ].to_string() + in_bits[ 3 ].to_string() + 
        in_bits[ 4 ].to_string() + in_bits[ 5 ].to_string() +
         in_bits[ 6 ].to_string() + in_bits[ 7 ].to_string() );

      //initial permutation
      bitset< 64 > permuted_bit_block = ip( bit_block );
      
      //Splitting the key
      bitset< 28 > left_key( 0 );
      bitset< 28 > right_key( 0 );
      per_split( key, left_key, right_key );
    
      //Splitting the plaintext into left and right chunks
      bitset< 32 > left_text( 0 );
      bitset< 32 > right_text( 0 );
      split_text( permuted_bit_block, left_text, right_text );
      
      for( uint r = 0; r < num_rounds; r++ )
      {
        //Permute & contract key to 48 bits
        bitset< 48 > shifted_key = shift_per( left_key, right_key, r );
        
        //Expand the right chunk
        bitset< 48 > expanded_right = expansion_box( right_text );
      
        //XOR permuted, contracted key with expanded right
        bitset< 48 > xored_right = expanded_right ^ shifted_key;

        //Put the result in the S-box
        bitset< 32 > substituted_right_text = substitution_box( xored_right );
       
        //Do the final permutation
        bitset< 32 > permuted_right = other_permutation( 
          substituted_right_text );
    
        //XOR dis with the left
        bitset< 32 > final_right = permuted_right ^ left_text;  

        left_text = right_text;
        right_text = final_right;      

      }
      
      //32 bit swap
      bitset< 32 > temp = right_text;
      right_text = left_text;
      left_text = temp;
      
      //Inverse Permutation
      bitset< 64 > cipher_bits( 0 );
      concat_text( cipher_bits, left_text, right_text );
      cipher_bits = ip_inv( cipher_bits );
      
      //Output the ciphertext
      bitset< 8 > out_bits[ 8 ];
      for( int i = 7; i >= 0; i-- )
      {
        for( uint j = 0; j < 8; j++ )
        {
          out_bits[ i ][ j ] = cipher_bits[ i * 8 + j ];
        }
       // if( chars_read > ( 8 - i ) ) //If char is not null
          ciphertext += char( out_bits[ i ].to_ulong() );
      }
    }
    return ciphertext;
  }
  
  void decrypt( string & inputFile, string & outputFile)
  {

  ifstream in_file;
  in_file.open(  inputFile, ios::binary );
  ofstream out_stream;
  out_stream.open(outputFile, std::ios_base::app);
      
  char charblock[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint chars_read;
  while( chars_read = in_file.read( charblock, 8  ).gcount() )
  {
    for( int i = 8 - chars_read; i > 0; i-- )
     {
       charblock[ 8 - i ] = 0;
     }
    
    bitset< 8 > in_bits[ 8 ];
    for( int i = 0; i < 8; i++ )
    {
      in_bits[ i ] = bitset< 8 >( charblock[ i ] );
    } 
     bitset< 64 > bit_block( in_bits[ 0 ].to_string() + in_bits[ 1 ].to_string()
        + in_bits[ 2 ].to_string() + in_bits[ 3 ].to_string() + 
        in_bits[ 4 ].to_string() + in_bits[ 5 ].to_string() +
         in_bits[ 6 ].to_string() + in_bits[ 7 ].to_string() );

      //initial permutation
      bitset< 64 > permuted_bit_block = ip( bit_block );
    
      //Splitting the key
      bitset< 28 > left_key( 0 );
      bitset< 28 > right_key( 0 );
      per_split( key, left_key, right_key );
    
      //Splitting the plaintext into left and right chunks
      bitset< 32 > left_text( 0 );
      bitset< 32 > right_text( 0 );
      split_text( permuted_bit_block, left_text, right_text ); 
      
      vector< bitset< 48 > > dec_keys;
      
      //per_split( key, left_key, right_key );
        
      for( uint r = 0; r < num_rounds; r++ )
      {
        bitset< 48 > newkey = shift_per( left_key, right_key, r  );
        dec_keys.push_back( newkey );
      }
      
      for( int r = num_rounds - 1; r >= 0; r-- )
      {//Permute & contract key to 48 bits
        bitset< 48 > shifted_key = dec_keys.at( r );
        
        //Expand the right chunk
        bitset< 48 > expanded_right = expansion_box( right_text );
       
        //XOR permuted, contracted key with expanded right
        bitset< 48 > xored_right = expanded_right ^ shifted_key;
  
        //Put the result in the S-box
        bitset< 32 > substituted_right_text = substitution_box( xored_right );

        //Do the final permutation
        bitset< 32 > permuted_right = other_permutation( 
          substituted_right_text );
	
        //XOR dis with da left haff
        bitset< 32 > final_right = permuted_right ^ left_text;  
       
        left_text = right_text;
        right_text = final_right;      
      }
      
      //32 bit swap
      bitset< 32 > temp = right_text;
      right_text = left_text;
      left_text = temp;
   
      //Inverse Permutation
      bitset< 64 > cipher_bits( 0 );
      concat_text( cipher_bits, left_text, right_text );
      cipher_bits = ip_inv( cipher_bits );

      //Output the plaintext 
      bitset< 8 > out_bits[ 8 ];
      for( int i = 7; i >= 0; i-- )
      {
        for( uint j = 0; j < 8; j++ )
        {
          out_bits[ i ][ j ] = cipher_bits[ i * 8 + j ];
        }
        if( out_bits[ i ] != 0 ) //If char is not null
          out_stream << char( out_bits[ i ].to_ulong() );
      }  
    }
    out_stream.close();
  
  }

string decrypt( string & ciphertext)
  {

  stringstream in_file;
  in_file << ciphertext;

  string plaintext = "";

  char charblock[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint chars_read;
  while( chars_read = in_file.read( charblock, 8  ).gcount() )
  {
    for( int i = 8 - chars_read; i > 0; i-- )
     {
       charblock[ 8 - i ] = 0;
     }
    
    bitset< 8 > in_bits[ 8 ];
    for( int i = 0; i < 8; i++ )
    {
      in_bits[ i ] = bitset< 8 >( charblock[ i ] );
    } 
     bitset< 64 > bit_block( in_bits[ 0 ].to_string() + in_bits[ 1 ].to_string()
        + in_bits[ 2 ].to_string() + in_bits[ 3 ].to_string() + 
        in_bits[ 4 ].to_string() + in_bits[ 5 ].to_string() +
         in_bits[ 6 ].to_string() + in_bits[ 7 ].to_string() );

      //initial permutation
      bitset< 64 > permuted_bit_block = ip( bit_block );
    
      //Splitting the key
      bitset< 28 > left_key( 0 );
      bitset< 28 > right_key( 0 );
      per_split( key, left_key, right_key );
    
      //Splitting the plaintext into left and right chunks
      bitset< 32 > left_text( 0 );
      bitset< 32 > right_text( 0 );
      split_text( permuted_bit_block, left_text, right_text ); 
      
      vector< bitset< 48 > > dec_keys;
      
      //per_split( key, left_key, right_key );
        
      for( uint r = 0; r < num_rounds; r++ )
      {
        bitset< 48 > newkey = shift_per( left_key, right_key, r  );
        dec_keys.push_back( newkey );
      }
      
      for( int r = num_rounds - 1; r >= 0; r-- )
      {//Permute & contract key to 48 bits
        bitset< 48 > shifted_key = dec_keys.at( r );
        
        //Expand the right chunk
        bitset< 48 > expanded_right = expansion_box( right_text );
       
        //XOR permuted, contracted key with expanded right
        bitset< 48 > xored_right = expanded_right ^ shifted_key;
  
        //Put the result in the S-box
        bitset< 32 > substituted_right_text = substitution_box( xored_right );

        //Do the final permutation
        bitset< 32 > permuted_right = other_permutation( 
          substituted_right_text );
	
        //XOR dis with da left haff
        bitset< 32 > final_right = permuted_right ^ left_text;  
       
        left_text = right_text;
        right_text = final_right;      
      }
      
      //32 bit swap
      bitset< 32 > temp = right_text;
      right_text = left_text;
      left_text = temp;
   
      //Inverse Permutation
      bitset< 64 > cipher_bits( 0 );
      concat_text( cipher_bits, left_text, right_text );
      cipher_bits = ip_inv( cipher_bits );

      //Output the plaintext 
      bitset< 8 > out_bits[ 8 ];
      for( int i = 7; i >= 0; i-- )
      {
        for( uint j = 0; j < 8; j++ )
        {
          out_bits[ i ][ j ] = cipher_bits[ i * 8 + j ];
        }
        if( out_bits[ i ] != 0 ) //If char is not null
          plaintext += char( out_bits[ i ].to_ulong() );
      }  
    }
    return plaintext;
  
  }
};
#endif
