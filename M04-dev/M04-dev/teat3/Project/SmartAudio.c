#include "SmartAudio.h"


SmartAudio_UNIFY Unify={
	SmartAudio_V1,
	SmartAudio_25mW,
	0,
	SmartAudio_in_range_PitMode,
	5865,
	SmartAudioCmd_None,
};

static const uint8_t SmartAudio_CRC8Tab[256] = 
{
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9,
};
static uint8_t CRC8(const uint8_t *data, uint8_t Len);         //CRC校验

static uint8_t CRC8(const uint8_t *data, uint8_t Len)
{
	uint8_t num;
	uint8_t crc = 0;
	for (num = 0; num<Len; num++)
		crc = SmartAudio_CRC8Tab[crc ^ *data++];
	return crc;
}

uint8_t SmartAudio_tx(uint8_t *buff)//整理打包数据帧
{
	buff[0] = 0x00;
	buff[1] = 0xAA;
	buff[2] = 0x55;
	buff[3] = Unify.HostCmd;
	uint8_t tx_len;
	if(Unify.HostCmd == SmartAudioCmd_GetSettingV1)
	{
		if(Unify.Version == SmartAudio_V2)
			buff[3] = SmartAudioCmd_GetSettingV2;
	}
	switch(Unify.HostCmd)
	{
		case SmartAudioCmd_GetSettingV1:
		case SmartAudioCmd_GetSettingV2:
			buff[4] = 0x06;
			buff[5] = Unify.Channel;
			buff[6] = Unify.Power;
			buff[7] = Unify.Mode;
			buff[8] = Unify.Frequency>>8;//高8位先发
			buff[9] = Unify.Frequency;
			buff[10] = CRC8(&buff[3],7);
			buff[11] = 0x00;
			tx_len = 12;
			break;
		case SmartAudioCmd_SetPower:
			buff[4] = 0x02;
			buff[5] = Unify.Power;
			buff[6] = CRC8(&buff[3],3);
			buff[7] = 0X00;
			tx_len = 8;
			break;
		case SmartAudioCmd_SetChannel:
			buff[4] = 0x02;
			buff[5] = Unify.Channel;
			buff[6] = CRC8(&buff[3],3);
			buff[7] = 0x00;
			tx_len = 8;
			break;
		case SmartAudioCmd_SetFrequency:
			buff[4] = 0x03;
			buff[5] = (Unify.Frequency>>8);
			buff[6] = Unify.Frequency;
			buff[7] = CRC8(&buff[3],4);
			buff[8] = 0x00;
			tx_len = 9;
			break;
		case SmartAudioCmd_SetOperation_mode:
			if(Unify.Version == SmartAudio_V2)
			{
				buff[4] = 0x02;
				buff[5] = Unify.Mode;
				buff[6] = CRC8(&buff[3],3);
				buff[7] = 0x00;
				tx_len = 8;
			}
			else 
			{
				tx_len = 0;
				memset(buff,0,USART_buffsize);
			}
			break;
		default:
			tx_len = 0;
			memset(buff,0,USART_buffsize);
			break;
	}
	return tx_len;//返回数据帧的实际长度，用于后续打包发送
}

uint8_t SmartAudio_rx(uint8_t *buff, uint8_t buff_len)//接收解析Host命令
{
	uint8_t Count;
	uint8_t len;
	uint8_t CRC_calculate;
	uint8_t CRC_frame;
	//<0x00><0xAA><0x55><cmd><length><data><crc>
	for(Count = 0;Count<buff_len-3;Count ++)
	{
		if(buff[Count] == 0xAA && buff[Count+1] == 0x55)
		{
			if(buff[Count+3] < (buff_len-Count-4))//判定数据帧能否完整存储在缓冲区
			{
				len =buff[Count+3]; 
				CRC_calculate = CRC8(&buff[Count], len+4);
				CRC_frame = buff[Count+3+len+1];
				if(CRC_frame == CRC_calculate)//判定CRC校验是否通过
				{
					Unify.HostCmd = buff[Count+2]>>1;
					switch(Unify.HostCmd)//开始进行HostCmd解析
					{
						case SmartAudioCmd_GetSettingV1:
							break;
						case SmartAudioCmd_SetPower:
							Unify.Power = buff[Count+4];
							break;
						case SmartAudioCmd_SetChannel:
							Unify.Channel = buff[Count+4];
							break;
						case SmartAudioCmd_SetFrequency:
							Unify.Frequency = (uint16_t)buff[Count+4]<<8;
							Unify.Frequency |= (uint16_t)buff[Count+5];
							break;
						case SmartAudioCmd_SetOperation_mode:
							if(Unify.Version == SmartAudio_V2) Unify.Mode = buff[Count+3];
							else Unify.Mode = SmartAudio_in_range_PitMode;
							break;
						default:
							break;
					}return 1;
				}
			}
		}			
	}return 0;
}

void SmartAudio_VTX_send(void)//VTX发送数据包
{
	USART_send_only();
	USART0_buff_Ctrl.send_buff_len = SmartAudio_tx(USART0_buff_Ctrl.BUFF_send);
	USART_send_buffer(USART0_buff_Ctrl.BUFF_send,USART0_buff_Ctrl.send_buff_len);//实际发送处理
	
	memset(USART0_buff_Ctrl.BUFF_send,0,USART_buffsize);//清空缓存区
	USART0_buff_Ctrl.send_buff_len = 0;//发送缓存区数据包长度值归零
	USART0_buff_Ctrl.FLAG_send_complete = 0;
}

void  SmartAudio_VTX_updatestate(void)
{
	if(SmartAudio_rx(USART0_buff_Ctrl.BUFF_receive,USART0_buff_Ctrl.receive_buff_len) == 1)//判定当前缓冲区内的帧是否有效
	{
		USART0_buff_Ctrl.FLAG_send_complete = 1;
		//delay_1ms(20);
		SmartAudio_VTX_send();
	}
	memset(USART0_buff_Ctrl.BUFF_receive,0,USART_buffsize);
	USART0_buff_Ctrl.receive_buff_len = 0;
	USART0_buff_Ctrl.FLAG_receive_complete = 0;
	USART_start_receive();
}

