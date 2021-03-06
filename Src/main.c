/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/**
 * Only read the Temp and Humidity + voltage
 * Cahnge - SW will be updated with some other features.
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "pinmap.h"
#include "peripherals.h"
#include "lcd_12864.h"
#include "BME280.h"
#include "ds18b20.h"
#include <stdio.h>
#include "usbd_cdc_if.h"
#include "rtc_api.h"
#include "Time.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

static States_loop current_state;
static Screen show;
Flags_main flags;
uint32_t citac=0;

static Buttons pushed_button; //cleared each main cycle

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

	/* USER CODE BEGIN 1 */
	char buffer_s [32];
	char buffer_usb [32];

	uint16_t T_led=20;

	uint32_t temperature=-20000;
	uint32_t humid = -5;
	uint32_t presure = 0;
	uint32_t actual_HALtick=0;
	uint32_t InputVoltage=0;
	int16_t count1;
	char aShowTime[50] = {0};
	//debug
	uint8_t beta2_part=12;
	Bool alfa_part=0, beta_part=0, show_time=TRUE;
	//debug

	//timeouts
	uint32_t backlite_compare, measure_compare, led_compare,time_compare, button_compare;

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_TIM22_Init();
//	MX_TIM21_Init();
	MX_I2C1_Init();
	MX_USB_DEVICE_Init();
	MX_RTC_Init();
	MX_ADC_Init();

	/* USER CODE BEGIN 2 */
	lcd12864_init(&hspi1);
	line(0,60,110,60,1);
	lcd_setCharPos(0,0);
	lcd_printString("Initialization unit\r");
	lcd_printString("Reading Temp_Hum\r");
	lcd_printString( "SW v 0.172");
	HAL_TIM_Encoder_Start(&htim22,TIM_CHANNEL_1);

	htim22.Instance->EGR = 1;           // Generate an update event
	htim22.Instance->CR1 = 1;           // Enable the counter

	BME280_init(&hi2c1,DEFAULT_SLAVE_ADDRESS); // initialization of temp/humid sensor BOSH

	HAL_Delay(1700);
	lcd_clear();

	current_state = MEASURING;
	show=desktop;
	//init timers
	led_compare = fill_comparer(LED_PERIODE);
	measure_compare = fill_comparer(MEASURE_PERIODE);
	time_compare = fill_comparer(TIME_PERIODE);
	button_compare = fill_comparer(BUT_DELAY);

	HAL_GPIO_WritePin(D_LCD_LIGHT_GPIO_Port,D_LCD_LIGHT_Pin,GPIO_PIN_SET);
	backlite_compare = fill_comparer(BACKLITE_TIMEOUT);
// NEW ADDED
	/*##- 4- Start the conversion process #######################################*/
	  if (HAL_ADC_Start(&hadc) != HAL_OK)
	  {
	    /* Start Conversation Error */
	    Error_Handler();
	  }

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	for (;;)
	{
		switch (current_state){
		case SLEEP:
		{

			snprintf(buffer_usb, 15, "SLEEP \n\r");
			CDC_Transmit_FS(buffer_usb,15);
CDC_Transmit_FS(aShowTime,15);
			snprintf(buffer_usb, 15, "\n\r %s \n\r", aShowTime);
			CDC_Transmit_FS(buffer_usb,14);

/*			RTC_TimeShow(&hrtc,aShowTime);
			snprintf(buffer_usb, 15, "%s \n\r", aShowTime);
			CDC_Transmit_FS(buffer_usb,13);
*/
			/* Disable all used wakeup sources*/
			HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
			/* Re-enable all used wakeup sources*/
			/* ## Setting the Wake up time ############################################*/
			/*  RTC Wakeup Interrupt Generation:
		  				    Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI))
		  				    Wakeup Time = Wakeup Time Base * WakeUpCounter
		  				      = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI)) * WakeUpCounter
		  				      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

		  				    To configure the wake up timer to 4s the WakeUpCounter is set to 0x1FFF:
		  				    RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16
		  				    Wakeup Time Base = 16 /(~39.000KHz) = ~0,410 ms
		  				    Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
		  				      ==> WakeUpCounter = ~4s/0,410ms = 9750 = 0x2616 */

			if ( HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 9250, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
			{
				lcd_setCharPos(2,1);
				lcd_printString(" ERR - no wake set" );
				HAL_Delay(2500);
			}
			/* Clear all related wakeup flags */
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

			//*Suspend Tick increment to prevent wakeup by Systick interrupt.Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)
			HAL_SuspendTick();

			// Enable Power Control clock
			__HAL_RCC_PWR_CLK_ENABLE();

			// Enter Sleep Mode , wake up is done once User push-button is pressed
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

			//* Resume Tick interrupt if disabled prior to sleep mode entry
			HAL_ResumeTick();


			snprintf(buffer_usb, 15, "RESUME \n\r");
			CDC_Transmit_FS(buffer_usb,10);
			RTC_TimeShow(&hrtc,aShowTime);
			CDC_Transmit_FS(aShowTime,15);
			snprintf(buffer_usb, 15, "\n\r %s \n\r", aShowTime);
			CDC_Transmit_FS(buffer_usb,14);

			current_state = MEASURING;
			show_time=TRUE;
			break;
		}
		case IDLE:
		{
			break;
		}
		case MEASURING:
		{

			temperature=BME280_getTemperature();
			humid=BME280_getHumidity();
			presure=BME280_getPressure();

			current_state = VOLTAGE;

			flags.new_data_to_show=TRUE;

			measure_compare = fill_comparer(MEASURE_PERIODE);
			break;
		case VOLTAGE:

		{
		    /*##- 5- Wait for the end of conversion #####################################*/
		    /*  Before starting a new conversion, you need to check the current state of
		         the peripheral; if it�s busy you need to wait for the end of current
		         conversion before starting a new one.
		         For simplicity reasons, this example is just waiting till the end of the
		         conversion, but application may perform other tasks while conversion
		         operation is ongoing. */
		    HAL_ADC_PollForConversion(&hadc, 10);

		    /* Check if the continous conversion of regular channel is finished */
		    if ((HAL_ADC_GetState(&hadc) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
		    {
		      /*##-6- Get the converted value of regular channel  ########################*/
		      InputVoltage = HAL_ADC_GetValue(&hadc);
		    }
		  }

		flags.new_data_to_show=TRUE;

		current_state = SLEEP;
			break;
		case LOG:
		{

			break;
		}
		default:
			break;
		}
		}// switch CURRENT STATE


	/*SCREEN*/
		switch (show){

		case blind:
		{
			lcd_clear();
			show = idle;
			break;
		}
		case desktop:
		{
			if (flags.new_data_to_show==TRUE){

				// BME280 sensor


				lcd_setCharPos(1,4);
				char_magnitude(2);
				snprintf(buffer_s, 12, "%d.%02d C",temperature/100,temperature%100);
				lcd_printString(buffer_s);

				lcd_setCharPos(4,4);
				snprintf(buffer_s, 14, "%d.%02d %%",(humid / 1024), humid%1024*100/1024);
				lcd_printString(buffer_s);
				char_magnitude(1);

			/*	lcd_setCharPos(6,4);
				snprintf(buffer_s, 18, "Pres %d.%02d hp",presure/100,presure%100);
				lcd_printString(buffer_s);
				*/
				lcd_setCharPos(6,0);
				snprintf(buffer_s, 20, "%d - %d.%02d V",InputVoltage,InputVoltage*66/2550,(InputVoltage*66%2550*100/255) );
								lcd_printString(buffer_s);


				flags.new_data_to_show=FALSE; // the data was showed.

				//debug

				lcd_setCharPos(2,20);
				if (alfa_part){
					lcd_printString("X");
					alfa_part = FALSE;
				}
				else{
					lcd_printString("_");
					alfa_part = TRUE;
				}
				//debug

			}// end if - new data to show
			if (show_time){

				RTC_DateShow(&hrtc,aShowTime);
				lcd_setCharPos(7,10);
				lcd_printString(aShowTime);
				RTC_TimeShow(&hrtc,aShowTime);
				//lcd_setCharPos(0,12);
				lcd_setCharPos(0,1);
				lcd_printString(aShowTime);

				//debug
				lcd_setCharPos(0,beta2_part);
				if (beta_part){
					lcd_printString("T");
					beta_part = FALSE;
				}
				else {
					lcd_printString("_");
					beta_part = TRUE;
				}
				beta2_part++;
				if (beta2_part>20){
					beta2_part=12;
				}
				lcd_setCharPos(7,0);
				snprintf(buffer_s, 18, "%d",hrtc.Instance->TR);
				lcd_printString(buffer_s);

				//debug
				/* end of the time part - new timer set.*/
				time_compare = fill_comparer(TIME_PERIODE);
				show_time = FALSE;
			}


			break;
		}
		case idle:
		{
			break;
		}
		case debug:
		{
			lcd_clear();
			RTC_DateShow(&hrtc,aShowTime);
			lcd_setCharPos(1,10);
			lcd_printString(aShowTime);

			RTC_TimeShow(&hrtc,aShowTime);
			lcd_setCharPos(1,0);
			lcd_printString(aShowTime);

			//CITAC
			uint32_t sekundy = (SlaveCounter << 16)/1000000;

			lcd_setCharPos(4,1);
			snprintf(buffer_s, 20, "%03d:%02d", sekundy/60,sekundy%60 );
			lcd_printString(buffer_s);

			// reading the actual number from encoder

			count1=htim22.Instance->CNT;
			lcd_setCharPos(5,0);
			snprintf(buffer_s, 25, "s %i \r t %i ms", (count1+T_led)*MAIN_LOOP*2, MAIN_LOOP*2*T_led);
			lcd_printString(buffer_s);

			lcd_setCharPos(7,0);
			snprintf(buffer_s, 9, "EN%03i", (count1));
			lcd_printString(buffer_s);

			HAL_Delay(500);
			break;
		}
		default:{
			lcd_setCharPos(1,1);
			lcd_printString("DEFAULT   DEFFFF" );
		}
		}// switch show

		/* *------ TIME ELAPSING CHECK -------* */
		actual_HALtick = HAL_GetTick();
		if(measure_compare <= actual_HALtick) //measure after defined periode.
		{
			current_state = MEASURING;
		}

		if(time_compare <= actual_HALtick) //measure after defined periode.
		{
			show_time = TRUE;
		}
		/*
		if (led_compare<=actual_HALtick){
			togleLED(LED1);
			led_compare=fill_comparer(LED_PERIODE);
		}//if BLIK LEDka
		 */
		if (backlite_compare <= actual_HALtick) // shut down the backlight
		{
			HAL_GPIO_WritePin(D_LCD_LIGHT_GPIO_Port,D_LCD_LIGHT_Pin,GPIO_PIN_RESET);
		}

		/* -- READ KEYBOARD --- */

		pushed_button = BUT_NONE;
		//if(isTimeout(TIM_BUT_SCAN))
		if(button_compare<=actual_HALtick) //every delay time
		{
			pushed_button = checkButtons();
			button_compare = fill_comparer(BUT_DELAY);

		}
		if(pushed_button != BUT_NONE) // any button pushed?
		{
			HAL_GPIO_WritePin(D_LCD_LIGHT_GPIO_Port,D_LCD_LIGHT_Pin,GPIO_PIN_SET);
			button_compare = fill_comparer(BUT_DELAY*4); // 4 - zpodeni cteni pri stisknuti
			backlite_compare = fill_comparer(BACKLITE_TIMEOUT);
		}

		// -- BUTTON PROCCESS
		switch (pushed_button){
		case BUT_1:
		{
			led_compare = 0xffffffff;
			Led1Clear;
			Led2Clear;
			HAL_GPIO_WritePin(D_LCD_LIGHT_GPIO_Port,D_LCD_LIGHT_Pin,GPIO_PIN_RESET);
			lcd_clear();
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
			break;
		}
		case BUT_2:
		{

			current_state = SLEEP;

			break;
		}
		case BUT_ENC:
		{
			//show = debug;
			break;
		}
		} // switch pushed button

		HAL_Delay(MAIN_LOOP);


		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_12;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC
			|RCC_PERIPHCLK_USB;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
		togleLED(LED2);
		HAL_Delay(500);
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
