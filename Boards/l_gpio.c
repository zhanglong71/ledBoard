#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_gpio.h"

void GPIO_initVOPPort(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure PD02() in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
    
  /* Configure PD03(5V_EN) in output pushpull mode */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_SetBits(GPIOD, GPIO_Pin_2);
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}

void GPIO_init485(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure PD02() in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  M485TR_R("不发送时，485处于接收状态");
}


void GPIO_VOPPWR_on(void)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
}

void GPIO_VOPPWR_off(void)
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    //GPIO_SetBits(GPIOD, GPIO_Pin_3);   // ???????????????????????
}

/** for test **/
void GPIO_init4led(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure PD01 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  //GPIO_Init(GPIOD, &GPIO_InitStructure);

  /** led2 **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  /** RGB_display **/
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void GPIO_led1_blink(void)
{
    GPIO_Toggle(GPIOD, GPIO_Pin_1);
}

void GPIO_led2_blink(void)
{
    GPIO_Toggle(GPIOC, GPIO_Pin_7);
}

void GPIO_led_blink(void)
{
#if 0
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7) == Bit_RESET) { \
        GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);    \
    } else {                                          \
        GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);  \
    }
    
    if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1) == Bit_RESET) { \
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_SET);    \
    } else {                                          \
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_RESET);  \
    }
#else
    GPIO_Toggle(GPIOD, GPIO_Pin_1);
    GPIO_Toggle(GPIOC, GPIO_Pin_7);
#endif
}

/*************************************************************************/
static void GPIO_bit0(void)
{
    GPIOC->BSRR = GPIO_Pin_5; /** GPIO_SetBits(GPIOC, GPIO_Pin_5); **/
    NOP();
    NOP();
    
    GPIOC->BRR = GPIO_Pin_5;  /** GPIO_ResetBits(GPIOC, GPIO_Pin_5); **/
}

static void GPIO_bit1(void)
{
    GPIOC->BSRR = GPIO_Pin_5;  /** GPIO_SetBits(GPIOC, GPIO_Pin_5); **/
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    
    GPIOC->BRR = GPIO_Pin_5;   /** GPIO_ResetBits(GPIOC, GPIO_Pin_5); **/
    
}

const paction_t_0 colorArr[] = {
        GPIO_bit1,
        GPIO_bit0,
    };

void LED_display(u32 __color)
{
   IRQ_disable();

   colorArr[!(__color & (1 << 23))]();
   colorArr[!(__color & (1 << 22))]();
   colorArr[!(__color & (1 << 21))]();
   colorArr[!(__color & (1 << 20))]();
   colorArr[!(__color & (1 << 19))]();
   colorArr[!(__color & (1 << 18))]();
   colorArr[!(__color & (1 << 17))]();
   colorArr[!(__color & (1 << 16))]();
   colorArr[!(__color & (1 << 15))]();
   colorArr[!(__color & (1 << 14))]();
   colorArr[!(__color & (1 << 13))]();
   colorArr[!(__color & (1 << 12))]();
   colorArr[!(__color & (1 << 11))]();
   colorArr[!(__color & (1 << 10))]();
   colorArr[!(__color & (1 << 9))]();
   colorArr[!(__color & (1 << 8))]();
   colorArr[!(__color & (1 << 7))]();
   colorArr[!(__color & (1 << 6))]();
   colorArr[!(__color & (1 << 5))]();
   colorArr[!(__color & (1 << 4))]();
   colorArr[!(__color & (1 << 3))]();
   colorArr[!(__color & (1 << 2))]();
   colorArr[!(__color & (1 << 1))]();
   colorArr[!(__color & (1))]();
   
   IRQ_enable();
}

/*************************************************************************
 * color value
 *************************************************************************/
u8 getColorDepth(void)
{
    u8 const led_depth_tab[] = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        12,
        15,
        19,
        24,
        30,
        37,
        45,
        54,
        64,
        75,
        87,
        100,
        114,
        129,
        145,
        162,
        180,
        199,
        219,
        240,      
        250,
        
        240,
        219,
        199,
        180,
        162,
        145,
        129,
        114,
        100,
        87,
        75,
        64,
        54,
        45,
        37,
        30,
        24,
        19,
        15,
        12,
        10,
        9,
        8,
        7,
        6,
        5,
        4,
        3,
        2,
        1
    };
    u8 static color_depth_index = 0;
    color_depth_index++;
    if (color_depth_index >= MTABSIZE(led_depth_tab)) {
        color_depth_index = 0;
    }
    return led_depth_tab[color_depth_index];
}
/*************************************************************************/
void ledRGBinit(void)
{
#if 1
    g_led_display.color = 0;
    g_led_display.tick = 0;

    g_led_display.ptimer = &(g_timer[3]);
    //g_led_display.sm_status = CLEDDISP_NONE;
    g_led_display.stepMsgType = CLED_STEP;
    g_led_display.overMsgType = CLED_OVER;

    ClrTimer_irq(g_led_display.ptimer);
#endif
}

void ledRGBProcess(void)
{
    u8 depth = 0;
    u32 color = 0;

    ClrTimer_irq(g_led_display.ptimer);
    depth = getColorDepth();
    if(g_led_display.color & 0x00ff0000) {
        color |= (depth << 16);
    }
    if(g_led_display.color & 0x0000ff00) {
        color |= (depth << 8);
    }
    if(g_led_display.color & 0x000000ff) {
        color |= (depth);
    }
    
    LED_display(color);
    SetTimer_irq(g_led_display.ptimer, g_led_display.tick, g_led_display.stepMsgType);
}

void ledRGBbreath_start(u32 _color, u16 _tick)
{
    ClrTimer_irq(g_led_display.ptimer);
    if (_tick < TIMER_70MS) {
        g_led_display.tick = TIMER_70MS;
    } else {
        g_led_display.tick = _tick;   // set mask
    }
    
    g_led_display.color = 0;   // set mask
    if(_color & 0x00ff0000) {
        g_led_display.color |= 0x00ff0000;
    }
    if(_color & 0x0000ff00) {
        g_led_display.color |= 0x0000ff00;
    }
    if(_color & 0x000000ff) {
        g_led_display.color |= 0x000000ff;
    }

    LED_display(g_led_display.color & 0x00010101);
    SetTimer_irq(g_led_display.ptimer, g_led_display.tick, g_led_display.stepMsgType);
}

void ledRGBbreath_stop(void)
{
    g_led_display.color = 0;   // mask
    g_led_display.tick = 0;
    ClrTimer_irq(g_led_display.ptimer);
}

