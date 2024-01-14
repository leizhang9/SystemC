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
    d_out.initialize(-1);
    valid.initialize(false);
	// ####################### UP TO HERE ####################### //
}

void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    bool write = wr_en.read() && !full.read();
    bool read = rd_en.read() && valid.read();
    bool write_through = (fill_level == 0 && write);
    bool to_update_output = false;
    
    unsigned char output_register;
    while(true) {
        wait();
        
        write = wr_en.read() && !full.read();
        read = rd_en.read() && valid.read();
        write_through = (fill_level == 0 && write); //
        
        if (write_through) {
            output_register = d_in.read();
            d_out.write(output_register);
            valid = true;
            fill_level++;  //
            to_update_output = true;
            {
            std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" write through: "<<(int)d_in.read()<<" at wr_ptr "<<wr_ptr<<" fill_level is: "<<fill_level<<std::endl;
            }
        }
        if (write && !write_through) {
                *(fifo_data + wr_ptr) = d_in.read();
                wr_ptr = (wr_ptr + 1)%fifo_size;
                fill_level++;
        }
        if (read) {
            if (to_update_output) {
                to_update_output = false;
                fill_level--;
                output_register = *(fifo_data);
                d_out.write(output_register);
                std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" read through: "<<(int)d_out.read()<<" at output "<<" fill_level is: "<<fill_level<<std::endl;
                //
                if (fill_level > 1 ) {
                    for (unsigned int i = 0; i++; i < fill_level) {
                        *(fifo_data + i) = *(fifo_data + i);
                    }
                    wr_ptr = (wr_ptr - 1)%fifo_size;
                    //rd_ptr = (rd_ptr - 1)%fifo_size; //
                    to_update_output = true;  //
                    std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<"shift happens"<<endl;
                }
            }
            else if (fill_level >0) { //0
                output_register = *(fifo_data + rd_ptr);
                d_out.write(output_register);
                rd_ptr.write((rd_ptr.read() + 1)%fifo_size);
                fill_level--;
                std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" read normally: "<<(int)d_out.read()<<" at rd_ptr "<<rd_ptr<<" fill_level is: "<<fill_level<<std::endl;
            }
        }
        
         if (fill_level <= 0 ) {
            valid.write(false);
        }
        else {
            valid.write(true);
        }
        
        full.write(fill_level == (fifo_size+1));
        
        if (write && !write_through) {
            std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" write: "<<(int)d_in.read()<<" at wr_ptr "<<wr_ptr<<" fill_level is: "<<fill_level<<std::endl;
        }
//         if(read) {
//             std::cout<<std::setw(9)<<sc_time_stamp()<< name()<<" read: "<<(int)d_out.read()<<" at rd_ptr "<<rd_ptr<<" fill_level is: "<<fill_level<<std::endl;
//         }
    }
	// ####################### UP TO HERE ####################### //
}
