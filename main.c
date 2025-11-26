/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
volatile uint32_t rpm_tick_count = 0;
volatile uint32_t last_tick_time = 0;
volatile uint32_t rpm_tick_count_diff = 0;
volatile uint32_t rpm = 0;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t byte;

uint8_t hour[2]; // Buffer for two characters of the hour
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

volatile uint8_t clock_hours = 0;
volatile uint8_t clock_mins = 0;
volatile uint8_t clock_secs = 0;
volatile uint8_t wall_clock_hr_update_flag = 0;

int pipeline_stage = 0;  // Current pipeline stage
int pipeline_positions[] = {1000, 1500, 2000, 2500};

uint8_t txd_msg_buffer[64] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

static void ADC_Select_CH(int CH);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ADC_Select_CH(int CH)
{
ADC_ChannelConfTypeDef sConfig = {0};
switch(CH)
{
case 0:
sConfig.Channel = ADC_CHANNEL_0;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 1:
sConfig.Channel = ADC_CHANNEL_1;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 2:
sConfig.Channel = ADC_CHANNEL_2;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 3:
sConfig.Channel = ADC_CHANNEL_3;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 4:
sConfig.Channel = ADC_CHANNEL_4;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 5:
sConfig.Channel = ADC_CHANNEL_5;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 6:
sConfig.Channel = ADC_CHANNEL_6;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 7:
sConfig.Channel = ADC_CHANNEL_7;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 8:
sConfig.Channel = ADC_CHANNEL_8;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 9:
sConfig.Channel = ADC_CHANNEL_9;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 10:
sConfig.Channel = ADC_CHANNEL_10;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 11:
sConfig.Channel = ADC_CHANNEL_11;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 12:
sConfig.Channel = ADC_CHANNEL_12;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 13:
sConfig.Channel = ADC_CHANNEL_13;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 14:
sConfig.Channel = ADC_CHANNEL_14;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
case 15:
sConfig.Channel = ADC_CHANNEL_15;
sConfig.Rank = 1;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
{
Error_Handler();
}
break;
}
}

int ADC_poll(){

	ADC_Select_CH(7);
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 1000);
	  uint8_t ADC_CH7 = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);
	return ADC_CH7;
}



//0 = purple, 1 = red, 2 = green, 3 = blue
void setledpin(int channel){
	switch(channel){
	case 0:
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin | GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin, GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin | GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin | GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RED_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin | GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RED_GPIO_Port, BLUE_Pin, GPIO_PIN_SET);
		break;
	default:
		//error (only happens if channel val>3).
		HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin | BLUE_Pin | GREEN_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(RED_GPIO_Port, BLUE_Pin|RED_Pin|GREEN_Pin, GPIO_PIN_SET);
		break;
	}
}

void pipeMeter(int channel) {
	TIM2->CCR1 = pipeline_positions[channel];
}

uint8_t rcv_intpt_flag = 0;
void get_rpm(void) {
    // Calculate RPM only if we have more than 1 tick (at least 1 full rotation)
    if (rpm_tick_count >= 20) {  // Only calculate RPM once 20 ticks have been counted
        uint32_t current_time = HAL_GetTick();  // Current time in ms
        rpm_tick_count_diff = current_time - last_tick_time;  // Time difference in ms

        // Calculate RPM if the time difference is valid
        if (rpm_tick_count_diff > 0) {
            // RPM = (60,000 ms per minute) / (time for 1 full rotation in ms)
            rpm = 60000 / (rpm_tick_count_diff);  // Divide by 20 to account for 20 slots per rotation
        } else {
        	rpm = 0;
        }

        // Update the last tick time
        last_tick_time = current_time;

        // Reset the tick count for the next calculation cycle
        rpm_tick_count = 0;
    }

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Init(&htim2);
  	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    int TIM2_Ch1_DCVAL = 500;
    int TIM2_Ch1_STEP = 20;

    TIM2->PSC = 16-1;
    TIM2->ARR = 20000-1;
    TIM2->CCR1 = TIM2_Ch1_DCVAL;
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    int TIM2_Ch2_DCVAL = 0;
    int TIM2_Ch3_DCVAL = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  pipeMeter(0);
	  	  sprintf((char*)txd_msg_buffer, "\r\nEnter SETUP Parameters\r\n");
	  	  HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	  //SETUP MODE BEGIN
	  	  uint8_t pipes[4];
	  	  int pumpPwms[4] = {0, 0, 0, 0};
	  	  int pwmSelect[4] = {0, 75, 85, 99};
	  	  int pwmRatios[4] = {0, 0, 0, 0};
	  	  int first_hours[4] = {0, 0, 0, 0};
	  	  int last_hours[4] = {0, 0, 0, 0};

	  	  //Get setup params
	  	  for(int i = 0; i < 4; i++) {
	  		  sprintf((char*)txd_msg_buffer, "\r\nPIPELINE (options: 0 to 3): ");
	  		  rcv_intpt_flag = 00;
	  		  HAL_UART_Receive_IT(&huart2,&byte,1);
	  		  HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  		  while(rcv_intpt_flag == (00)) {}
	  		  pipes[i] = byte - 48;
	  		  uint8_t pipeSelect = byte - 48;

	  		  sprintf((char*)txd_msg_buffer, "\r\nPump PWM (options 0 to 3): ");
	  		  rcv_intpt_flag = 00;
	  		  HAL_UART_Receive_IT(&huart2,&byte,1);
	  		  HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  		  while(rcv_intpt_flag == (00)) {}
	  		  pumpPwms[pipeSelect] = byte - 48;
	  		  pwmRatios[pipeSelect] = pwmSelect[byte - 48];
	  	  }
	  	  //sprintf((char*)txd_msg_buffer, "\r\nLOOP DONE %lu", byte - 48);
	  	   //HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	  	  int pipe_times[4][3]; //4 slot array of 3 slot arrays
	  	  //each 3 slot array is of form: CHANNEL NAME, START TIME, END TIME

	  	  for(int i = 0; i < 4; i ++) {
	  	      int time_info[3];
	  	      time_info[0] = i;

	  		  sprintf((char*)txd_msg_buffer, "\r\nPipeline %d Pump FIRST HOUR (options: 00 to 23):  ", i);

	  			  HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  			  rcv_intpt_flag = 00;
	  			  HAL_UART_Receive_IT(&huart2,&byte,1);
	  			  while(rcv_intpt_flag == (00)) {}
	  			  first_hours[i] += (byte - 48) * 10;
	  			  time_info[1] = (byte - 48) * 10;

	  			  rcv_intpt_flag = 00;
	  			  HAL_UART_Receive_IT(&huart2,&byte,1);
	  			  while(rcv_intpt_flag == (00)) {}
	  			  first_hours[i] += byte - 48;
	  			  time_info[1] += byte - 48;


	  	        sprintf((char*)txd_msg_buffer, "\r\nPipeline %d Pump LAST HOUR (options: 00 to 23):  ", i);

	  			HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	        rcv_intpt_flag = 00;
	  	        HAL_UART_Receive_IT(&huart2,&byte,1);
	  	        while(rcv_intpt_flag == (00)) {}
	  	        last_hours[i] = (byte - 48) * 10;
	  	        time_info[2] = (byte - 48) * 10;

	  	        rcv_intpt_flag = 00;
	  	        HAL_UART_Receive_IT(&huart2,&byte,1);
	  	        while(rcv_intpt_flag == (00)) {}
	  	        last_hours[i] += byte - 48;
	  			time_info[2] += byte - 48;

	  			memcpy(pipe_times[i], time_info, sizeof(time_info));
	  	        // You can use 'hour' for further processing, like validation or calculation
	  	    }
	  /*
	  	  for(int i = 0; i < 4; i++) {
	  	  		sprintf((char*)txd_msg_buffer, "\r\npipe %d, start: %d, end %d\r\n", pipe_times[i][0], pipe_times[i][1], pipe_times[i][2]);
	  	  		HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	  	  }
	  */
	  	  //sort pipe_times
	  	  for(int i = 0; i < 3; i++) {
	  		  for(int j = i; j < 3; j ++) {
	  			  if(pipe_times[j][1] > pipe_times[j + 1][1]) {
	  				  int temp[3];
	  				  memcpy(temp, pipe_times[j], sizeof(pipe_times[j]));
	  				  memcpy(pipe_times[j], pipe_times[j + 1], sizeof(pipe_times[j + 1]));
	  				  memcpy(pipe_times[j + 1], temp, sizeof(temp));
	  			  }
	  		  }
	  	  }

	  /*
	  	  for(int i = 0; i < 4; i++) {
	  	  	  		sprintf((char*)txd_msg_buffer, "pipe %d, start: %d, end %d\r\n", pipe_times[i][0], pipe_times[i][1], pipe_times[i][2]);
	  	  	  		HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	  	  	  }
	  */
	  	   sprintf((char*)txd_msg_buffer, "\r\n\r\n			Printing SETUP Parameters\r\n\r\n\r\n");
	  	   HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	   for(int i = 0; i < 4; i++) {

	  	       sprintf((char*)txd_msg_buffer, "PIPELINE: %d    		Pump PWM: %d    		", i, pumpPwms[i]);
	  	       HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	       sprintf((char*)txd_msg_buffer, "Pump FIRST HOUR: %d    		Pump LAST HOUR: %d\r\n",first_hours[i], last_hours[i]);
	  	       HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  	   }


	  		  sprintf((char*)txd_msg_buffer, "\r\n\r\nSETUP is done. Press Blue Button for RUN MODE");
	  		  	   HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	  		  	   GPIO_PinState buttonState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  		  	   volatile int flag = GPIO_PIN_SET;
	  		  	   while(buttonState != GPIO_PIN_RESET) {
	  		  		   buttonState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  		  		   if(flag == GPIO_PIN_SET) {
	  		  			   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	  		  			   flag = GPIO_PIN_RESET;
	  		  		   } else {
	  		  			   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	  		  			   flag = GPIO_PIN_SET;
	  		  		   }
	  		  		   HAL_Delay(125);
	  		  	   }
	  		  	   int zeroRatio = ADC_poll() * 100 / 233;

	  		  	   for(int i = 0; i < 4; i++) {
	  		  		   if(pwmRatios[i] == 0) {
	  		  			 pwmRatios[i] = zeroRatio;
	  		  		   }
	  		  	   }

	  		  	   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	  		  	   sprintf((char*)txd_msg_buffer, "\r\n\r\n\r\n			RUN MODE\r\n");
	  		  	   	   HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  		  	   // SETUP MODE END

	  		  	   // RUN MODE START
	  		  	   	   for(int i = 0; i < 4; i++) {
	  		  	   		sprintf((char*)txd_msg_buffer, "pwmRatios[%d] = %d\r\n", i, pwmRatios[i]);
	  		  	   		HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  		  	   	   }
	  		  	   sprintf((char*)txd_msg_buffer, "\r\n CLOCK : PIPE : PWM : RPM : DEPTH :\r\n");
	  		  	   HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  		  	   sprintf((char*)txd_msg_buffer, "---------------------------------------------\r\n");
	  		  	   	   HAL_UART_Transmit(&huart2,txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	  		  	       clock_hours = 0;
	  		  	       clock_mins = 0;
	  		  	       clock_secs = 0;
	  			  	   HAL_TIM_Base_Start_IT(&htim5);
	  			  	   int current = 0;
	  			  	 TIM2->CCR3 = 600 * pwmRatios[0];
	  			  	 setledpin(0);
	  		  	   	while(clock_hours <= 26) {
	  		  	   	get_rpm();
	  			  	   	if(wall_clock_hr_update_flag && clock_mins == 0 && clock_hours < 26 && clock_secs == 0) {
	  			  	   			  sprintf((char*)txd_msg_buffer, " %d	:", clock_hours - 1);
	  			  	   			  HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  			  	   			  wall_clock_hr_update_flag = 0;

	  			  	   			  int current_channel = pipe_times[current][0];
	  			  	   			  int is_invalid = 0;
	  			  	   			  if(clock_hours - 1 >= pipe_times[current][1] && clock_hours - 1<= pipe_times[current][2]) {
	  				  	   			  sprintf((char*)txd_msg_buffer, "	%d  :", pipe_times[current][0]);
	  				  	   			  HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  				  	   			  //TIM2->CCR2 = 600 * pwmSelect[pipe_times[current][0]];
	  			  	   			  } else if(clock_hours - 1>= pipe_times[current + 1][1] && clock_hours <= pipe_times[3][2]) {
	  				  	   			  //sprintf((char*)txd_msg_buffer, "WE ARE MOVING TO THE NEXT PIPE\r\n");
	  				  	   			  //HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  			  	   				  TIM2->CCR3 = 0;
	  			  	   				  current++;
	  			  	   				current_channel = pipe_times[current][0];
	  				  	   			  sprintf((char*)txd_msg_buffer, "	%d  :", pipe_times[current][0]);
	  				  	   			  TIM2->CCR2 = 600 * pwmRatios[pipe_times[current][0]];
	  				  	   			  HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  				  	   			  setledpin(current_channel);
	  				  	   			  pipeMeter(current_channel);
	  			  	   			  } else {
	  				  	   			  sprintf((char*)txd_msg_buffer, "	   :", pipe_times[current][0]);
	  				  	   			  HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  				  	   			  is_invalid++;
	  				  	   			  TIM2->CCR3 = 0;
	  				  	   			  TIM2->CCR2 = 0;
	  				  	   			  setledpin(4);
	  			  	   			  }
	  			  	   			  //get_rpm();

	  			  	   			if(!is_invalid) {
	  				  	   			sprintf((char*)txd_msg_buffer, "  %d  :\r\n", pumpPwms[current_channel]);
	  				  	   			HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  			  	   			} else {
	  				  	   			sprintf((char*)txd_msg_buffer, "     :\r\n", pumpPwms[current_channel]);
	  				  	   			HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	  			  	   			}
	  			  	   		sprintf((char*)txd_msg_buffer, " We have CCR2 value of: %d, PWM Percentage: %d\r\n", TIM2->CCR2, pwmRatios[pipe_times[current][0]]);
	  			  	   		HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
	  			  	   		if(is_invalid) {
	  			  	   			rpm = 0;
	  			  	   		} else {
	  				  	   		//get_rpm();
	  			  	   		}
	  				        sprintf((char*)txd_msg_buffer, " Current RPM: %lu \r \n", rpm);
	  				        HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	  			  	   	}


	  		  	   	}
	  		  	   	TIM2->CCR2 = 0;
	  		  	   	pipeMeter(0);
	    	   			sprintf((char*)txd_msg_buffer, "\r\n\r\n     irrigation complete \r\n");
	    	   			HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);

	    	   			while(1) {};
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = (16000-1)/300;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BLUE_Pin|RED_Pin|GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BLUE_Pin RED_Pin GREEN_Pin */
  GPIO_InitStruct.Pin = BLUE_Pin|RED_Pin|GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : HCSR04_TRIG_Pin */
  GPIO_InitStruct.Pin = HCSR04_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HCSR04_TRIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RPM_TICK_Pin */
  GPIO_InitStruct.Pin = RPM_TICK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RPM_TICK_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART2) {
		HAL_UART_Transmit(&huart2, &byte, 1, 100);
		rcv_intpt_flag = 1;
	}
}

void get_hour_input(uint8_t *hour) {
    // Receive the first character (tens place)
    rcv_intpt_flag = 00;
    HAL_UART_Receive_IT(&huart2, &hour[0], 1); // Receive first digit
    while(rcv_intpt_flag == (00)) {}

    // Receive the second character (ones place)
    rcv_intpt_flag = 00;
    HAL_UART_Receive_IT(&huart2, &hour[1], 1); // Receive second digit
    while(rcv_intpt_flag == (00)) {}

    // Optional: Print the received hour (for debug purposes)
    char debug_msg[64];
    sprintf(debug_msg, "Received hour: %c%c\r\n", hour[0], hour[1]);
    HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), 1000);

    // Convert the two characters into an integer (for example, "12" -> 12)
    *hour = (hour[0] - '0') * 10 + (hour[1] - '0');
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if((htim->Instance == TIM5)) {
		wall_clock_hr_update_flag = 0;
		clock_secs += 2;

		if((clock_secs == 60)) {
			clock_mins += 1;
			clock_secs = 0;

			if((clock_mins == 60)) {
				clock_hours += 1;
				clock_mins = 0;
			}
			wall_clock_hr_update_flag = 1;
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == RPM_TICK_Pin) {
		rpm_tick_count += 1;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
