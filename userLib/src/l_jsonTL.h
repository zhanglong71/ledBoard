#ifndef __L_JSONTL_H__
#define __L_JSONTL_H__

char JsonParseL0(char* jsonstrbuf, kv_t* jsonstcarr);
void reportVersion(void);

/***********************************************/
void generateLedDispAckOk(char* to);
void generateLedDispAckErr(char* to);

/***********************************************/

#endif
