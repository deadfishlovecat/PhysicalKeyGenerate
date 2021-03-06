/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   主函数C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "main.h"					//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项


const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0;
uint8_t g_RF24L01RxBuffer[ 200 ] = { 0 }; 
uint8_t rssi_buffer[15] = {0};
@near int rssi_data[frame_num] = { 0 };
//@near int tmp_value_2[frame_num] = {0 };
int rssi=0;
uint8_t temp=65;
uint8_t rssi_ascii[3];

//uint8_t *changeline='\n';

int sendFlag=0;
uint8_t mChannel=0;

//进行交织，结果保存在tmp_value_1中
/*
void interleave(uint8_t m)
{
	//变量初始化
	uint8_t data_len;
	uint8_t remainder;
	uint8_t quo;
	uint8_t i;
	uint8_t j;
	uint8_t index;
	
	data_len = frame_num;
	remainder = frame_num % m;
	quo = data_len/m;
	index = 0;
	for(i =0; i < m; i++)
	{
		if (i < remainder)
		{
			for(j=0; j<quo+1; j++)
			{
				tmp_value_1[index] = rssi_data[i+j*m];
			}
			
		}
		else
		{
			for(j=0; j<quo; j++)
			{
				tmp_value_1[index] = rssi_data[i+j*m];
			}
			
		}
	}
}
*/
//
/**
  * @brief :主函数 
  * @param :无
  * @note  :无
  * @retval:无
  */ 
int main( void )
{	
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t send_index = 0;
	uint8_t rece_index = 0;
	
	uint8_t loop_num = frame_num/5;
	int index = 0;
	
	//串口初始化
	drv_uart_init( 38400);
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
	drv_spi_init( );
	
	//CC1101初始化
	CC1101_Init( );
	for( i = 0; i < 6; i++ )
	{
		led_red_flashing( );
		led_green_flashing( );
		drv_delay_ms( 500 );
	}
	
	//首先准备发送模式
	CC1101_Clear_RxBuffer( );
	CC1101_Set_Mode( RX_MODE );
	while(1)
	{
		//首先等待射频
		
		i = CC1101_Rx_Packet( g_RF24L01RxBuffer,&rssi );
		
		if( 0 != i )
		{
			rece_index += i;
			while(rece_index<g_RF24L01RxBuffer[0]+1)
			{
				CC1101_Clear_RxBuffer( );
				CC1101_Set_Mode( RX_MODE );
				i = CC1101_Rx_Packet( g_RF24L01RxBuffer+rece_index,&rssi );
				rece_index += i;
			}
			led_green_on();
			drv_uart_tx_bytes(g_RF24L01RxBuffer, rece_index);
			i = drv_uart_rx_bytes(g_RF24L01RxBuffer);
			CC1101_Tx_Packet(g_RF24L01RxBuffer, i, ADDRESS_CHECK);
			while(send_index < i-60)
			{
				CC1101_Tx_Packet( g_RF24L01RxBuffer+send_index, 60 , ADDRESS_CHECK );
				send_index += 60;
			}
			CC1101_Tx_Packet( g_RF24L01RxBuffer+send_index, i-send_index , ADDRESS_CHECK );
		}
		
	}
	led_green_off( );
	return 0;
	

	
	
		
	
		

	
}

