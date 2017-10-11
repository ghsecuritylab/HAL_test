#include "lwip/debug.h"
#include "lwip/apps/httpd.h"
#include "lwip/tcp.h"
#include "fs.h"

#include <string.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#define NUM_CONFIG_CGI_URIS	1 //CGI��URI����
#define NUM_CONFIG_SSI_TAGS	2  //SSI��TAG����

//����LED��BEEP��CGI handler
const char* Login_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//const char* Set_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//const char* Ctrl_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const tCGI ppcURLs[]= //cgi����
{
	{"/login.cgi",Login_CGI_Handler},
//	{"/set.cgi",Set_CGI_Handler},
//	{"/ctrl.cgi",Ctrl_CGI_Handler}
};
static const char *ppcTAGs[]=  //SSI��Tag
{
	"w", //�¶�ֵ
	"t"
};


//SSIHandler����Ҫ�õ��Ĵ���ADC�ĺ���
void ADC_Handler(char *pcInsert)
{ 
    uint32_t temp = 0;   
	temp = HAL_GetTick();
		sprintf(pcInsert,"%d,%d",temp/1000,temp%1000);
}



//SSI��Handler���
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 

				break;
		case 1:
					ADC_Handler(pcInsert);
			break;
	}
	return strlen(pcInsert);
}


//SSI�����ʼ��
void httpd_ssi_init(void)
{  
	//����SSI���
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//��web�ͻ��������������ʱ��,ʹ�ô˺�����CGI handler����
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //����iLOOP
		}
	}
	return (-1);
}

const char *Login_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])

{

  uint8_t i=0; //???????????????i???
      uint8_t passchack=0;
      iIndex =FindCGIParameter("username",pcParam,iNumParams);  //??bktime????
      if(iIndex != -1) //??pagingvol???
      {
           for(i = 0;i < iNumParams;i++)
           {
                 if(strcmp(pcParam[i],"username")== 0)  //??CGI??
                 {
                      if(strcmp(pcValue[i],"admin")== 0)//?????
                      {
                            passchack++;
                      }
                 }
                 else if(strcmp(pcParam[i],"password") == 0)  //??CGI??
                 {
                      if(strcmp(pcValue[i],"admin")== 0)//????
                      {
                            passchack++;
                      }
                 }
           }
           if(passchack > 1)//??????????
           {
                 return "/menu.html";     //????????????????????
           }
      }
      return "/index.html";      //????????????????????
}

//CGI�����ʼ��
void httpd_cgi_init(void)
{ 
  //����CGI���
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}
