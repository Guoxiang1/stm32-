#ifndef _BC25_H_
#define _BC25_H_
#include "sys.h"






void BC25_GPIO_INIT(void);
void NB_RST(void);
void BC25_QSCLK(void);
void BC25_CPSMS(void);
void BC25_NNMI(void);
void BC25_CIMI(void); //���NB����
void BC25_CGATT(void);//��������״̬
void BC25_CGPADDR(void);//��ѯ�Ƿ�ע��
uint8_t BC25_INIT(void);

#endif



