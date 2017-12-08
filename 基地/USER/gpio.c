#include "gpio.h"
#include "stm32f10x.h"

static void gpio_init(gpio_init_t *gpio)
{
	GPIO_InitTypeDef						GPIO;
	if(gpio->mode==GPIO_Mode_AF_PP)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(gpio->rcc, ENABLE);
  GPIO.GPIO_Mode=gpio->mode;
	GPIO.GPIO_Speed=gpio->speed;
	GPIO.GPIO_Pin=gpio->pin;//按板子定
	GPIO_Init(gpio->gpio,&GPIO);
}


#define key_set 0
static void Scan(gpio_init_t *gpio,hw *hw)
{
	if(hw->readinputdatabit(gpio->gpio,gpio->pin)==key_set)
		{
			if(hw->time>4)
				hw->status=1;
			else	
				hw->time+=1;
		}
	else
		{
			hw->time=0;
			hw->status=0;
		}
}




//gpio初始化变量
gpio_init_t \
gpio_hw1=		{RCC_APB2Periph_GPIOE,	GPIOE,GPIO_Pin_4,  	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw2=		{RCC_APB2Periph_GPIOC,	GPIOC,GPIO_Pin_14, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw3=		{RCC_APB2Periph_GPIOC,	GPIOC,GPIO_Pin_0, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw4=		{RCC_APB2Periph_GPIOE,	GPIOE,GPIO_Pin_6, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw5=		{RCC_APB2Periph_GPIOC,	GPIOC,GPIO_Pin_2, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw6=		{RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_2, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw7=		{RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_4, 	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_hw8=		{RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_0,  GPIO_Speed_50MHz,	GPIO_Mode_IPU};

gpio_init_t \
gpio_can1r ={RCC_APB2Periph_GPIOB,	GPIOB,GPIO_Pin_8,	GPIO_Speed_50MHz,	GPIO_Mode_IPU},\
gpio_can1t=	{RCC_APB2Periph_GPIOB,	GPIOB,GPIO_Pin_9,	GPIO_Speed_50MHz,	GPIO_Mode_AF_PP},\
gpio_usartr={RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_10,	GPIO_Speed_50MHz,	GPIO_Mode_IN_FLOATING},\
gpio_usartt={RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_9,	GPIO_Speed_50MHz,	GPIO_Mode_AF_PP},\
gpio_LED1={RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_7,	GPIO_Speed_50MHz,	GPIO_Mode_Out_PP},\
gpio_LED2={RCC_APB2Periph_GPIOA,	GPIOA,GPIO_Pin_8,	GPIO_Speed_50MHz,	GPIO_Mode_Out_PP};

//红外初始化变量
hw \
hw1={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw2={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw3={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw4={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw5={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw6={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw7={0,0,&Scan,&GPIO_ReadInputDataBit},\
hw8={0,0,&Scan,&GPIO_ReadInputDataBit};

void whole_Scan(void)
{
	Scan(&gpio_hw1,&hw1);
	Scan(&gpio_hw2,&hw2);
	Scan(&gpio_hw3,&hw3);
	Scan(&gpio_hw4,&hw4);
	Scan(&gpio_hw5,&hw5);
	Scan(&gpio_hw6,&hw6);
	Scan(&gpio_hw7,&hw7);
	Scan(&gpio_hw8,&hw8);
}

void Bsp_GPIO_Init(void)
{
	//时钟afio开启集成在gpio_init内，所以说不需要在这里开启
	//如要添加pinremapconfig需在内添加相应函数
	//红外传感器gpio初始化
	gpio_init(&gpio_hw1);
	gpio_init(&gpio_hw2);
	gpio_init(&gpio_hw3);
	gpio_init(&gpio_hw4);
	gpio_init(&gpio_hw5);
	gpio_init(&gpio_hw6);
	gpio_init(&gpio_hw7);
	gpio_init(&gpio_hw8);	
	//can总线gpio初始化
	gpio_init(&gpio_can1r);
	gpio_init(&gpio_can1t);
	//串口gpio初始化
	gpio_init(&gpio_usartr);
	gpio_init(&gpio_usartt);
	gpio_init(&gpio_LED1);
	gpio_init(&gpio_LED2);
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
}

