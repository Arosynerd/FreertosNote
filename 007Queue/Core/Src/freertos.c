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
#include "queue.h"
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
osThreadId defaultTaskHandle;
osMessageQId myQueue01Handle;
osMessageQId myBigQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t showTaskInfo_handle;
TaskHandle_t task3_handle;//receive task queue
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 2, uint8_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* definition and creation of myBigQueue */
  osMessageQDef(myBigQueue, 2, char *);
  myBigQueueHandle = osMessageCreate(osMessageQ(myBigQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  if(myQueue01Handle == NULL) {
    printf("myQueue01 create error\r\n"); 
  }
  else{
    printf("myQueue01 create ok\r\n");
  }
  if(myBigQueueHandle == NULL) {
    printf("myBigQueue create error\r\n");
  }
  else{
    printf("myBigQueue create ok\r\n");
  }
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  taskENTER_CRITICAL();
  xTaskCreate(task1, "task1", 128, NULL, 1, &task1_handle);
  xTaskCreate(task2, "task2", 256, NULL, 2, &task2_handle);
  // xTaskCreate(task3, "task3", 128, NULL, 3, &task3_handle);
  // //xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  // // 创建任务3
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
//外部中断回调函数
uint64_t pretime = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  static uint8_t count = 0;
  uint64_t nowtime;
  BaseType_t err;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE; // Correct variable for ISR context

  if(GPIO_Pin == GPIO_PIN_0) {
    //中断处理
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100) return;
    err = xQueueSendToBackFromISR(myQueue01Handle, &count, &xHigherPriorityTaskWoken);
    if(err != pdPASS) printf("send error\r\n");
    else printf("queue1 send ok\r\n");
    ++count;
  }
  else if(GPIO_Pin == GPIO_PIN_2) {
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100) return;
    char *message = "hello world";
    err = xQueueSendToBackFromISR(myBigQueueHandle, &message, &xHigherPriorityTaskWoken);
    if(err != pdPASS) printf("send error\r\n");
    else printf("bigqueue send ok\r\n");
  }
  pretime = nowtime;

  // Perform a context switch if required
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/* USER CODE END Application */

