#include "task3.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
extern TaskHandle_t task1_handle;
extern TaskHandle_t task2_handle;
void task3(void *pvParameters)
{
  for (;;)
  {

    vTaskDelay(1000);
  }
}
