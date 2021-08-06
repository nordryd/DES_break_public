#include<iostream>
#include<string>
#include<fstream>
#include<map>
#ifndef FILE_EQ_HPP
#define FILE_EQ_HPP

using namespace std;

double file_eq( string & file1_name, string & file2_name )
{
  double num_similar = 0;
  double total_chars = 0;
 
  ifstream fs1;
  ifstream fs2;
 
  fs1.open( file1_name, ios::binary );
  fs2.open( file2_name, ios::binary );
  
  char buff1[1];
  char buff2[1];
  while( fs1.read( buff1, 1 ) && fs2.read( buff2, 1 ) )
  {
    if( buff1[0] == buff2[0] ) num_similar += 1.0;
    total_chars += 1.0;
  }
  return num_similar / total_chars;
}

double file_eq_by_char_num( string & file1_name, string & file2_name )
{
  map< char, int > m1;
  map< char, int > m2;
  ifstream fs1;
  ifstream fs2;
 
  fs1.open( file1_name, ios::binary );
  fs2.open( file2_name, ios::binary );
  
  char buff[1];
  while( fs1.read( buff, 1 ) )
  {
    m1[ buff[ 0 ] ]++;
  }
  while( fs2.read( buff, 1 ) )
  {
    m2[ buff[ 0 ] ]++;
  }
  
  double charnums_sim = 0;
  double total_chars = 0;
  for( auto pair : m1 )
  {
    charnums_sim += min( m1[ pair.first ], m2[ pair.first ] );
    total_chars += max( m1[ pair.first ], m2[ pair.first ] );
  }
  return charnums_sim / total_chars;
}


double str_sim( string & s1, string & s2 )
{
  map< char, int > m1;
  map< char, int > m2;

  for( char n : s1 )
  {
    m1[ n ]++;
  }
  for( char n : s2 )
  {
    m2[ n ]++;
  }
  
  double charnums_sim = 0;
  double total_chars = 0;
  if(m1.size() > m2.size() )
  {
    for( auto pair : m1 )
    {
      charnums_sim += min( m1[ pair.first ], m2[ pair.first ] );
      total_chars += max( m1[ pair.first ], m2[ pair.first ] );
    }
  }
  else
  {
    for( auto pair : m2 )
    {
      charnums_sim += min( m1[ pair.first ], m2[ pair.first ] );
      total_chars += max( m1[ pair.first ], m2[ pair.first ] );
    } 
  }
  return charnums_sim / total_chars;
}

#endif
