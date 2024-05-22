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
#include "AOD.h"
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

unsigned long previousMillis = 0;
const long interval = 1000;
int delay = 100;

uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t dow;
uint8_t day;
uint8_t month;
uint16_t year;
char time_str[9];
char date_str[11];
char nameOfDay[4];

bool Menu_Stat = false;
uint8_t Menu_Cnt = 0;
bool Menu_Set = false;
uint8_t Menu_Set_cnt;
uint8_t Menu_Set_TimeDate = 0;

uint8_t Alarm_Stat = 0;
uint8_t hour_alarm, minute_alarm;
bool Set_hour_alarm = false;
bool Set_minute_alarm = false;
uint8_t Menu_Set_Alarm = 0;
bool Alarm_Start = false;
bool Alarm_Trig = true;
uint8_t Alarm_Duration = 0;
bool Blink_Alarm_Display = false;

bool set_Hour = false;
bool set_Min = false;
bool set_Day = false;
bool set_Month = false;
bool set_Year = false;

bool blink_cursor = false;

bool btn_Menu_Select, btn_Up, btn_Down;

#define Alarm_Stat_Address 0x00
#define Alarm_Hour_Address 0x02
#define Alarm_Minute_Address 0x01
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

  /* MCU Configuration -------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
//  EE_Init();
//  /* USER CODE END SysInit */
//  Alarm_Stat = EE_Read(Alarm_Stat_Address);
//  HAL_Delay(100);
//  hour_alarm = EE_Read(Alarm_Hour_Address);
//  HAL_Delay(100);
//  minute_alarm = EE_Read(Alarm_Minute_Address);
//  HAL_Delay(100);

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  DS1307_Init(&hi2c1);
  /*
    DS1307_SetHour(14);
    DS1307_SetMinute(51);
    DS1307_SetSecond(00);
    DS1307_SetDayOfWeek(1);		// 0-6 (0 is Sunday)
    DS1307_SetDate(21);
    DS1307_SetMonth(5);
    DS1307_SetYear(2024);
  */
  ssd1306_Init();
  AOD();
  HAL_Delay(2000);
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    unsigned long currentMillis = HAL_GetTick();
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      if (Menu_Stat == false)
      {
        Get_Date_Time();
        ssd1306_Fill(Black);

        if (Alarm_Stat == 1 && hour==hour_alarm && min == minute_alarm && Alarm_Trig == true)
        {
          Alarm_Start = true;
          Alarm_Trig = false;
        }

        if (min != minute_alarm)
        {
          Alarm_Trig = true;
        }

        if (Alarm_Start == true)
        {
          Alarm_Duration++;
          if (Alarm_Duration > 30)
          {
            Alarm_Start = false;
            Alarm_Duration = 0;
          }

          Blink_Alarm_Display = !Blink_Alarm_Display;
          ssd1306_Fill(Black);
          ssd1306_SetCursor(82, 22);
          if (Blink_Alarm_Display == true)
          {
            ssd1306_WriteString("Alarm", Font_6x8, White);
          }
          else
          {
            ssd1306_WriteString("Alarm", Font_6x8, Black);
          }

          ssd1306_SetCursor(82, 32);
          if (hour_alarm < 10)
          {
            ssd1306_WriteString("0", Font_6x8, White);
            ssd1306_WriteString(hour_alarm, Font_6x8, White);
          }
          else
          {
            ssd1306_WriteString(hour_alarm, Font_6x8, White);
          }
          ssd1306_WriteString(":", Font_6x8, White);
          if (minute_alarm < 10)
          {
            ssd1306_WriteString("0", Font_6x8, White);
            ssd1306_WriteString(minute_alarm, Font_6x8, White);
          }
          else
          {
            ssd1306_WriteString(minute_alarm, Font_6x8, White);
          }
        }
        else
        {
          ssd1306_Fill(Black);
          Digital_Time_Date();
        }
        ssd1306_UpdateScreen();
      }
    }

    read_Button();

    if (btn_Menu_Select == 0 && Alarm_Stat == true)
    {
      Alarm_Start = false;
      Alarm_Duration = 0;
      read_Button();
    }

    // condition for display menu
    if (btn_Menu_Select == 0 && Menu_Stat == false)
    {
      Menu_Cnt = 1;
      Menu_Stat = true;
    }

    // display menu
    while (Menu_Stat == true)
    {
      read_Button();

      if (btn_Down == 0)
      {
        Menu_Cnt++;
        if (Menu_Cnt > 3)
          Menu_Cnt = 1;
      }

      if (btn_Up == 0)
      {
        Menu_Cnt--;
        if (Menu_Cnt < 1)
          Menu_Cnt = 3;
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
        Menu_Set_Alarm = 1;
        // Alarm_Stat = EE_Read(Alarm_Stat_Address);
        // HAL_Delay(100);
        // hour_alarm = EE_Read(Alarm_Hour_Address);
        // HAL_Delay(100);
        // minute_alarm = EE_Read(Alarm_Minute_Address);
        // HAL_Delay(100);
      }

      if (btn_Menu_Select == 0 && Menu_Cnt == 3)
      {
        Menu_Cnt = 1;
        Menu_Stat = false;
      }

      // display menu
      if (Menu_Cnt == 1)
      {
        Display_Menu(Menu_Cnt);
      }
      else if (Menu_Cnt == 2)
      {
        Display_Menu(Menu_Cnt);
      }
      else if (Menu_Cnt == 3)
      {
        Display_Menu(Menu_Cnt);
      }

      // time and date setting time
      while (Menu_Set == true && Menu_Set_cnt == 1)
      {
        Set_Time_Date();
        HAL_Delay(50);
      }

      while (Menu_Set == true && Menu_Set_cnt == 2)
      {
        Set_Alarm();
        HAL_Delay(50);
      }

      HAL_Delay(50);
    }
  }
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void read_Button(void)
{
  btn_Menu_Select = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
  HAL_Delay(20);
  btn_Up = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
  HAL_Delay(20);
  btn_Down = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
  HAL_Delay(20);
}

void Get_Date_Time(void) {
	sec = DS1307_GetSecond();
	min = DS1307_GetMinute();
	hour = DS1307_GetHour();
	dow = DS1307_GetDayOfWeek();
	day = DS1307_GetDate();
	month = DS1307_GetMonth();
	year = DS1307_GetYear();
}

void Digital_Time_Date(void)
{
	ssd1306_Fill(Black);
	const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


	ssd1306_FillRectangle(0,0, 128, 18, Black);
	ssd1306_FillRectangle(0,18, 128, 31, White);
	ssd1306_FillRectangle(0,31, 128, 64, Black);

	// display time
	sprintf(time_str, "%02d:%02d", hour, min);
	ssd1306_SetCursor(25, 36);
	ssd1306_WriteString(time_str, Font_16x26, White);

	// display day of week

	 sprintf(nameOfDay, "%s", days[dow]);
	 uint8_t display_center_area = (127 - 65) / 2;
	 uint8_t center_text = (strlen(nameOfDay) * 6) / 2;
	 uint8_t x_text = 36 + (display_center_area - center_text);
	 ssd1306_SetCursor(x_text, 21);
	 ssd1306_WriteString(nameOfDay, Font_6x8, Black);


	// display date
	sprintf(date_str, "%02d,%s,%d", day, months[month - 1], year);
	ssd1306_SetCursor(30, 5);
	ssd1306_WriteString(date_str, Font_7x10, White);

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

void Display_Menu(uint16_t slct)
{
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
}
static int prev_Menu_Set_TimeDate = -1;
static bool prev_set_Hour = false;
static bool prev_set_Min = false;
static bool prev_set_Day = false;
static bool prev_set_Month = false;
static bool prev_set_Year = false;

void Set_Time_Date(void)
{
    read_Button();

    if (prev_Menu_Set_TimeDate != Menu_Set_TimeDate)
    {
        ssd1306_Fill(Black);
        Digital_Time_Date();
        prev_Menu_Set_TimeDate = Menu_Set_TimeDate;

        ssd1306_SetCursor(0, 20);
        ssd1306_WriteString("Back", Font_6x8, White);
        ssd1306_SetCursor(27, 20);
        ssd1306_WriteString("Save", Font_6x8, White);

        if (Menu_Set_TimeDate == 1)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Set", Font_6x8, White);
            ssd1306_SetCursor(0, 10);
            ssd1306_WriteString("hour", Font_6x8, White);
            ssd1306_DrawBitmap(66, 10, Select_Top, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 2)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Set", Font_6x8, White);
            ssd1306_SetCursor(0, 10);
            ssd1306_WriteString("minute", Font_6x8, White);
            ssd1306_DrawBitmap(84, 10, Select_Top, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 3)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Set", Font_6x8, White);
            ssd1306_SetCursor(0, 10);
            ssd1306_WriteString("day", Font_6x8, White);
            ssd1306_DrawBitmap(56, 15, Select_Buttom, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 4)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Set", Font_6x8, White);
            ssd1306_SetCursor(0, 10);
            ssd1306_WriteString("month", Font_6x8, White);
            ssd1306_DrawBitmap(77, 15, Select_Buttom, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 5)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Set", Font_6x8, White);
            ssd1306_SetCursor(0, 10);
            ssd1306_WriteString("year", Font_6x8, White);
            ssd1306_DrawBitmap(105, 15, Select_Buttom, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 6)
        {
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Save", Font_6x8, White);
            ssd1306_SetCursor(0, 8);
            ssd1306_WriteString("Setting", Font_6x8, White);
            ssd1306_DrawBitmap(36, 29, Select_Top, 5, 3, White);
        }
        else if (Menu_Set_TimeDate == 7)
        {
            ssd1306_DrawBitmap(9, 29, Select_Top, 5, 3, White);
        }
        ssd1306_UpdateScreen();
    }

    if (btn_Down == 0)
    {
        Menu_Set_TimeDate++;
        if (Menu_Set_TimeDate > 7)
        {
            Menu_Set_TimeDate = 1;
        }
    }

    if (btn_Up == 0)
    {
        Menu_Set_TimeDate--;
        if (Menu_Set_TimeDate < 1)
        {
            Menu_Set_TimeDate = 7;
        }
    }

    if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 1)
    {
      set_Hour = true;
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 2)
    {
      set_Min = true;
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 3)
    {
      set_Day = true;
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 4)
    {
      set_Month = true;
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 5)
    {
      set_Year = true;
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 6)
    {
      DS1307_SetHour(hour);
      DS1307_SetMinute(min);
      DS1307_SetSecond(0);
      DS1307_SetDate(day);
      DS1307_SetMonth(month);
      DS1307_SetYear(year);
      Display_to_Save();
    }
    else if (btn_Menu_Select == 0 && Menu_Set_TimeDate == 7)
    {
      Menu_Set_cnt = 0;
      Menu_Set = false;
    }

    if (set_Hour != prev_set_Hour)
    {
        prev_set_Hour = set_Hour;
        while (set_Hour)
        {
          read_Button();
          ssd1306_Fill(Black);
          ssd1306_SetCursor(0, 0);
          ssd1306_WriteString("Set", Font_6x8, White);
          ssd1306_SetCursor(0, 10);
          ssd1306_WriteString("hour", Font_6x8, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("Back", Font_6x8, White);
          ssd1306_SetCursor(27, 20);
          ssd1306_WriteString("Save", Font_6x8, White);

          Digital_Time_Date();

          if (btn_Down == 0)
          {
            hour++;
            if (hour > 23)
            {
              hour = 0;
            }
          }

          if (btn_Up == 0)
          {
            hour--;
            if (hour > 254)
            {
              hour = 23;
            }
          }

          if (btn_Menu_Select == 0)
          {
            set_Hour = false;
          }

          blink_cursor = !blink_cursor;
          if (blink_cursor==true)
          {
            ssd1306_DrawBitmap(66, 10, Select_Top, 5, 3, White);
          }
          else
          {
            ssd1306_DrawBitmap(66, 10, Select_Top, 5, 3, Black);
          }
          ssd1306_UpdateScreen();
          HAL_Delay(50);
        }
    }

    if (set_Min != prev_set_Min)
    {
        prev_set_Min = set_Min;
        while (set_Min)
        {
          read_Button();
          ssd1306_Fill(Black);
          ssd1306_SetCursor(0, 0);
          ssd1306_WriteString("Set", Font_6x8, White);
          ssd1306_SetCursor(0, 10);
          ssd1306_WriteString("minute", Font_6x8, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("Back", Font_6x8, White);
          ssd1306_SetCursor(27, 20);
          ssd1306_WriteString("Save", Font_6x8, White);

          Digital_Time_Date();

          if (btn_Down == 0)
          {
            min++;
            if (min > 59)
            {
              min = 0;
            }
          }

          if (btn_Up == 0)
          {
            min--;
            if (min > 254)
            {
              min = 59;
            }
          }

          if (btn_Menu_Select == 0)
          {
            set_Min = false;
          }

          blink_cursor = !blink_cursor;
          if (blink_cursor==true)
          {
            ssd1306_DrawBitmap(84, 10, Select_Top, 5, 3, White);
          }
          else
          {
            ssd1306_DrawBitmap(84, 10, Select_Top, 5, 3, Black);
          }
          ssd1306_UpdateScreen();
          HAL_Delay(50);
        }
    }

    if (set_Day != prev_set_Day)
    {
        prev_set_Day = set_Day;
        while (set_Day)
        {
          read_Button();
          ssd1306_Fill(Black);
          ssd1306_SetCursor(0, 0);
          ssd1306_WriteString("Set", Font_6x8, White);
          ssd1306_SetCursor(0, 10);
          ssd1306_WriteString("day", Font_6x8, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("Back", Font_6x8, White);
          ssd1306_SetCursor(27, 20);
          ssd1306_WriteString("Save", Font_6x8, White);

          Digital_Time_Date();

          if (btn_Down == 0)
          {
            day++;
            if (day > 31)
            {
              day = 1;
            }
          }

          if (btn_Up == 0)
          {
            day--;
            if (day < 1)
            {
              day = 31;
            }
          }

          if (btn_Menu_Select == 0)
          {
            set_Day = false;
          }

          blink_cursor = !blink_cursor;
          if (blink_cursor==true)
          {
            ssd1306_DrawBitmap(56, 15, Select_Buttom, 5, 3, White);
          }
          else
          {
            ssd1306_DrawBitmap(56, 15, Select_Buttom, 5, 3, Black);
          }
          ssd1306_UpdateScreen();
          HAL_Delay(50);
        }
    }

    if (set_Month != prev_set_Month)
    {
        prev_set_Month = set_Month;
        while (set_Month)
        {
          read_Button();
          ssd1306_Fill(Black);
          ssd1306_SetCursor(0, 0);
          ssd1306_WriteString("Set", Font_6x8, White);
          ssd1306_SetCursor(0, 10);
          ssd1306_WriteString("month", Font_6x8, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("Back", Font_6x8, White);
          ssd1306_SetCursor(27, 20);
          ssd1306_WriteString("Save", Font_6x8, White);

          Digital_Time_Date();

          if (btn_Down == 0)
          {
            month++;
            if (month > 12)
            {
              month = 1;
            }
          }

          if (btn_Up == 0)
          {
            month--;
            if (month < 1)
            {
              month = 12;
            }
          }

          if (btn_Menu_Select == 0)
          {
            set_Month = false;
          }

          blink_cursor = !blink_cursor;
          if (blink_cursor == true)
          {
            ssd1306_DrawBitmap(77, 15, Select_Buttom, 5, 3, White);
          }
          else
          {
            ssd1306_DrawBitmap(77, 15, Select_Buttom, 5, 3, Black);
          }
          ssd1306_UpdateScreen();
          HAL_Delay(50);
        }
    }

    if (set_Year != prev_set_Year)
    {
        prev_set_Year = set_Year;
        while (set_Year)
        {
          read_Button();
          ssd1306_Fill(Black);
          ssd1306_SetCursor(0, 0);
          ssd1306_WriteString("Set", Font_6x8, White);
          ssd1306_SetCursor(0, 10);
          ssd1306_WriteString("year", Font_6x8, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("Back", Font_6x8, White);
          ssd1306_SetCursor(27, 20);
          ssd1306_WriteString("Save", Font_6x8, White);

          Digital_Time_Date();

          if (btn_Down == 0)
          {
            year++;
            if (year > 2100)
            {
              year = 2000;
            }
          }

          if (btn_Up == 0)
          {
            year--;
            if (year < 2000)
            {
              year = 2100;
            }
          }

          if (btn_Menu_Select == 0)
          {
            set_Year = false;
          }

          blink_cursor = !blink_cursor;
          if (blink_cursor==true)
          {
            ssd1306_DrawBitmap(105, 15, Select_Buttom, 5, 3, White);
          }
          else
          {
            ssd1306_DrawBitmap(105, 15, Select_Buttom, 5, 3, Black);
          }
          ssd1306_UpdateScreen();
          HAL_Delay(50);
        }
    }
}

void Display_to_Save(void)
{
  ssd1306_Fill(Black);
  ssd1306_SetCursor(33, 20);
  ssd1306_WriteString("Save", Font_16x24, White);
  ssd1306_UpdateScreen();
}

void Set_Alarm(void)
{
  read_Button();
  ssd1306_Fill(Black);
  Set_Alarm_Display();

  // selecting alarm setting
  if (btn_Down == 0)
  {
    Menu_Set_Alarm++;
    if (Menu_Set_Alarm > 5)
    {
      Menu_Set_Alarm = 1;
    }
  }

  if (btn_Up == 0)
  {
    Menu_Set_Alarm--;
    if (Menu_Set_Alarm < 1)
    {
      Menu_Set_Alarm = 5;
    }
  }

  if (btn_Menu_Select == 0 && Menu_Set_Alarm == 1)
  {
    Set_hour_alarm = true;
  }

  if (btn_Menu_Select == 0 && Menu_Set_Alarm == 2)
  {
    Set_minute_alarm = true;
  }

  // enable alarm and save alarm setting
  if (btn_Menu_Select == 0 && Menu_Set_Alarm == 3)
  {
    Display_to_Save();
    Alarm_Stat = 1;
//    if (EE_Read(Alarm_Stat_Address) != Alarm_Stat)
//    {
//      EE_Write(Alarm_Stat_Address, Alarm_Stat);
//    }
//    HAL_Delay(100);
//    if (EE_Read(Alarm_Hour_Address) != hour_alarm)
//    {
//      EE_Write(Alarm_Hour_Address, hour_alarm);
//    }
//    HAL_Delay(100);
//    if (EE_Read(Alarm_Minute_Address) != minute_alarm)
//    {
//      EE_Write(Alarm_Minute_Address, minute_alarm);
//    }
//    HAL_Delay(100);
//
//    Alarm_Stat = EE_Read(Alarm_Stat_Address);
//    HAL_Delay(100);
//    hour_alarm = EE_Read(Alarm_Hour_Address);
//    HAL_Delay(100);
//    minute_alarm = EE_Read(Alarm_Minute_Address);
//    HAL_Delay(100);
  }

  // disable alarm
//  if (btn_Menu_Select == 0 && Menu_Set_Alarm == 4)
//  {
//    Display_to_Save();
//    Alarm_Stat = 0;
//    hour_alarm = 0;
//    minute_alarm = 0;
//    if (EE_Read(Alarm_Stat_Address) != Alarm_Stat)
//    {
//      EE_Write(Alarm_Stat_Address, Alarm_Stat);
//    }
//    HAL_Delay(100);
//    if (EE_Read(Alarm_Hour_Address) != hour_alarm)
//    {
//      EE_Write(Alarm_Hour_Address, hour_alarm);
//    }
//    HAL_Delay(100);
//    if (EE_Read(Alarm_Minute_Address) != minute_alarm)
//    {
//      EE_Write(Alarm_Minute_Address, minute_alarm);
//    }
//    HAL_Delay(100);
//
//    Alarm_Stat = EE_Read(Alarm_Stat_Address);
//    HAL_Delay(100);
//    hour_alarm = EE_Read(Alarm_Hour_Address);
//    HAL_Delay(100);
//    minute_alarm = EE_Read(Alarm_Minute_Address);
//    HAL_Delay(100);
//  }

  if (btn_Menu_Select == 0 && Menu_Set_Alarm == 5)
  {
    Menu_Set_Alarm = 0;
    Menu_Set = false;
  }

  // display cursor
  if (Menu_Set_Alarm == 1)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 2)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 3)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 4)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 5)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }

  // setting hour
  while (Set_hour_alarm == true && Menu_Set_Alarm == 1)
  {
    read_Button();
    ssd1306_Fill(Black);

    if (btn_Down == 0)
    {
      delay = 50;
      hour_alarm++;
      if (hour_alarm > 23)
      {
        hour_alarm = 0;
      }
    }

    if (btn_Up == 0)
    {
      delay = 50;
      hour_alarm--;
      if (hour_alarm < 0)
      {
        hour_alarm = 23;
      }
    }

    if (btn_Menu_Select == 0)
    {
      Set_hour_alarm = false;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor)
    {
      ssd1306_DrawBitmap(48, 18, Select_Top, 5, 3, White);
    }
    else
    {
      ssd1306_DrawBitmap(48, 18, Select_Top, 5, 3, Black);
    }
    ssd1306_UpdateScreen();
  }
  ssd1306_UpdateScreen();
  HAL_Delay(500);
}

void Set_Alarm_Display(void)
{
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

  if (hour_alarm < 10)
  {
    ssd1306_SetCursor(45, 9);
    ssd1306_WriteString("0", Font_6x8, White);
    ssd1306_WriteString(hour_alarm, Font_6x8, White);
  }
  else
  {
    ssd1306_SetCursor(45, 9);
    ssd1306_WriteString(hour_alarm, Font_6x8, White);
  }
  ssd1306_WriteString(":", Font_6x8, White);
  if (minute_alarm < 10)
  {
    ssd1306_WriteString("0", Font_6x8, White);
    ssd1306_WriteString(minute_alarm, Font_6x8, White);
  }
  else
  {
    ssd1306_WriteString(minute_alarm, Font_6x8, White);
  }

  ssd1306_SetCursor(5, 24);
  ssd1306_WriteString("Enable", Font_6x8, White);

  ssd1306_SetCursor(50, 24);
  ssd1306_WriteString("Disable", Font_6x8, White);

  ssd1306_SetCursor(100, 24);
  ssd1306_WriteString("Back", Font_6x8, White);
}

void Set_Alarm_Cursor(uint8_t slct)
{
  if (slct == 1)
  {
    ssd1306_DrawBitmap(48, 18, Select_Top, 5, 4, White);
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
