/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "time.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
char year = 24;
char month = 10;
char day = 3;
int hour = 22;
int min = 11;
int sec = 15;
char time_str[9];
char date_str[11];

bool Menu_Stat = false;
int Menu_Cnt = 0;
bool Menu_Set = false;
int Menu_Set_cnt;
int Menu_Set_TimeDate = 0;
bool Alarm_Stat = 0;
uint16_t btn_Menu_Select, btn_Up, btn_Down;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    Digital_Time_Date();
    
    read_Button();

    if (btn_Menu_Select == 0 && Menu_Stat == false)
    {
      Menu_Cnt = 1;
      Menu_Stat = true;
    }
    while (Menu_Stat == true) {
      read_Button();
      if(btn_Down == 0) {
        Menu_Cnt++;
        if(Menu_Cnt > 3) {
          Menu_Cnt = 1;
        }
      }
      
      if(btn_Up == 0) {
        Menu_Cnt--;
        if(Menu_Cnt < 1) {
          Menu_Cnt = 3;
        }
      }

      if (btn_Menu_Select == 0 && Menu_Cnt == 1)
      {
        Menu_Set = true;
        Menu_Set_cnt = 1;
        Menu_Set_TimeDate = 1;
      }

      if (btn_Menu_Select == 0 && Menu_Cnt == 2)
      {
        Menu_Set = true;
        Menu_Set_cnt = 2;
      }

      if (btn_Menu_Select == 0 && Menu_Cnt == 3)
      {
        Menu_Cnt = 1;
        Menu_Stat = false;
      }

      if(Menu_Cnt == 1) {
        Display_Menu(Menu_Cnt);
      } else if(Menu_Cnt == 2) {
        Display_Menu(Menu_Cnt);
      } else if(Menu_Cnt == 3) {
        Display_Menu(Menu_Cnt);
      }

      while(Menu_Set == true && Menu_Set_cnt == 1) {
        Set_Time_Date();
      }

      while(Menu_Set == true && Menu_Set_cnt == 2) {
        Set_Alarm();
      }
    } 
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void read_Button() {
  btn_Menu_Select = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
  btn_Up = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  btn_Down = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
}

void Digital_Time_Date() {
    ssd1306_Fill(Black);
    time_t currentTime;
    struct tm *localTime;

    // Get the current time
    time(&currentTime);
    localTime = localtime(&currentTime);

    int hour = localTime->tm_hour;
    int min = localTime->tm_min;
    int sec = localTime->tm_sec;
    int dayOfWeek = localTime->tm_wday; // This gives the day of the week (0-6, Sunday = 0)
    int day = localTime->tm_mday; // Day of the month (1-31)
    int month = localTime->tm_mon + 1; // Month (0-11, so add 1 to get 1-12)
    int year = localTime->tm_year + 1900; // Year since 1900

    char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    // display time
    sprintf(time_str, "%02d:%02d:%02d", hour, min, sec);
    ssd1306_SetCursor(63, 0);
    ssd1306_WriteString(time_str, Font_6x8, White);

    // display day of week
    if(Menu_Stat == false) {
      char *nameOfDay = days[dayOfWeek];
      int display_center_area = (127 - 65) / 2;
      int center_text = (strlen(nameOfDay) * 6) / 2;
      int x_text = 56 + (display_center_area - center_text);

      ssd1306_SetCursor(x_text, 10);
      ssd1306_WriteString(nameOfDay, Font_6x8, White);
    }

    // display date
    sprintf(date_str, "%02d,%s,%d", day, months[month - 1], year);
    ssd1306_SetCursor(53, 20);
    ssd1306_WriteString(date_str, Font_6x8, White);

    ssd1306_UpdateScreen();
}

const unsigned char Select_Top[] = {
    0x20, 0x70, 0xf8};

// 'Select_Side', 3x5px
const unsigned char Select_Side[] = {
    0x80, 0xc0, 0xe0, 0xc0, 0x80};

// 'Select_Buttom', 5x3px
const unsigned char Select_Buttom[] = {
    0xf8, 0x70, 0x20};

void Display_Menu(uint16_t slct) {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(40, 0);
    ssd1306_WriteString("Menu", Font_6x8, White);

    ssd1306_SetCursor(7, 7);
    ssd1306_WriteString("Set Time and Date", Font_6x8, White);

    ssd1306_SetCursor(7, 15);
    ssd1306_WriteString("Set Alarm", Font_6x8, White);

    ssd1306_SetCursor(7, 23);
    ssd1306_WriteString("Back", Font_6x8, White);
      
    if (slct == 1)
    {
      ssd1306_DrawBitmap(0, 8, Select_Side, 3, 5, White);
    }
    else if (slct == 2)
    {
      ssd1306_DrawBitmap(0, 16, Select_Side, 3, 5, White);
    }
    else if (slct == 3)
    {
      ssd1306_DrawBitmap(0, 24, Select_Side, 3, 5, White);
    }
    ssd1306_UpdateScreen();
    HAL_Delay(1000);
}


void Set_Time_Date() { 

    ssd1306_Fill(Black);
    Menu_Stat = true;
    Digital_Time_Date();

    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString("Save", Font_6x8, White);

    ssd1306_SetCursor(27, 20);
    ssd1306_WriteString("Back", Font_6x8, White);

    slct++;
    if (slct == 1) //hour
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Set", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("hour", Font_6x8, White);

      ssd1306_DrawBitmap(66, 10, Select_Top, 5, 3, White);
    }
    else if (slct == 2) // minute
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Set", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("minute", Font_6x8, White);

      ssd1306_DrawBitmap(84, 10, Select_Top, 5, 3, White);
    }
    else if (slct == 3) // day
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Set", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("day", Font_6x8, White);

      ssd1306_DrawBitmap(56, 15, Select_Buttom, 5, 3, White);
    }
    else if (slct == 4) // month
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Set", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("month", Font_6x8, White);
      
      ssd1306_DrawBitmap(77, 15, Select_Buttom, 5, 3, White);
    }
    else if (slct == 5) //year
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Set", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("year", Font_6x8, White);

      ssd1306_DrawBitmap(105, 15, Select_Buttom, 5, 3, White);
    }
    else if (slct == 6) // save
    {
      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Save", Font_6x8, White);

      ssd1306_SetCursor(0, 10);
      ssd1306_WriteString("Setting", Font_6x8, White);

      ssd1306_DrawBitmap(9, 29, Select_Top, 5, 3, White);
    }
    else if (slct == 7) // back
    {
      ssd1306_DrawBitmap(36, 29, Select_Top, 5, 3, White);
    }
    ssd1306_UpdateScreen();
    HAL_Delay(500);

}

void Set_Alarm() {
  read_Button();
  ssd1306_Fill(Black);
  if (Alarm_Stat == 0)
  {
    ssd1306_SetCursor(15, 0);
    ssd1306_WriteString("Status : Disable", Font_6x8, White);
  }
  else
  {
    ssd1306_SetCursor(18, 0);
    ssd1306_WriteString("Status : Enable", Font_6x8, White);
  }

  ssd1306_SetCursor(45, 9);
  ssd1306_WriteString("00:00", Font_6x8, White);

  ssd1306_SetCursor(5, 24);
  ssd1306_WriteString("Enable", Font_6x8, White);

  ssd1306_SetCursor(50, 24);
  ssd1306_WriteString("Disable", Font_6x8, White);

  ssd1306_SetCursor(100, 24);
  ssd1306_WriteString("Back", Font_6x8, White);

  slct++;
  if (slct == 1)
  {
    ssd1306_DrawBitmap(48, 18, Select_Top, 5, 3, White);
  }
  else if (slct == 2)
  {
    ssd1306_DrawBitmap(66, 18, Select_Top, 5, 3, White);
  }
  else if (slct == 3)
  {
    ssd1306_DrawBitmap(0, 25, Select_Side, 3, 5, White);
  }
  else if (slct == 4)
  {
    ssd1306_DrawBitmap(45, 25, Select_Side, 3, 5, White);
  }
  else if (slct == 5)
  {
    ssd1306_DrawBitmap(95, 25, Select_Side, 3, 5, White);
  }
  ssd1306_UpdateScreen();
  HAL_Delay(500);
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

#ifdef  USE_FULL_ASSERT
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
