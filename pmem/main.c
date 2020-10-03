#include "main.h"

#define PMEM_APP_SIZE sizeof(AppConfig)
#define PMEM_CRC_SIZE sizeof(uint8_t)
#define PMEM_CHANNEL_SIZE sizeof(ChannelParam)

#define PMEM_APP_CRC_ADDR 0
#define PMEM_APP_ADDR PMEM_CRC_SIZE

#define PMEM_CHANNEL_ADDR(ind) 		PMEM_CRC_SIZE + PMEM_APP_SIZE + ind * (PMEM_CRC_SIZE + PMEM_CHANNEL_SIZE) + PMEM_CRC_SIZE
#define PMEM_CHANNEL_CRC_ADDR(ind)	PMEM_CRC_SIZE + PMEM_APP_SIZE + ind * (PMEM_CRC_SIZE + PMEM_CHANNEL_SIZE)
#define PMEM_CRC_INI 17

uint8_t pmem_calcCrc(const void *data, size_t sz){
	uint8_t crc = PMEM_CRC_INI;
	uint8_t *pt =(uint8_t *) data;
	for (size_t i = 0; i < sz; ++i) {
		crc_update(&crc, pt[i]);
	}
	return crc;
}

uint8_t pmem_calcAppConfigCrc(const AppConfig *item){
	return pmem_calcCrc(item, sizeof (AppConfig));
}

uint8_t pmem_calcChannelCrc(const ChannelParam *item){
	return pmem_calcCrc(item, sizeof (ChannelParam));
}

void pmem_setAppConfigCrc(const AppConfig *item){
	uint8_t crc = pmem_calcAppConfigCrc(item);
	EEPROM.put(PMEM_APP_CRC_ADDR, crc);
}

void pmem_setChannelCrc(const ChannelParam *item, size_t ind){
	uint8_t crc = pmem_calcChannelCrc(item);	
	printd("   channel save crc: ");printdln(crc);
	EEPROM.put(PMEM_CHANNEL_CRC_ADDR(ind), crc);
}

int pmem_checkSize(size_t channel_count){
	size_t total_bytes = channel_count * sizeof(ChannelParam) + sizeof(uint8_t) + 1;
	if(EEPROM.length() <= total_bytes){
		return 0;
	}
	return 1;
}

int pmem_checkAppConfigCrc(AppConfig *item){
	uint8_t crc1 = EEPROM[0];
	uint8_t crc2 = pmem_calcAppConfigCrc(item);
	if(crc1 != crc2){
		return 0;
	}
	return 1;
}

int pmem_checkChannelCrc(ChannelParam *item, int ind){
	uint8_t crc1 = EEPROM[PMEM_CHANNEL_CRC_ADDR(ind)];
	printd("   channel get crc: ");printdln(crc1);
	uint8_t crc2 = pmem_calcChannelCrc(item);
	if(crc1 != crc2){
		printd("   bad crc: ");printd(crc1);printd(" ");printdln(crc2);
		return 0;
	}
	return 1;
}


int pmem_hasSpaceForChannel(size_t ind){
	size_t required_bytes = PMEM_CHANNEL_ADDR(ind) + sizeof(ChannelParam) + 1;
	if(EEPROM.length() < required_bytes){
		return 0;
	}
	return 1;
}

int pmem_hasSpaceForAppConfig(){
	size_t required_bytes = PMEM_APP_ADDR + sizeof(AppConfig) + 1;
	if(EEPROM.length() < required_bytes){
		printdln("no space for app_config");
		return 0;
	}
	return 1;
}

int pmem_getPChannel(ChannelParam *item, size_t ind) {
	if(!pmem_hasSpaceForChannel(ind)){
		return 0;
	}
	ChannelParam t_item;
	EEPROM.get(PMEM_CHANNEL_ADDR(ind), t_item);
	if(!pmem_checkChannelCrc(&t_item, ind)){
		return 0;
	}
	*item = t_item;
	return 1;
}

int pmem_getPChannelForce(ChannelParam *item, size_t ind) {
	if(!pmem_hasSpaceForChannel(ind)){
		return 0;
	}
	EEPROM.get(PMEM_CHANNEL_ADDR(ind), *item);
	return 1;
}

int pmem_getAppConfig(AppConfig *item) {
	if(!pmem_hasSpaceForAppConfig()){
		return 0;
	}
	AppConfig tout;
	EEPROM.get(PMEM_APP_ADDR, tout);
	if(!pmem_checkAppConfigCrc(&tout)){
		return 0;
	}
	*item = tout;
	return 1;
}

int pmem_getAppConfigForce(AppConfig *item) {
	//if(!pmem_hasSpaceForAppConfig()){
	//	return 0;
//	}
	AppConfig temp;
	EEPROM.get(PMEM_APP_ADDR, temp);
	*item = temp;	
	return 1;
}

int pmem_savePChannel(const ChannelParam *item, size_t ind){
	printd("(saving channel: ");printd(ind);
	if(!pmem_hasSpaceForChannel(ind)){
		printd("no space) ");
		return 0;
	}
	EEPROM.put(PMEM_CHANNEL_ADDR(ind), *item);
	pmem_setChannelCrc(item, ind);
	printd("done) ");
	return 1;
}


int pmem_saveAppConfig(const AppConfig *item){
	printdln("saving device");
	if(!pmem_hasSpaceForAppConfig()){
		printdln("pmem: no space for device");
		return 0;
	}
	EEPROM.put(PMEM_APP_ADDR, *item);
	pmem_setAppConfigCrc(item);
	return 1;
}
PMEMCHANNEL_DEF_SET_FIELD_FUNC(id, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(display_kind, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(display_p1, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(display_p2, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(display_p3, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(display_text_alignment, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(serial_id, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(mode, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(remote_id, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(acp_command, int)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(time, unsigned long)
PMEMCHANNEL_DEF_SET_FIELD_FUNC(float_precision, int)

PMEMCHANNEL_DEF_GET_FIELD_FUNC(display_kind, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(display_p1, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(display_p2, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(display_p3, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(display_text_alignment, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(serial_id, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(mode, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(remote_id, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(acp_command, int)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(time, unsigned long)
PMEMCHANNEL_DEF_GET_FIELD_FUNC(float_precision, int)



