/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 陳一        */
/*Editor: 黃柏瑞      */
/**********************/

#ifndef _GPIO_H
#define _GPIO_H

#include "stm32f10x.h"

#define ON 0
#define OFF 1

/******************************************************************/
/*Function: initialize GPIO bits as digital input                 */
/*Variables: gpiox:GPIOA/B/C..., pinRead: GPIO_Pin_1/2... or All  */
/*Return: void                                                    */
/******************************************************************/
void init_gpio_read(GPIO_TypeDef * gpiox, uint16_t pinRead);

/******************************************************************/
/*Function: initialize GPIO bits as digital output                */
/*Variables: gpiox:GPIOA/B/C..., pinRead: GPIO_Pin_1/2... or All  */
/*Return: void                                                    */
/******************************************************************/
void init_gpio_write(GPIO_TypeDef * gpiox, uint16_t pinWrite);

/**********************************************************************************/
/*Function: set GPIO output as on or off                                          */
/*Variables: gpiox:GPIOA/B/C..., pinRead: GPIO_Pin_1/2... or All, set: ON or OFF  */
/*Return: void                                                                    */
/**********************************************************************************/
void set_gpio(GPIO_TypeDef * gpiox, uint16_t pinWrite, int set);

/******************************************************************/
/*Function: scan GPIO bits                                        */
/*Variables: gpiox:GPIOA/B/C..., pinRead: GPIO_Pin_1/2... or All  */
/*Return: On or OFF                                               */
/******************************************************************/
uint8_t scan_gpio(GPIO_TypeDef* gpiox, uint16_t pinWrite);

#endif
