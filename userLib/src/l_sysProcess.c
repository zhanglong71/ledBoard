#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include <string.h>
#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_timer.h"
#include "l_gpio.h"
#include "l_actionFIFO.h"
#include "l_sysProcess.h"
#include "hk32f0301mxxc.h"

int sysProcess(void *pMsg)
{
    int iRet = TRUE;
    u8 buf[U8FIFOSIZE];
    u8Data_t u8Data;
    //u8 pCh = NULL;
    kv_t KVarr[6];
    // u8 len = 0;
    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART2RX:
	    #if 1
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            //u8 len = 0;
            u8Data_t u8Data;
            while (u8FIFOout_irq(&g_uart2RxQue, &u8Data) == TRUE) {
                //u8FIFOin_irq(&g_uart2TxQue, &u8Data); //????????????????????????????
            } 
        }
        #else
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            /** do something to uart3 data **/
            //???????????????????????????????????????
            
            u8 len = 0;
            u8Data_t u8Data;
            objType_t objtype = sm_receiveData(buf);
            if ((objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body)) {
                #if 0
            	len = strlen(buf);
                for(int i = 0; i < len; i++) {
                    u8Data.u8Val = buf[i];
                    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
                }
                #endif
            }
            //???????????????????????????????????????
        }
       #endif

        break;
    //case CMSG_MASTER:
    case CUART1_TOUT:
        /************************************
         * receive:  {"xxx":led,"R":0,"G":250,"B":0}  --- 只显示颜色
         *   或      {"xxx":led,"R":0,"G":250,"B":0,"TIM":12}  --- 支持颜色渐变】
         *          
         * ack:
         *
         *    {"led":xxx,"DIS":OK}
         *    {"led":xxx,"DIS":err}
         ************************************/
        memset(buf, 0, sizeof(buf));
        memset(KVarr, 0, sizeof(KVarr));
        u8 i = 0;
 #if 1
        for(i = 0; ((i < MTABSIZE(buf)) && (u8FIFOout_irq(&g_uart1RxQue, &u8Data) == TRUE)); i++) {
            buf[i] = u8Data.u8Val;
        }
        u8FIFOinit_irq(&g_uart1RxQue);  /** !!!!!!!!!!!! **/
        buf[i] = '\0';
 #endif
 #if 0
        for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
            u8Data.u8Val = buf[i];
            rs485_stor_irq(&u8Data);
        }
 #else
        if (JsonParseL0(buf, KVarr)) {
            u8 red = 0;
            u8 green = 0;
            u8 blue = 0;
            u8 tim = 0;
            u8 color_flag = 0;
            u8 src_idx = 0xff;
            for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                 if (strstr(KVarr[i].value, "led")) {
                     Mset_bit(color_flag, 1);
                     src_idx = i;
                 }
                 if (strstr(KVarr[i].key, "R")) {
                     Mset_bit(color_flag, 2);
                     red = atoi(KVarr[i].value);
                 }
			     if (strstr(KVarr[i].key, "G")) {
                     Mset_bit(color_flag, 3);
                     green = atoi(KVarr[i].value);
                 }
                 if (strstr(KVarr[i].key, "B")) {
                     Mset_bit(color_flag, 4);
                     blue = atoi(KVarr[i].value);
                 }
                 if (strstr(KVarr[i].key, "TIM")) {
                     Mset_bit(color_flag, 5);
                     tim = atoi(KVarr[i].value);
                 }
            }
            if (Mget_bit(color_flag, 1) && Mget_bit(color_flag, 2) && Mget_bit(color_flag, 3) && Mget_bit(color_flag, 4)) {
                if ((red <= 250) && (green <= 250) && (blue <= 250)) {
                    /** construct ack**/
                    u32 color = ((green << 16) | (red << 8) | blue);
                    if (tim == 0) {
                        ledRGBbreath_stop();
                        LED_display(color);
                    } else  {
                        ledRGBbreath_start(color, tim);
                    }
                    generateLedDispAckOk(KVarr[src_idx].key);
                } else {
                    generateLedDispAckErr(KVarr[src_idx].key);
                }
            } 
            else
            {
                /*** nothing ***/
            }
        };
#endif
        break;
        
    case CMSG_UART1RX:
	    #if 0
        if(u8FIFOisEmpty(&g_uart1RxQue) != TRUE) {
            //u8 len = 0;
            
            //while (u8FIFO_last(&g_uart1RxQue, &u8Data) == TRUE) {
            while (u8FIFOout_irq(&g_uart1RxQue, &u8Data) == TRUE) {
                u8FIFOin_irq(&g_uart2TxQue, &u8Data);//????????????????????????????
            } 
        }
        #endif
        break;
        
    case CMSG_UART2TX:
    #if 1
        sm_sendData(NULL);
    #endif
        break;
    
    case CMSG_UART1TX:
        /** no what ? **/
        
        break;

    case CGET_CHAR:
    #if 1
        (void)reportGetCharCmd(NULL);
    #endif
        break;
		
    case CPUT_CHAR:
         #if 0  //?????????????????? for test only
			(void)JsonParse(NULL, NULL);
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
			break;
        
    case CGETDEVINFO_REQ:
    case CGETDEVINFO_RSPERROR:
    #if 1
        (void)reportDevInfo(NULL);
    #endif
        break;
        
    case CREPORT_RSPERROR:   /** report Services error, try again **/
    case CGETDEVINFO_RSPOK:  /** according protocal report Services after devInfo reported ok **/
        #if 1
        (void)reportService(0);
        #endif
        break;
        
    case CREPORT_RSPOK:
        /** do nothing **/
        #if 0  //?????????????????? for test only
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'e';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 's';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        break;

    case CWIFI_STATUS:
        /** do what ? **/
        #if 0
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
        break;    
        
    case CHEART_BEAT:
        #if 1
        (void)reportHeartbeat(NULL);
        #endif
        break;

    case CWIFI_TEST:
        /** do test only! 2-byte data !!! **/
        #if 0
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
        break;
        
    case CPMT_TOUT:        /** 一段提示动作完成 **/
		actProcess(&g_promptQueue);
		break;
        
    case C485_TOUT:
		rs485actProcess();
		break;
    
    case C485_OVER:    
        rs485actOver();
        break;
        
    case CLED_STEP:
        ledRGBProcess();
        break;
                          
    default:
        iRet = FALSE;
        break;	
	}
    return  iRet;   
}
/**********************************************************************************************************/

