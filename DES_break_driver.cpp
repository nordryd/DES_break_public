
#include <iostream>
#include "DES_break.hpp"
#include <cmath>

using namespace std;



int main()
{
   uint64_t realkey_num = uint64_t(6969);
   uint64_t max = uint64_t(1000000);
   bitset<64>realkey(realkey_num);
   string pt = "Hey Jaiswal what up that's great that's lit that's super duper lit how is life good wow god I hate this class";
   string ct = des_encrypt( pt, realkey );
   DESBreakThread dbt(5, 0, max , pt, ct );
   dbt.crack();
   bitset<64>key =dbt.get_key();
 
   cout << realkey << endl << realkey.to_ullong() << endl << key << endl << key.to_ullong()<< endl;
   cout << ct << endl << des_encrypt( pt, key ) << endl << endl << endl;
   cout << pt << endl << des_decrypt( ct, key ) << endl;
   return 0;
}
