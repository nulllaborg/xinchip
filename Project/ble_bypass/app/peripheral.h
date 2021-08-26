#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#include <stdio.h>
#include <string.h>

#include "ble.h"
#define min(a,b) ((a)<(b)?(a):(b))

#define ADV_MIN_INTERVAL 0x0030
#define ADV_MAX_INTERVAL 0x0030

//BLE device name
#define DEV_NAME_DEFAULT    "NL-16"
#define DEV_NAME_LEN        12

// 系统定时器间隔时间
#define SBP_PERIODIC_EVT_PERIOD                   100

//最大记录的从机地址
#define MAX_PERIPHERAL_MAC_ADDR                   8

//mac地址的字符长度 (一个字节等于两个字符)
#define MAC_ADDR_CHAR_LEN                          6

typedef enum
{
    SLAVE_MODE = 0,         // SLAVE mode   
    MASTER_MODE = 1,        // master mode
}EM_ROLE_MODE;

typedef struct 
{
    uint8_t work_mode;     // BLE_WORK_MODE 0:Serial 1: Drive 2: iBeacon
    uint8_t device_name[DEV_NAME_LEN];                 
    EM_ROLE_MODE role_mode;                  // 0: Master 1: Slave defalut 1
    uint32_t passwd;
    uint8_t auth;                     //0: no password 1:need password

    uint8_t mac_addr[MAC_ADDR_CHAR_LEN];          //本机mac地址 最大12位 字符表示
    //uint8_t AutoConnectMac[MAC_ADDR_CHAR_LEN+1];    //指定去连接的mac地址
    uint8_t AutoConnectFlag;

    //曾经成功连接过的从机个数
    uint8_t ever_connect_peripheral_mac_addr_conut;
    //曾经成功连接过的从机个数,当前index， 用于增加从机地址时快速插入或读取
    uint8_t ever_connect_peripheral_mac_addr_index;
    //最新一次成功连接过的从机地址index， 用于针对AT+CONNL 这个指令
    uint8_t last_connect_peripheral_mac_addr_index;
    //曾经成功连接过的从机地址
    uint8_t ever_connect_mac_status[MAX_PERIPHERAL_MAC_ADDR][MAC_ADDR_CHAR_LEN];

   // Para: 000000～009999 
   // 000000 代表持续连接，其
   // 余代表尝试的毫秒数
   // Default:001000
    uint16_t try_connect_time_ms;           // 尝试连接时间---目前无效
    int8_t rssi;                              //  RSSI 信号值
    uint8_t rxGain;                           //  接收增益强度
    uint8_t Mini_Interval, Max_Interval;
    uint8_t txPower;                          //  发射信号强度
    uint16_t ibeacon_adver_time_ms;         // 广播间隔
    uint16_t serv_uuid;         // 服务uuid
    uint16_t char_uuid;         // 字符uuid
}ST_BLE_CONFIG;


enum adv_type{
	ADV_IND,
	ADV_DIRECT_IND,
	ADV_SCAN_IND,
	ADV_NONCONN_IND,
};

typedef enum
{
    BLE_STATE_IDLE,                    //无连接-空闲状态
    BLE_STATE_CONNECTING,              //连接中...
    BLE_STATE_CONNECTED,               //已连接上
    BLE_STATE_DISCONNECTING,           //断开连接中
    BLE_STATE_ADVERTISING              //从机广播中
}BLE_STATE;

BLE_STATE peripheral_connect_state(void);
void peripheral_set_mac(uint8_t mac[]);
void peripheral_set_connect_state(BLE_STATE state);
void peripheral_setup(ST_BLE_CONFIG ble_config);
void peripheral_set_device_name(uint8_t *device_name);
void peripheral_send(uint8_t *buffer, uint16_t length);
#endif
