#include "task2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

void task2(void *pvParameters) {
  for (;;) {
    printf("task2运行....\r\n");
    HAL_Delay(2000);
    vTaskDelay(1000);
  }
}
