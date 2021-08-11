#include "NRF24L01.h"

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x31,0x4E,0x6F,0x64,0x65}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x31,0x4E,0x6F,0x64,0x65}; //接收地址

void NRF_Init(void){
	//GPIO settings for CS/CE pin
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitTypeDef GPIO_InitStructure;
	//CE: PB0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  //GPIO_SetBits(GPIOB,GPIO_Pin_0);
	//CSN: PB1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  //GPIO_SetBits(GPIOB,GPIO_Pin_0);
	//IRQ: PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  //GPIO_SetBits(GPIOB,GPIO_Pin_10);
	
	//SPI settings
	SPI_Config(SPI_Mode_Master, 0, SPI_BaudRatePrescaler_8);

  GPIO_ResetBits(GPIOB, GPIO_Pin_0);//CE: Enable the nrf
	GPIO_SetBits(GPIOB, GPIO_Pin_1);//CS: cancel the chip selection
}

u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;  
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//Select nrf
	SPI_Send(reg);//发送寄存器号
  status = SPI_Receive();
	SPI_Send(value); //写入寄存器的值
	GPIO_SetBits(GPIOB, GPIO_Pin_1);//禁止SPI传输      
	return(status);                 //返回状态值
}

u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;     
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//使能SPI传输        
	SPI_Send(reg);//发送寄存器号
	SPI_Send(0XFF);//读取寄存器内容
  reg_val = SPI_Receive();
	GPIO_SetBits(GPIOB, GPIO_Pin_1);//禁止SPI传输            
	return(reg_val);           //返回状态值
} 

//在指定位置读出指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
  u8 status,u8_ctr;          
  GPIO_ResetBits(GPIOB, GPIO_Pin_1); //使能SPI传输
	SPI_Send(reg);//发送寄存器值(位置),并读取状态值  
  status = SPI_Receive();
  for(u8_ctr=0;u8_ctr<len;u8_ctr++){
		SPI_Send(0XFF);
    pBuf[u8_ctr]=SPI_Receive();//读出数据
	}
  GPIO_ResetBits(GPIOB, GPIO_Pin_1); //关闭SPI传输
  return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
  u8 status,u8_ctr;       
  GPIO_ResetBits(GPIOB, GPIO_Pin_1);//使能SPI传输
  SPI_Send(reg);//发送寄存器值(位置),并读取状态值  
  status = SPI_Receive();
  for(u8_ctr=0; u8_ctr<len; u8_ctr++){
    SPI_Send(*pBuf++); //写入数据
		//printf("%d",flag);
		for(int i=0;i<5;i++){}
	}
  GPIO_SetBits(GPIOB, GPIO_Pin_1);//关闭SPI传输
  return status;          //返回读到的状态值
}

//const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
//const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了          
void NRF24L01_RX_Mode(void)
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_0);    
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
  //写RX节点地址  
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址     
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);           //设置RF通信频率        
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
  //即32个字节
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  //向CONFIG(00)写入0000 1111;PRIM_RX=1 接收模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC =1 CRC使能
  GPIO_SetBits(GPIOB, GPIO_Pin_0); //CE为高,进入接收模式 
}     

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了          
//CE为高大于10us,则启动发送.  
void NRF24L01_TX_Mode(void)
{                                                        
  GPIO_ResetBits(GPIOB, GPIO_Pin_0);       
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);
  //写TX节点地址 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 
  //设置TX节点地址,主要为了使能ACK      
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
  //向CONFIG(00)写入0000 1111;PRIM_RX=0 发射模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC   =1 CRC使能
  GPIO_SetBits(GPIOB, GPIO_Pin_0); //CE为高,10us后启动发送
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;  
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
	GPIO_SetBits(GPIOB, GPIO_Pin_0);//启动发送       
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)!=0);//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
	  return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

//启动NRF24L01接收一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;                                           
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值      
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}      
	return 1;//没收到任何数据
}

//检测24L01是否存在
//返回值:0，成功;1，失败 
u8 NRF24L01_Check(void)
{
    u8 buf[5]={0XAA,0XAA,0XAA,0XAA,0XAA};//写入5个0XAA字节
    u8 i;    
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.    
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
    for(i=0;i<5;i++)
        if(buf[i]!=0XAA)
            break;                                 
    if(i!=5)return 1;//检测24L01错误    
    return 0;        //检测到24L01
} 
