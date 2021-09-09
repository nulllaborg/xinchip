/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

#define __BTSTACK_FILE__ "hci_transport_ram.c"


#include <inttypes.h>
#include "btstack_config.h"
#include "btstack_debug.h"
#include "hci.h"
#include "hci_transport.h"
#include "bluetooth_company_id.h"
#include "btstack_run_loop.h"
#include "btstack_uart_block.h"
#include "btstack_run_loop_embedded.h"

typedef enum {
    RX_IDLE = 1,
    RX_DONE
} RX_STATE;

typedef enum {
    TX_IDLE = 1,
    TX_DONE
} TX_STATE;

// write state
static TX_STATE tx_state; 
// packet reader state machine
static RX_STATE rx_state;
static uint16_t rx_size;

static void dummy_handler(uint8_t packet_type, uint8_t *packet, uint16_t size) { }
static uint8_t packet_sent_event[] = { HCI_EVENT_TRANSPORT_PACKET_SENT, 0};
static void (*packet_handler)(uint8_t packet_type, uint8_t *packet, uint16_t size) = dummy_handler;
extern unsigned char *TRAhcit_Generic_Get_Rx_Buf(uint8_t ptype, int size, uint8_t *hci_header);
extern void TRAhcit_Generic_Commit_Rx_Buf(unsigned char buffer_type); 	
extern uint8_t TRAhcit_Register_Generic_Dispatcher(uint32_t (*callback)(uint8_t *data, uint32_t pdu_len, uint8_t *header, uint8_t head_len));

// incoming packet buffer
static uint8_t hci_packet_with_pre_buffer[HCI_INCOMING_PRE_BUFFER_SIZE + HCI_INCOMING_PACKET_BUFFER_SIZE + 1]; // packet type + max(acl header + acl payload, event header + event data)
static uint8_t * hci_packet = &hci_packet_with_pre_buffer[HCI_INCOMING_PRE_BUFFER_SIZE];

// data source for integration with BTstack Runloop
static btstack_data_source_t transport_data_source;

static void btstack_embedded_process(btstack_data_source_t *ds, btstack_data_source_callback_type_t callback_type) {
    
    if(callback_type != DATA_SOURCE_CALLBACK_POLL) return;
    
    if (tx_state == TX_DONE) {
        tx_state = TX_IDLE;
        packet_handler(HCI_EVENT_PACKET, &packet_sent_event[0], sizeof(packet_sent_event));
    }
    if (rx_state != RX_DONE) return;

    packet_handler(hci_packet[0], &hci_packet[1], rx_size-1);
    rx_state = RX_IDLE;                              
    return;
}

static uint32_t hci_transport_ram_receive_packet(uint8_t *data, uint32_t pdu_len, uint8_t *header, uint8_t head_len){

    if(rx_state == RX_DONE) {     
        log_error("\n%s error h:", __func__);
        for(uint16_t i=0; i<head_len; i++) {
           log_error("%x ", header[i]); 
        }
        log_error("d:");
        for(uint16_t i=0; i<pdu_len; i++) {
           log_error("%x ", data[i]); 
        }
        log_error("\n");		
		return 0;
    }
 
	memcpy(hci_packet, header, head_len);
	memcpy(hci_packet+head_len, data, pdu_len);

    rx_size = head_len + pdu_len;
	rx_state = RX_DONE;	
	return 1;
}

static int hci_transport_ram_open(void){

    tx_state = TX_IDLE;
    rx_state = RX_IDLE;   

    // set up polling data_source
    btstack_run_loop_set_data_source_handler(&transport_data_source, btstack_embedded_process);
    btstack_run_loop_enable_data_source_callbacks(&transport_data_source, DATA_SOURCE_CALLBACK_POLL);
    btstack_run_loop_add_data_source(&transport_data_source);

    TRAhcit_Register_Generic_Dispatcher(hci_transport_ram_receive_packet);
    return 0;
}

static int hci_transport_ram_close(void){
    // remove data source
    btstack_run_loop_disable_data_source_callbacks(&transport_data_source, DATA_SOURCE_CALLBACK_POLL);
    btstack_run_loop_remove_data_source(&transport_data_source);
    
    return 0;
}

static void hci_transport_ram_register_packet_handler(void (*handler)(uint8_t packet_type, uint8_t *packet, uint16_t size)){
    packet_handler = handler;
}

static int hci_transport_ram_can_send_now(uint8_t packet_type){
    return tx_state == TX_IDLE;
}

static int hci_transport_ram_send_packet(uint8_t packet_type, uint8_t * packet, int size){

    // write in progress
    if (tx_state != TX_IDLE) {
        log_error("%s with tx_state = %u, type %u, data %02x %02x %02x\n", __func__, tx_state, packet_type, packet[0], packet[1], packet[2]);
        return -1;
    }

    int  nsize;
    unsigned char *rx_packet = (unsigned char*)0;
    
    if(packet_type == 1) {            /*CMD Packet*/
      nsize = size;
    }
    else if(packet_type == 2) {       /*ACL Packet*/
      nsize = size - 4;
    }
    rx_packet = TRAhcit_Generic_Get_Rx_Buf(packet_type, nsize, packet);

    if(rx_packet == 0) {
        log_error("%s fatal error! \n" ,__func__);
        return 0;
    }
    if(packet_type == 1) {            /*CMD Packet*/    
        memcpy(rx_packet , (packet+3) , (nsize-3));
    } else if(packet_type == 2) {     /*ACL Packet*/
        memcpy(rx_packet , (packet+4) , nsize);
    }
    TRAhcit_Generic_Commit_Rx_Buf(packet_type);
    tx_state = TX_DONE;

    return 0;
}

static const hci_transport_t hci_transport_ram = {
    /* const char * name; */                                        "ram",
    /* void   (*init) (const void *transport_config); */            NULL,
    /* int    (*open)(void); */                                     &hci_transport_ram_open,
    /* int    (*close)(void); */                                    &hci_transport_ram_close,
    /* void   (*register_packet_handler)(void (*handler)(...); */   &hci_transport_ram_register_packet_handler,
    /* int    (*can_send_packet_now)(uint8_t packet_type); */       &hci_transport_ram_can_send_now,
    /* int    (*send_packet)(...); */                               &hci_transport_ram_send_packet,
    /* int    (*set_baudrate)(uint32_t baudrate); */                NULL,
    /* void   (*reset_link)(void); */                               NULL,
    /* void   (*set_sco_config)(uint16_t voice_setting, int num_connections); */ NULL, 
};

const hci_transport_t * hci_transport_ram_instance(void) {

    return &hci_transport_ram;
}
