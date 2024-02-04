#include "consumer.h"
#include "producer.h"
#include "fifo_3.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]) {

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// instances of the modules
    producer pr1("producer");
    fifo_3 fi1("fifo_3", 50);  //with parameter
    consumer co1("consumer");
    pr1.prod2fifo_socket.bind(fi1.fifo2prod_socket);
    co1.consum2fifo_socket.bind(fi1.fifo2consum_socket);
	// ####################### UP TO HERE ####################### //

	// simulation duration may be modified by command line parameter
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

	return (0);
}
