#include "bcd_decoder.h"

void bcd_decoder::decode() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    unsigned short int val_int;
    char hi_int, lo_int;
    val_int = val.read();
    hi_int = val_int/10;
    lo_int = val_int%10;
    hi.write(hi_int);
    lo.write(lo_int);
    
	// ####################### UP TO HERE ####################### //
}
