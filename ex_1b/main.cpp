#include "stimulus.h"
#include "counter.h"
#include "bcd_decoder.h"

int sc_main(int argc, char *argv[]) {

	sc_signal<bool> clock, reset_n;
	sc_signal<unsigned short int> count_val;
	sc_signal<char> v_hi, v_lo;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    stimulus st1("stimulus");
    st1.rst_n(reset_n);
    st1.clk(clock);
    
    counter co1("counter");
    co1.rst_n(reset_n);
    co1.clk(clock);
    co1.cnt(count_val);
    
    bcd_decoder bc1("bcd_decoder");
    bc1.val(count_val);
    bc1.hi(v_hi);
    bc1.lo(v_lo);
    
    sc_trace_file *tf = sc_create_vcd_trace_file("trace_file");
	sc_trace(tf, clock, "clock");
	sc_trace(tf, reset_n, "reset_n");
    sc_trace(tf, count_val, "count_val");
    sc_trace(tf, v_hi, "v_hi");
    sc_trace(tf, v_lo, "v_lo");
	sc_trace(tf, co1.test, "co1.test");
    
	// ####################### UP TO HERE ####################### //

	int n_cycles;
	if(argc != 2) {
		cout << "default n_cycles = 3000" << endl;  //ge36cig change from 2000 to 3000
		n_cycles = 3000;    //ge36cig change from 2000 to 3000
	}
	else {
		n_cycles = atoi(argv[1]);
		cout << "n_cycles = " << n_cycles << endl;
	}

	sc_start(n_cycles, SC_NS);

	sc_close_vcd_trace_file(tf);

	return 0;
}
