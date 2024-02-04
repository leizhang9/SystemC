#include "Cpu.h"
#include "IoModule.h"
#include "reporting.h"

using namespace sc_core;
using namespace tlm;

void Cpu::processor_thread(void) {

	while(true) {
		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// read new packet descriptor
        wait(packetReceived_interrupt.value_changed_event());
        
        std::vector<soc_address_t> address_list{
            0x20000000, 
            0x30000000, 
            0x40000000, 
            0x50000000};
		// ####################### UP TO HERE ####################### //

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// Forward the packet descriptor to an arbitrary port
        while(packetReceived_interrupt.read()) {
            //read descripter
            startTransaction(TLM_READ_COMMAND, 
                             0x10000000, 
                             (unsigned char*) &m_packet_descriptor, 
                             sizeof(m_packet_descriptor));
            // read header packet
            unsigned int sizeheader = sizeof(sc_time) + sizeof(uint64_t) + IpPacket::MINIMAL_IP_HEADER_LENGTH;
            startTransaction(TLM_READ_COMMAND, 
                             0x00000000 + m_packet_descriptor.baseAddress, 
                             (unsigned char*) &m_packet_header, 
                             sizeheader);
            
            if (!verifyHeaderIntegrity(m_packet_header)) {
                discardDescriptor();
            }
            else {
                unsigned int nextHop = makeNHLookup(m_packet_header);
                
                startTransaction(TLM_WRITE_COMMAND,
                                 address_list[nextHop],
                                 (unsigned char*) &m_packet_descriptor,
                                 sizeof(m_packet_descriptor));
                decrementTTL(m_packet_header);
                updateChecksum(m_packet_header);
                // write back to packet memory
                startTransaction(TLM_WRITE_COMMAND,
                                 0x00000000 + m_packet_descriptor.baseAddress,
                                 (unsigned char*) &m_packet_header,
                                 sizeheader);
            }
        }
		// ####################### UP TO HERE ####################### //
	}
}

// ############# COMPLETE THE FOLLOWING SECTION ############# //
// startTransaction
void Cpu::startTransaction(tlm_command command, soc_address_t address,
                            unsigned char *data, unsigned int dataSize) {
    tlm_phase phase;
    sc_time delay_time;
    do {
        delay_time = SC_ZERO_TIME;
        phase = BEGIN_REQ;
        tlm_sync_enum tlm_resp;
        payload.set_command(command);
        payload.set_address(address);
        payload.set_data_ptr(data);
        payload.set_data_length(dataSize);
        
        tlm_resp = initiator_socket->nb_transport_fw(payload, phase, delay_time);
        wait(transactionFinished_event); //2nd transaction phase finished
        
        if (tlm_resp != TLM_UPDATED) {
            std::cout << sc_time_stamp() << " error in Cpu::startTransaction" << std::endl;
            exit(1);
        }
        if (payload.get_response_status() == TLM_OK_RESPONSE) {
            break;
        }
    }while(true);
}

void Cpu::discardDescriptor() {
    startTransaction(TLM_WRITE_COMMAND,
                    0x10000000,
                    (unsigned char*) &m_packet_descriptor,
                    sizeof(m_packet_descriptor));
}
// ####################### UP TO HERE ####################### //

// nb_transport_bw: implementation of the backward path callback
tlm_sync_enum Cpu::nb_transport_bw(tlm_generic_payload& transaction,
		tlm_phase& phase, sc_time& delay_time) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
    //check whether the phase is set appropriately
    if (phase != BEGIN_RESP) {
        std::cerr << "expected BEGIN_RESP but not got" << std::endl;
        exit(1);
    }
    delay_time += sc_time(CLK_CYCLE_BUS);
    transactionFinished_event.notify(delay_time);
    phase = END_RESP;
    return TLM_COMPLETED;
	// ####################### UP TO HERE ####################### //
}

unsigned int Cpu::instances = 0;
