#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "system.h"
#include "peripheral.h"

ST_BLE_CONFIG ble_config;
extern void ble_tick_handler(void);
volatile unsigned int syStickCount = 0;

void _systick_handler(void)
{
	  ble_tick_handler();//mainloop超时计数函数
		if (peripheral_connect_state() == BLE_STATE_CONNECTED)
		{
			  digitalWrite(24, LOW);
		} else {
				if (syStickCount++ > 10) {     //计数一次10ms
						syStickCount = 0;
						digitalToggle(24);
           // digitalToggle(25);
			}
		}
}

static void set_bd_mac_from_flash_id(void) 
{
     #if 0 //方式1：手动设置
     ble_config.mac_addr[0]=0x13;
     ble_config.mac_addr[1]=0x11;
     ble_config.mac_addr[2]=0x02;
     ble_config.mac_addr[3]=0x20;
     ble_config.mac_addr[4]=0x7a;
     ble_config.mac_addr[5]=0x91;
     #endif
     #if 0 //方式2：配合机台烧录器或者上位机烧录器将唯一mac烧录到flash的地址第136个字节处
     init_spi_flash(0, SPIM_CLK_16MHZ);
     spi_flash_release_powerdown();
     spi_flash_read(136, ble_config.mac_addr, 6);
     spi_flash_enter_powerdown();
     #endif
     #if 1 //方式3：从flash的ID(16个字节)中选取6个字节作为mac, 经与flash厂商沟通选取按照如下规则
     uint8_t flash_id_buf[16]={0};
     init_spi_flash(0, SPIM_CLK_16MHZ);
     spi_flash_release_powerdown();
     spi_flash_read_128bitsID(flash_id_buf);
     ble_config.mac_addr[0]= flash_id_buf[11]; 
     ble_config.mac_addr[1]= (flash_id_buf[10]&0x01)|(flash_id_buf[9]<<1); 
     ble_config.mac_addr[2]= (flash_id_buf[9]>>7)|((flash_id_buf[8]&0x01)<<1)|(flash_id_buf[7]<<2); 
     ble_config.mac_addr[3]= (flash_id_buf[6]&0x0F)|((flash_id_buf[5]&0x0F)<<4); 
     ble_config.mac_addr[4]= (flash_id_buf[4]&0x0F)|((flash_id_buf[3]&0x0F)<<4); 
     ble_config.mac_addr[5]= ((flash_id_buf[3]&0xF0)>>4)|((flash_id_buf[2]&0xF)<<4);
     spi_flash_enter_powerdown();
     #endif
}

extern int serial1_read_tag(unsigned int timeout, const char* tag);

int	main(void)
{
		int at_len = 0;
		//int pre_time_ms = 0;
	  uint8_t buff[128];

    board_init();
		register_systick_callback(_systick_handler, 10);

    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
	  digitalWrite(24, HIGH);
	  digitalWrite(25, HIGH);
	  Serial1.begin(BAUD_115200);
    Serial1.printf("main\r\n");
	  set_bd_mac_from_flash_id();
	  spi_flash_release_powerdown();

	  set_bd_mac_from_flash_id();
	  peripheral_setup(ble_config);
    while(1) {
      ble_mainloop();  //31us
		  // Serial1.printf("pre_time_ms %d \r\n", pre_time_ms);
			at_len = Serial1.available();
			if (at_len)
			{
					if (peripheral_connect_state() == BLE_STATE_CONNECTED) {
						Serial1.read_buf(buff, at_len);
						peripheral_send(buff, at_len);
					} else {
            if (serial1_read_tag(20, "\r\n") > -1)
						{   
							  at_len = Serial1.available();
                Serial1.read_buf(buff, at_len);
                Serial1.write_buf(buff, at_len);
					      //at_cmd_handle(buff, at_len);
						} else {
								at_len = Serial1.available();
                Serial1.read_buf(buff, at_len);
                Serial1.write_buf(buff, at_len);
							  Serial1.printf("ERROR\r\n");
						}
			  }
			}
    }
}
