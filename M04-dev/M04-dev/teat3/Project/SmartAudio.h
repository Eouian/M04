#include "gd32f1x0.h"
#include <stdio.h>
#include "systick.h"

//frame structure:<Start code> <Commands> <Frame length> <Payload> <CRC>

//Command
#define SmartAudioCmd_None								0x00//无命令
#define SmartAudioCmd_GetSettingV1				0x01//配置V1的应答
#define SmartAudioCmd_GetSettingV2				0x09//配置V2的应答
#define SmartAudioCmd_SetPower						0x02//配置电源
#define SmartAudioCmd_SetChannel					0x03//配置通道
#define SmartAudioCmd_SetFrequency				0x04//配置频率
#define SmartAudioCmd_SetOperation_mode		0x05//配置模式（仅限V2）

//Version
#define SmartAudio_VN   0x00//未知
#define SmartAudio_V1		0x01//V1
#define SmartAudio_V2		0x09//V2

//Power-Level
#define SmartAudio_25mW			0
#define SmartAudio_200mW		1
#define SmartAudio_500mW		2
#define SmartAudio_800mW		3

//Operation Mode
#define SmartAudio_in_range_PitMode		0x01
#define SmartAudio_out_range_PitMode	0x02
#define SmartAudio_disable_PitMode 		0x04//off only
#define SmartAudio_lock_unlock_VTX 		0x08//0:lock,1:unlock

typedef struct
{
	uint8_t Version;
	uint8_t Power;
	uint8_t Channel;
	uint8_t Mode;
	uint16_t Frequency;
	uint8_t HostCmd;
}SmartAudio_UNIFY;
void SmartAudio_tx(uint8_t *buff);//SmartAudio打包发送应答主机
uint8_t SmartAudio_rx(uint8_t *buff, uint8_t buff_len);//SmartAudio接收主机请求命令


