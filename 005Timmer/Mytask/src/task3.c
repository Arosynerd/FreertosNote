#include "task3.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
extern uint8_t flag;
extern TaskHandle_t task1_handle;
extern TaskHandle_t task2_handle;
static uint8_t pre_status = Suspended;
void task3(void *pvParameters)
{
  for (;;)
  {
    printf("task3运行....\r\n");
    if (flag != pre_status)
    {
      if (flag == Suspended)
      {
        vTaskSuspend(task1_handle);
        vTaskSuspend(task2_handle);
        printf("task1和task2已挂起\r\n");
      }
      else
      {
        vTaskResume(task1_handle);
        vTaskResume(task2_handle);
        printf("task1和task2已恢复\r\n");
      }
      pre_status = flag;
    }

    vTaskDelay(1000);
  }
}
