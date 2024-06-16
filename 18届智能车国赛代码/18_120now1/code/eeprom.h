/*
 * eeprom.h
 *
 *  Created on: 2023Äê7ÔÂ14ÈÕ
 *      Author: ÈþÕÙa
 */

#ifndef CODE_EEPROM_H_
#define CODE_EEPROM_H_

#include "zf_common_headfile.h"

void Parameter_read_eeprom_canshu(void);
void Parameter_read_eeprom_canshu_2(void);
void Parameter_read_eeprom_canshu_3(void);
void Parameter_write_eeprom_canshu(void);
void Parameter_write_eeprom_canshu_2(void);
void Parameter_write_eeprom_canshu_3(void);
void Parameter_read_eeprom_xianshi(void);
void Parameter_write_eeprom_xianshi(void);
extern bool show_tuxiang;
extern bool show_canshu;
extern bool chuankou;
extern bool show_time;



#endif /* CODE_EEPROM_H_ */
