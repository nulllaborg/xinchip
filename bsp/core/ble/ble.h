#ifndef	__BLE_C_H
#define	__BLE_C_H

#include "bluetooth.h"

#ifndef UNUSED
#define UNUSED(x) (void)(sizeof(x))
#endif

void	ble_init(void *init);
void	ble_mainloop(void);

typedef struct {
    uint16_t (*att_read_callback)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size);
    int (*att_write_callback)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
    void (*packet_handler) (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
    const uint8_t *ble_device_db;
}blestack_init_t;

/**
 * State of BTstack 
 */
typedef enum {
    HCI_STATE_OFF = 0,
    HCI_STATE_INITIALIZING,
    HCI_STATE_WORKING,
    HCI_STATE_HALTING,
    HCI_STATE_SLEEPING,
    HCI_STATE_FALLING_ASLEEP
} HCI_STATE;

#define BTSTACK_EVENT_STATE                                0x60
#define BTSTACK_EVENT_NR_CONNECTIONS_CHANGED               0x61
#define DAEMON_EVENT_VERSION                               0x63
#define L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE   0x77
#define ATT_EVENT_MTU_EXCHANGE_COMPLETE                    0xB5
#define ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE         0xB6
#define ATT_EVENT_CAN_SEND_NOW                             0xB7
/** 
 * @brief Sets a fixed random address for advertising
 * @param addr
 * @note Sets random address mode to type off
 */
void gap_random_address_set(bd_addr_t addr);

/**
 * @brief Set Advertisement Data
 * @param advertising_data_length
 * @param advertising_data (max 31 octets)
 * @note data is not copied, pointer has to stay valid
 * @note '00:00:00:00:00:00' in advertising_data will be replaced with actual bd addr
 */
void gap_advertisements_set_data(uint8_t advertising_data_length, uint8_t * advertising_data);

/**
 * @brief Set Advertisement Paramters
 * @param adv_int_min
 * @param adv_int_max
 * @param adv_type
 * @param direct_address_type
 * @param direct_address
 * @param channel_map
 * @param filter_policy
 * @note own_address_type is used from gap_random_address_set_mode
 */
void gap_advertisements_set_params(uint16_t adv_int_min, uint16_t adv_int_max, uint8_t adv_type,
	uint8_t direct_address_typ, bd_addr_t direct_address, uint8_t channel_map, uint8_t filter_policy);

/** 
 * @brief Enable/Disable Advertisements. OFF by default.
 * @param enabled
 */
void gap_advertisements_enable(int enabled);

/** 
 * @brief Set Scan Response Data
 *
 * @note For scan response data, scannable undirected advertising (ADV_SCAN_IND) need to be used
 *
 * @param advertising_data_length
 * @param advertising_data (max 31 octets)
 * @note data is not copied, pointer has to stay valid
 * @note '00:00:00:00:00:00' in scan_response_data will be replaced with actual bd addr
 */
void gap_scan_response_set_data(uint8_t scan_response_data_length, uint8_t * scan_response_data);


static inline uint16_t little_endian_read_16(const uint8_t * buffer, int pos){
    return (uint16_t)(((uint16_t) buffer[pos]) | (((uint16_t)buffer[(pos)+1]) << 8));
}
/**
 * @brief Get event type
 * @param event
 * @return type of event
 */
static inline uint8_t hci_event_packet_get_type(const uint8_t * event){
    return event[0];
}

/**
 * @brief Get field state from event BTSTACK_EVENT_STATE
 * @param event packet
 * @return state
 * @note: btstack_type 1
 */
static inline uint8_t btstack_event_state_get_state(const uint8_t * event){
    return event[2];
}

/***
 * @brief Get subevent code for le event
 * @param event packet
 * @return subevent_code
 */
static inline uint8_t hci_event_le_meta_get_subevent_code(const uint8_t * event){
    return event[2];
}
/**
 * @brief Get field status from event HCI_EVENT_CONNECTION_COMPLETE
 * @param event packet
 * @return status
 * @note: btstack_type 1
 */
static inline uint8_t hci_event_connection_complete_get_status(const uint8_t * event){
    return event[2];
}

/**
 * @brief Get field connection_handle from event HCI_EVENT_CONNECTION_COMPLETE
 * @param event packet
 * @return connection_handle
 * @note: btstack_type 2
 */
static inline uint16_t hci_event_connection_complete_get_connection_handle(const uint8_t * event){
    return little_endian_read_16(event, 3);
}

/**
 * @brief Get field connection_handle from event HCI_EVENT_DISCONNECTION_COMPLETE
 * @param event packet
 * @return connection_handle
 * @note: btstack_type 2
 */
static inline uint16_t hci_event_disconnection_complete_get_connection_handle(const uint8_t * event){
    return little_endian_read_16(event, 3);
}
/**
 * @brief Get field connection_handle from event HCI_SUBEVENT_LE_CONNECTION_COMPLETE
 * @param event packet
 * @return connection_handle
 * @note: btstack_type H
 */
static inline hci_con_handle_t hci_subevent_le_connection_complete_get_connection_handle(const uint8_t * event){
    return little_endian_read_16(event, 4);
}

/**
 * @brief Get field result from event L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE
 * @param event packet
 * @return result
 * @note: btstack_type 2
 */
static inline uint16_t l2cap_event_connection_parameter_update_response_get_result(const uint8_t * event){
    return little_endian_read_16(event, 4);
}
/**
 * @brief Get field reason from event HCI_EVENT_DISCONNECTION_COMPLETE
 * @param event packet
 * @return reason
 * @note: btstack_type 1
 */
static inline uint8_t hci_event_disconnection_complete_get_reason(const uint8_t * event){
    return event[5];
}
/**
 * @brief Get field MTU from event ATT_EVENT_MTU_EXCHANGE_COMPLETE
 * @param event packet
 * @return MTU
 * @note: btstack_type 2
 */
static inline uint16_t att_event_mtu_exchange_complete_get_MTU(const uint8_t * event){
    return little_endian_read_16(event, 4);
}

/**
 * @brief Request an update of the connection parameter for a given LE connection
 * @param handle
 * @param conn_interval_min (unit: 1.25ms)
 * @param conn_interval_max (unit: 1.25ms)
 * @param conn_latency
 * @param supervision_timeout (unit: 10ms)
 * @returns 0 if ok
 */
int gap_request_connection_parameter_update(hci_con_handle_t con_handle, uint16_t conn_interval_min,
	uint16_t conn_interval_max, uint16_t conn_latency, uint16_t supervision_timeout);

/*
 * @brief notify client about attribute value change
 * @param con_handle
 * @param attribute_handle
 * @param value
 * @param value_len
 * @return 0 if ok, error otherwise
 */
int att_server_notify(hci_con_handle_t con_handle, uint16_t attribute_handle, const uint8_t *value, uint16_t value_len);

/*
 * @brief indicate value change to client. client is supposed to reply with an indication_response
 * @param con_handle
 * @param attribute_handle
 * @param value
 * @param value_len
 * @return 0 if ok, error otherwise
 */
int att_server_indicate(hci_con_handle_t con_handle, uint16_t attribute_handle, const uint8_t *value, uint16_t value_len);
/**
 * @brief Get field status from event ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE
 * @param event packet
 * @return status
 * @note: btstack_type 1
 */
static inline uint8_t att_event_handle_value_indication_complete_get_status(const uint8_t * event){
    return event[2];
}

#endif
