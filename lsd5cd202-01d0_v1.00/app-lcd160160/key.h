#ifndef __KEY_H__
#define __KEY_H__



extern void fn_systick_interrupt(void);
extern void fn_key_process(void);
extern void fn_systick_init(void);
extern void fn_key_init(void);


extern unsigned char g_u8_main_menu_index;
extern unsigned char g_u8_main_local_data_index;
extern unsigned char g_u8_main_measure_data_index;
extern unsigned char g_u8_main_para_set_index;

extern unsigned char g_u8_main_measure_data_detial_index;


#endif
