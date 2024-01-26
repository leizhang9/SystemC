#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register process
    SC_METHOD(read_write_fifo);
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
    d_out.initialize(-1);
    valid.initialize(false);
	// ####################### UP TO HERE ####################### //
}

void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	if(wr_en.read() && !full.read()){
		unsigned char data = d_in.read();
		if(fill_level == 0 && !valid.read()){ //write_through
			d_out.write(data);
			valid.write(true);
		}
		else{   // normal write
			*(fifo_data + wr_ptr.read()) = data;
			wr_ptr.write((wr_ptr.read()+1) % fifo_size);
			fill_level++;
			if(fill_level == fifo_size) 
                full.write(true);
		}
	}
	
	if(rd_en.read() && valid.read()){
		if(fill_level == 0)
        {
			valid.write(false);
        }
		else{
			unsigned char data = *(fifo_data + rd_ptr.read());
			rd_ptr.write((rd_ptr.read()+1) % fifo_size);
			d_out.write(data);
			valid.write(true);
			fill_level--;
			full.write(false);
		}
	}
	// ####################### UP TO HERE ####################### //
}
