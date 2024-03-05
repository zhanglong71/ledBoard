#ifndef __L_GPIO_H_
#define __L_GPIO_H_

void GPIO_initVOPPort(void);
void GPIO_init485(void);

void GPIO_VOPPWR_off(void);
void GPIO_VOPPWR_on(void);


void GPIO_init4led(void);
void GPIO_led_blink(void);
void GPIO_led1_blink(void);
void GPIO_led2_blink(void);
void LED_display(u32 __color);

u8 getColorDepth(void);
void ledRGBinit(void);
void ledRGBProcess(void);
void ledRGBbreath_start(u32 _color, u16 _tick);
void ledRGBbreath_stop(void);

#endif
