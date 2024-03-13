#include <EEPROM.h>
#include "config.h"

void EPROM_save(String topicSet, String encoded);
void EPROM_write(int addr, String encoded);
String EPROM_read(int addr);
void EPROM_init();
int EPROM_getAddress(String topicSet);