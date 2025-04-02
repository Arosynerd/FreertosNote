# Freertos

任务1：启动单词软件定时器和周期定时器，它们每次溢出执行输出它们的溢出次数

外部按键：两个，开启或停止两个定时器

## 配置定时器任务

```
/* Software timer definitions. */
#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                ( 8 )
#define configTIMER_QUEUE_LENGTH                 10
#define configTIMER_TASK_STACK_DEPTH             256
```

任务调度器根据这些宏设置定时器任务

## 创建定时器任务

### 创建句柄

```
TimerHandle_t xTimer1;
TimerHandle_t xTimer2;
```

### 创建定时器

```
 xTimer1 = xTimerCreate( "Timer1", pdMS_TO_TICKS(1000), pdFALSE, (void *)1, timer1_callback );
 xTimer2 = xTimerCreate( "Timer2", pdMS_TO_TICKS(2000), pdTRUE, (void *)2, timer2_callback );
 
```

参数：

1

2

3

4 单次还是多次



### 定时器回调函数

```
void timer1_callback(TimerHandle_t xTimer) {
  static uint16_t count = 0;
  printf("timer1 value = %d\r\n", ++count);
}
void timer2_callback(TimerHandle_t xTimer) {
  static uint16_t count = 0;
  printf("timer2 value = %d\r\n", ++count);
}
```

计数

## 外部中断按键

```
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
```

启动或停止定时器
