#include <iomanip>
#include "fifo_1a.h"

fifo_1a::fifo_1a(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register processes
    SC_THREAD(write_fifo);
    sensitive<<clk.pos();
    
    SC_THREAD(read_fifo);
    sensitive<<clk.pos();
	// ####################### UP TO HERE ####################### //

	SC_METHOD(set_flags);
	sensitive << rd_ptr << wr_ptr;

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports
    full.initialize(false);
    empty.initialize(true);
    d_out.initialize(-1); 
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    while(true) {
        wait();
        if (wr_en.read() && !full.read()) {  
            *(fifo_data + wr_ptr) = d_in.read(); //fifo_data is unsigned char *
            std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" write: "<<(int)d_in.read()<<" at wr_ptr "<<wr_ptr<<" fill_level is: "<<fill_level<<std::endl;
            wr_ptr = (wr_ptr + 1)%fifo_size;
            fill_level++;
        }
    }
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::read_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    while(true) {
        wait();
        if (rd_en.read() && !empty.read()) {
            d_out = *(fifo_data+rd_ptr);
            std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" read: "<<(int)d_out<<" at rd_ptr "<<rd_ptr<<" fill_level is: "<<fill_level<<std::endl;
            rd_ptr = (rd_ptr + 1)%fifo_size;
            fill_level--;
        }
    }
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::set_flags() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// set 'full' & 'empty' flags according to fill level
    if (fill_level == fifo_size) {
        full.write(true);
    }
    else {
        full.write(false);
    }
    
    if (fill_level == 0) {
        empty.write(true);
    }
    else{
        empty.write(false);
    }
    
	// ####################### UP TO HERE ####################### //
}
