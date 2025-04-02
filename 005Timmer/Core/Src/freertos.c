/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "showTaskInfo.h"
#include "usart.h"
#include "timers.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t showTaskInfo_handle;
TaskHandle_t task3_handle;//receive task queue

TimerHandle_t xTimer1;
TimerHandle_t xTimer2;
void timer1_callback(TimerHandle_t xTimer);
void timer2_callback(TimerHandle_t xTimer);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  taskENTER_CRITICAL();
  xTimer1 = xTimerCreate( "Timer1", pdMS_TO_TICKS(1000), pdFALSE, (void *)1, timer1_callback );
  xTimer2 = xTimerCreate( "Timer2", pdMS_TO_TICKS(2000), pdTRUE, (void *)2, timer2_callback );
  // xTaskCreate(task1, "task1", 128, NULL, 1, &task1_handle);
  // xTaskCreate(task2, "task2", 128, NULL, 2, &task2_handle);
  // xTaskCreate(task3, "task3", 128, NULL, 3, &task3_handle);
  //xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  // 创建任务3
  // BaseType_t xReturned = xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  // if( xReturned != pdPASS ) {
  //   // 任务创建失败，打印错误信息
  //   printf("Task 3 creation failed! Error code: %d\r\n", xReturned);
  // }
  // vTaskSuspend(task1_handle);
  // vTaskSuspend(task2_handle);
  vTaskDelete(NULL);
  taskEXIT_CRITICAL();
 
  //删除本任务
  
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// External interrupt callback function
uint64_t pretime = 0;
uint8_t flag = Suspended; // Default suspended
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  uint64_t nowtime;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE; // Declare this variable once for ISR-safe API calls

  if(GPIO_Pin == GPIO_PIN_0) {
    // Interrupt handling
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100) return;
    printf("开启定时器\r\n");
    // Start timers
    xTimerStartFromISR(xTimer1, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    xTimerStartFromISR(xTimer2, &xHigherPriorityTaskWoken); // Corrected argument
  }
  else if(GPIO_Pin == GPIO_PIN_2) {
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100) return;
    printf("关闭定时器\r\n");
    // Stop timers
    xTimerStopFromISR(xTimer1, &xHigherPriorityTaskWoken); // Corrected argument
    xTimerStopFromISR(xTimer2, &xHigherPriorityTaskWoken); // Corrected argument
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // Ensure context switch if needed
  }
  pretime = nowtime;
}
// Timer callback functions
void timer1_callback(TimerHandle_t xTimer) {
  static uint16_t count = 0;
  printf("timer1 value = %d\r\n", ++count);
}
void timer2_callback(TimerHandle_t xTimer) {
  static uint16_t count = 0;
  printf("timer2 value = %d\r\n", ++count);
}
/* USER CODE END Application */

