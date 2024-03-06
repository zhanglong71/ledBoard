
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"

#include "l_arch.h"
#include <string.h>
#include <stdio.h>

#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_timer.h"
#include "l_gpio.h"
#include "l_jsonTL.h"
#include "l_rs485.h"
// #include "main.h"
/**********************************************************************************************/
void reportVersion(void)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[32];
    
    sprintf(buf, "led softVER:202401231423.v01");

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        // u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}

/**********************************************************************************************/
#if 0
int doNothing(unsigned *arg)
{
    (void)arg;
    return 0;
}
#endif
/*******************************************************************************
 * prase json body(JsonParseL0)
 * 
 * example:
 * {key1:value1, key2:value2, ... ,keyn:valuen}
 *******************************************************************************/
char JsonParseL0(char* jsonstrbuf, kv_t* jsonstcarr)
{
    u8 j_u8,k_u8,i_u8 = 0;
    char *p[CMAX1_COUPLE * 2];
    char *pChar = (char *)jsonstrbuf;
    jsonstcarr[0].KVIndex = 0;

    /** 1. simily as '{ ... }'! **/
    if((jsonstrbuf[0] != '{') || (jsonstrbuf[strlen(jsonstrbuf) - 1] != '}')) {
        return(0);
    }

    jsonstrbuf[strlen(jsonstrbuf) - 1] = '\0';            /** overwrite the start '}' ! **/
    for(j_u8 = 0; j_u8 < strlen(jsonstrbuf); j_u8++) {    /** overwrite the end '{' ! **/
        jsonstrbuf[j_u8] = jsonstrbuf[j_u8 + 1];
    }
    
    while((p[i_u8]  = strtok(pChar, ":,")) != NULL) {    /** split the string ... **/
        i_u8++;
        pChar = NULL;
    }
 
    for(j_u8 = 0; ((j_u8 < i_u8/2) && (j_u8 < CMAX1_COUPLE)); j_u8++) {
        jsonstcarr[j_u8].KVIndex = i_u8/2 - j_u8;
        strcpy(jsonstcarr[j_u8].key, p[j_u8 * 2]);
        strcpy(jsonstcarr[j_u8].value, p[j_u8 * 2 + 1]);

        jsonstcarr[j_u8].key[strlen(jsonstcarr[j_u8].key) - 1] = '\0';           /** overwrite the start '"' ? **/
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].key); k_u8++)                 /** overwrite the end '"' ? **/
        {
            jsonstcarr[j_u8].key[k_u8] = jsonstcarr[j_u8].key[k_u8 + 1];
        }
    }
 
    return (1);
}

/*********************************************
 * {"voi":xxxx,"PLY":OK}
 * {"voi":xxxx,"PLY":err}
 *********************************************/
void generateLedDispAckOk(char* to)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[32];
    
    sprintf(buf, "{\"led\":%s,\"DIS\":OK}", to);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        rs485_stor_irq(&u8Data);
    }
}
void generateLedDispAckErr(char* to)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[32];
    
    sprintf(buf, "{\"led\":%s,\"DIS\":err}", to);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        rs485_stor_irq(&u8Data);
    }
}

