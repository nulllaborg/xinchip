
#include "btstack_run_loop_embedded.h"
#include "btstack.h"

typedef struct {
    uint16_t (*att_read_callback)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size);
    int (*att_write_callback)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
    void (*packet_handler) (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
    const uint8_t *ble_device_db;
}blestack_init_t;


static btstack_packet_callback_registration_t hci_event_callback_registration;
extern const hci_transport_t * hci_transport_ram_instance(void);

static btstack_packet_handler_t  att_handler = (btstack_packet_handler_t)0;
static void att_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    if(packet_type != HCI_EVENT_PACKET) return;
    if(att_handler == (btstack_packet_handler_t)0) return;
    
    if((packet[0] == ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE) ||
       (packet[0] == ATT_EVENT_MTU_EXCHANGE_COMPLETE) ||
       (packet[0] == ATT_EVENT_CAN_SEND_NOW)) {  
        att_handler(HCI_EVENT_PACKET, channel, packet, size);
    }        
}

void    ble_stack_init(void *init)
{
	// start with BTstack init - especially configure HCI Transport
    btstack_memory_init();
    btstack_run_loop_init(btstack_run_loop_embedded_get_instance());
    hci_dump_open((const char *)1, HCI_DUMP_STDOUT);
    
    // init HCI
    hci_init(hci_transport_ram_instance(), (void*)0);

    blestack_init_t *btstack_init = (blestack_init_t *)init;
    // inform about BTstack state
    hci_event_callback_registration.callback = btstack_init->packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    l2cap_init();
    l2cap_register_packet_handler(btstack_init->packet_handler);
    
    // setup le device db
    le_device_db_init();

    // setup SM: Display only
    sm_init();
	sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    sm_set_authentication_requirements(SM_AUTHREQ_BONDING);

    // setup ATT server
    att_server_init(btstack_init->ble_device_db, 
                    btstack_init->att_read_callback, 
                    btstack_init->att_write_callback);                    
    att_handler = btstack_init->packet_handler;    
    att_server_register_packet_handler(att_packet_handler);
    
    // turn on!
	hci_power_control(HCI_POWER_ON);
}

void    ble_stack_schedule(void)
{
    btstack_run_loop_execute();
}
