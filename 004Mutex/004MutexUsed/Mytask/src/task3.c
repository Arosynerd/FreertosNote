#include "task3.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
extern QueueHandle_t semphore_mutex_handle;
void task3(void *pvParameters)
{
  BaseType_t err;
  while (1)
  {
      err = xSemaphoreTake(semphore_mutex_handle, portMAX_DELAY);
      if(err == pdTRUE){
          printf("task3获得信号量\r\n");
      }
      else{
          printf("task3获取信号量超时\r\n");
      }
      printf("task3运行....\r\n");
      HAL_Delay(1000);
      err = xSemaphoreGive(semphore_mutex_handle);
      if(err != pdTRUE){
          printf("task3释放信号量失败\r\n"); 
      }
      vTaskDelay(1000);
      
  }
}
