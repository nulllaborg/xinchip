#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "profile.h"
#include "ble.h"
#include "btstack_defines.h"
#include "peripheral.h"
#include "HardwareSerial.h"

static btstack_context_callback_registration_t  user_data_callback;
static hci_con_handle_t user_data_client_configuration_connection;

static uint16_t user_data_client_configuration;
uint8_t *p_buff = NULL;
uint16_t u_buff_length=0;
uint8_t notify_flag = 0;

extern int att_server_register_can_send_now_callback(btstack_context_callback_registration_t * callback_registration, hci_con_handle_t con_handle);
extern uint8_t gatt_client_read_value_of_characteristics_by_uuid16(btstack_packet_handler_t callback, hci_con_handle_t con_handle, uint16_t start_handle, uint16_t end_handle, uint16_t uuid16);
extern void gatt_client_init(void);


void _HWradio_Go_To_Idle_State_Patch (void){

}

BLE_STATE g_conn_stat = BLE_STATE_IDLE;

BLE_STATE peripheral_connect_state(void)
{
    return g_conn_stat;
}

void peripheral_set_connect_state(BLE_STATE state)
{
    g_conn_stat = state;
}

// att_read_callback helpers
static uint16_t att_read_callback_handle_blob(const uint8_t * blob, uint16_t blob_size, uint16_t offset, uint8_t * buffer, uint16_t buffer_size){
    if (buffer){
        uint16_t bytes_to_copy = min(blob_size - offset, buffer_size);
        memcpy(buffer, &blob[offset], bytes_to_copy);
        return bytes_to_copy;
    } else {
        return blob_size;
    }
}

// read requests
static uint16_t att_read_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size){

   // printf("%s, con_handle=%x, att_handle=%x, offset=%x, buffer=%x, size=%x\n",__func__, con_handle, att_handle, offset,(uint32_t)buffer, buffer_size);
		switch (att_handle) {
			
			case ATT_CHARACTERISTIC_0000FFE1_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
					return att_read_callback_handle_blob((const uint8_t *)"Char0 Bluetooth Bypass", 22, offset, buffer, buffer_size);
					//break;
			case ATT_CHARACTERISTIC_0000FFE2_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
				  return att_read_callback_handle_blob((const uint8_t *)"Char1 Set AT Command", 20, offset, buffer, buffer_size);
					//break;
		}
		return 0;
    
}

// write requests
static int att_write_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size){
    
    uint32_t le_notification_enabled;
    // printf("%s, con_handle=%x, att_handle=%x, offset=%x, buffer=%x, size=%x\n", __func__, con_handle, att_handle, offset, (uint32_t)buffer, buffer_size);  
    if (transaction_mode != ATT_TRANSACTION_MODE_NONE) return 0;
    switch(att_handle)
    {
    case ATT_CHARACTERISTIC_0000FFE1_0000_1000_8000_00805F9B34FB_01_CLIENT_CONFIGURATION_HANDLE:
        le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
        Serial1.printf("Notifications enabled %u\n",le_notification_enabled);
				user_data_client_configuration = le_notification_enabled;
		    user_data_client_configuration_connection = con_handle;
        break;   
    case ATT_CHARACTERISTIC_0000FFE1_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
    case ATT_CHARACTERISTIC_0000FFE2_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        Serial1.write_buf(buffer, buffer_size);
        break;
    default:
        break;
    }    
    return 0;
}

static void data_can_send_now(void * context){
	hci_con_handle_t con_handle = (hci_con_handle_t) (int) context;
	if(u_buff_length>20 && p_buff!=NULL)
	{
	    att_server_notify(con_handle, ATT_CHARACTERISTIC_0000FFE1_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, p_buff, 20);
		p_buff += 20;
		u_buff_length -= 20;
		if (user_data_client_configuration){
			user_data_callback.callback = &data_can_send_now;
			user_data_callback.context  = (void*) (int) user_data_client_configuration_connection;
			att_server_register_can_send_now_callback(&user_data_callback, user_data_client_configuration_connection);
		}
	}else{
		att_server_notify(con_handle, ATT_CHARACTERISTIC_0000FFE1_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, p_buff, u_buff_length);
		p_buff = NULL;
		u_buff_length = 0;
	}
}

void peripheral_send(uint8_t *buffer, uint16_t length)
{
	p_buff = buffer;
	u_buff_length = length;
	if (user_data_client_configuration){
		user_data_callback.callback = &data_can_send_now;
		user_data_callback.context  = (void*) (int) user_data_client_configuration_connection;
		att_server_register_can_send_now_callback(&user_data_callback, user_data_client_configuration_connection);
	}
}

static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    if (packet_type != HCI_EVENT_PACKET) return;
    switch(hci_event_packet_get_type(packet))
    {
    case BTSTACK_EVENT_STATE:
        if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
        //printf("BTstack up and running.\n");
        break;
        
    case HCI_EVENT_LE_META:
        switch (hci_event_le_meta_get_subevent_code(packet)) 
        {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE: {
                hci_con_handle_t connection_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
                //printf("\n CONNECT RIGHT ! (HANDLE = 0x%x)\n", connection_handle);
                peripheral_set_connect_state(BLE_STATE_CONNECTED);
                //gap_request_connection_parameter_update(connection_handle, 12, 12, 0, 0x0048);
                //printf("Connected, requesting conn param update for handle 0x%04x\n", connection_handle);
					      gatt_client_read_value_of_characteristics_by_uuid16(packet_handler,connection_handle,0x01,0xff,0x2A00);
            }
            break;
        default:
            break;
        }
        break;
            
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        printf("\n DISCONNECT ! (HANDLE = 0x%x) ", hci_event_disconnection_complete_get_connection_handle(packet)); 
        printf("REASON = 0x%x\n", hci_event_disconnection_complete_get_reason(packet));
        peripheral_set_connect_state(BLE_STATE_IDLE);
        break;
    
    case L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE:   
        printf("L2CAP Connection Parameter Update Complete, response: %x\n", l2cap_event_connection_parameter_update_response_get_result(packet));
        break;

    case ATT_EVENT_MTU_EXCHANGE_COMPLETE:
        printf("ATT MTU = %u\n", att_event_mtu_exchange_complete_get_MTU(packet));
        break;
    
    case ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE:
        (att_event_handle_value_indication_complete_get_status(packet) == 0) ? printf("INDICATION OK\n") : printf("INDICATION FAIL\n");
        break;
    default:
        break;
    }
}

static const blestack_init_t blestack_init = {
    
    /* uint16_t (*att_read_callback)(...);*/ att_read_callback,
    /* int (*att_write_callback)(...);    */ att_write_callback,
    /* void (*packet_handler)(...)        */ packet_handler,
    /* const uint8_t *ble_device_db       */ profile_data
};

uint8_t adv_data[] = {
    // Flags general discoverable, BR/EDR not supported
    0x02, 0x01, 0x06, 
    // Name
    0x0A, 0x09, /* device name'X', 'i', 'c', '_', 'B', 'L', 'E', 'v', '1', */
};

void peripheral_set_device_name(uint8_t *device_name)
{
   // set device name
	  uint8_t adv_data_len = 0, *p;
	  
		adv_data_len = sizeof(adv_data) + strlen((const char*)device_name);
	  adv_data[3] = strlen((const char*)device_name) + 1;
    p = malloc(adv_data_len);
			if (p != NULL){
				
				memcpy(p, adv_data,  sizeof(adv_data));
				memcpy(p+sizeof(adv_data), device_name, strlen((const char*)device_name));
				gap_advertisements_set_data(adv_data_len, (uint8_t*)p);
			  //free(p);
		}
}

const uint8_t scanresp_data[] = {
    //- Manufacture.
    0x0c, 0xff, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', 
    //0x03,0x03,0x0,0xFF,0x09,0x09,0x42,0x52,0x54,0x20,0x44,0x41,0x54,0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0	
};
uint8_t scanresp_data_len = sizeof(scanresp_data);

void peripheral_set_mac(uint8_t mac[])
{ 
	extern uint8_t bd_addr[6];
	memcpy(bd_addr, mac, 6);
}

extern void sm_peripheral_setup(uint8_t auth, uint32_t passwd);
extern int btstack_main(void);

void peripheral_setup(ST_BLE_CONFIG ble_config)
{
	  peripheral_set_mac(ble_config.mac_addr);
    gatt_client_init();
    ble_init((void *)&blestack_init);
	  sm_peripheral_setup(ble_config.auth, ble_config.passwd);
	  btstack_main();
    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    bd_addr_t null_addr; 
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, ADV_IND, 0, null_addr, 0x07, 0x00);
    peripheral_set_device_name(ble_config.device_name);
    gap_scan_response_set_data(scanresp_data_len , (uint8_t*) scanresp_data);
    gap_advertisements_enable(1);
}
