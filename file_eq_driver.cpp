#include "file_eq.hpp"
#include <sys/socket.h>
int main( int argc, char * argv[])
{
  if( argc != 3 )
  {
    cerr << "Usage: " << argv[ 0 ] << " <file1_name>  <file2_name> " << endl;  
      
    return 1;
  }
  
  string fn1 = string( argv[ 1 ] );
  string fn2 = string( argv[ 2 ] );
  
  double similarity_perfect = file_eq( fn1, fn2 );
  double similarity_charnum = file_eq_by_char_num( fn1, fn2 );

  cout << "CHAR-BY-CHAR-SIM: " << similarity_perfect << endl;
  cout << "CHAR POPULATION SIM: " << similarity_charnum << endl;
  return 0;
}
