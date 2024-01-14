#include "counter.h"

void counter::count() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    //test
    test++;
    while(true) {
        if (!rst_n) {
            //wait(); //here, wait() will wait for extra a change of clk.pos or rst.neg
            cnt_int = 0;
            cnt.write(cnt_int);
            wait(); //wait for clk.pos or rst_n.neg
        }
        else {
//             test = 66;
            //wait();
            test = 88;
            cnt_int += 1;
            if (cnt_int==100) {
                cnt_int = 0;
            }
            cnt.write(cnt_int);
            wait();
        }
    }
	// ####################### UP TO HERE ####################### //
}
