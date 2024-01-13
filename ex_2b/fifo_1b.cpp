#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register process
    SC_THREAD(read_write_fifo);
    sensitive<<clk.pos();
	// ####################### UP TO HERE ####################### //

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports
    full.initialize(false);
    d_out.initialize(0);
    valid.initialize(false);
	// ####################### UP TO HERE ####################### //
}

void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    bool write_through = 0;
    while(true) {
        if (fill_level == 0) {
            write_through = 1;
        }
        else {
            
        }
    }
	// ####################### UP TO HERE ####################### //
}
