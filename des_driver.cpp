#include "des.hpp"

using namespace std;
using namespace std::chrono; 

int main( int argc, char * argv[])
{
  /* The timing code was inspired by the following link:
   * https://stackoverflow.com/questions/9089842/c-chrono-system
   * -time-in-milliseconds-time-operations
   */
  time_point<std::chrono::system_clock> now = system_clock::now();
  auto duration = now.time_since_epoch();
  auto start_time = duration_cast
    <milliseconds>(duration).count();
    
  bool encrypt_mode;
  int num_rounds;
  string input_filename;
  string output_filename;
  uint64_t key_hex;
  
  if( argc != 6 )
  {
    cerr << "Usage: " << argv[ 0 ] << " <encrypt> " << "<# of rounds > " << 
      " <input filename> " << " <output filename> " << "<key>" << endl << endl;
      
    cerr << "<encrypt> 1 if you wish to encrypt, 0 for decryption" << endl;
    cerr << "<# of rounds> how many rounds you want done, from 1-16" << endl;
    cerr << "<input filename> the name of the input file" << endl;
    cerr << "<output filename> the name of the output file" << endl;
    cerr << "<key> key as 16 hex values " << endl << endl;    
    
    cerr << "EXAMPLE:" << endl <<
    "./des 0 16 cipher.txt plain.txt 1234567890123456" << endl
    << "decrypts cipher.txt with 16 rounds and the key 0x1234567890123456" 
    <<" and puts result in plain.txt" << endl;
    
    return 1;
  }
  
  encrypt_mode = atoi( argv[ 1 ] );
  num_rounds = atoi( argv[ 2 ] );
  input_filename = string( argv[ 3 ] );
  output_filename =string( argv[ 4 ] );
  string hex_string = string( argv[ 5 ] );
  
  if( !( num_rounds > 0 && num_rounds <= 16 ) )
  {
    cerr << "ERROR! Number of rounds must be between 1 and 16, inclusive."
     << endl;
    return 1;
  }
  
  /**
   * Convert the hex input to a bitset. Inspired by this Stack Overflow
   * thread
   * https://stackoverflow.com/questions/18354787/hex-input-to-bitset64-c
   */  
  istringstream ost( hex_string );
  ost >> hex >> key_hex;
  bitset<64> key( key_hex );
  
  des DES = des(key, 16);
  
  if( encrypt_mode ) DES.encrypt(input_filename, output_filename);
  else DES.decrypt(input_filename, output_filename );


  //Calculate and output the time taken to run the program
  now = system_clock::now();
  duration = now.time_since_epoch();
  auto end_time = duration_cast
    <milliseconds>(duration).count();
    
  cout << ( encrypt_mode ? "Encryption time: " : "Decryption time: " ) <<
    (end_time - start_time) << "ms" << endl;

  return 0; 
}
