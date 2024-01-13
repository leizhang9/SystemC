#include "consumer.h"
#include "producer.h"
#include "fifo_1b.h"

int sc_main(int argc, char *argv[]) {
	// the following instruction generates a clock signal with clock named
	// "clock" with a period of 100 ns, a duty cycle of 50%, and a falling edge
	// after 50 ns
	sc_clock clk("clock", 100, SC_NS, 0.5, 50, SC_NS, false);

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in the required commands to instantiate and connect producer, fifo,
	// and consumer
    sc_signal<unsigned char> d_in, d_out;
    sc_signal<bool> wr_en, full, rd_en, valid;
    
    producer pr1("producer");
    pr1.clk(clk);
    pr1.d_out(d_in);
    pr1.wr_en(wr_en);
    pr1.full(full);
    
    fifo_1b fi1("fifo_1b", 5);
    fi1.clk(clk);
    fi1.d_in(d_in);
    fi1.wr_en(wr_en);
    fi1.full(full);
    fi1.d_out(d_out);
    fi1.rd_en(rd_en);
    fi1.valid(valid); //
    
    consumer co1("consumer");
    co1.clk(clk);
    co1.d_in(d_out);
    co1.rd_en(rd_en);
    co1.valid(valid);  //
    
	// ####################### UP TO HERE ####################### //

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in code to generate traces that can be used to observe the
	// functionality of the model with the waveform viewer gtkwave
    sc_trace_file *tf = sc_create_vcd_trace_file("trace_file");
    sc_trace(tf, clk, "clock");
    sc_trace(tf, valid, "valid");
    sc_trace(tf, co1.fetch, "co1.fetch");
    sc_trace(tf, rd_en, "rd_en");
    sc_trace(tf, co1.data_valid, "co1.data_valid");
	sc_trace(tf, d_out, "d_in_for_consumer");
    
    sc_trace(tf, clk, "clock"); //not wrong, but not displayed
    sc_trace(tf, full, "full");
	sc_trace(tf, wr_en, "wr_en");
    sc_trace(tf, d_in, "d_out_for_producer"); //cannot contain space!!!!
	// ####################### UP TO HERE ####################### //

	sc_time sim_dur = sc_time(5000, SC_NS);
	if(argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	}
	else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
		cout << "Simulation time = " << sim_dur << endl;
	}

	// start simulation
	sc_start(sim_dur);

	sc_close_vcd_trace_file(tf);

	return 0;
}
