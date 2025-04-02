# Freertos

启动任务创建四个任务，挂起两个。

未挂起任务3执行挂起恢复任务1、2。通过接收外部中断按键改变的标志位决定是否执行

任务4执行显示个任务状态。

## 创建任务句柄

```
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t showTaskInfo_handle;
TaskHandle_t task3_handle;//receive task queue
/* USER CODE END FunctionPrototypes */
```



## 启动任务创建

```
 osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
 defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
```

这是st hal对freertos的抽象函数

然后定义其任务实现，在下面

## 其他任务在启动任务的创建

```
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  taskENTER_CRITICAL();
  xTaskCreate(task1, "task1", 128, NULL, 1, &task1_handle);
  xTaskCreate(task2, "task2", 128, NULL, 2, &task2_handle);
  xTaskCreate(task3, "task3", 128, NULL, 3, &task3_handle);
  //xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  // 创建任务3
  BaseType_t xReturned = xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  if( xReturned != pdPASS ) {
    // 任务创建失败，打印错误信息
    printf("Task 3 creation failed! Error code: %d\r\n", xReturned);
  }
  vTaskSuspend(task1_handle);
  vTaskSuspend(task2_handle);
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
```

各任务方法定义在各个单个文件，引入对应的.h包即可，使得结构清晰

遇到不能创建第四个任务，随后通过修改配置文件的内存分配解决

### 任务逻辑

创建4个任务，挂起任务1、2，删除本任务。

## 外部中断（按键）

```
uint64_t pretime = 0;
uint8_t flag = Suspended;//挂起默认
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  uint64_t nowtime;
  if(GPIO_Pin == GPIO_PIN_0) {
    //中断处理
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100)return;
    printf("pressed\r\n");
    
    flag = flag == Suspended ? Running : Suspended;
  }
  pretime = nowtime;
}
```

修改任务1、2的挂起或恢复的状态标志位

在任务3处理任务状态变更

## task3

```
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
```

## task4

```
char task_info[500];
void showTaskInfo(void *pvParameters) {
    while(1){
        vTaskList(task_info);
        printf("%s", task_info);
        printf("========================================\r\n");
        vTaskDelay(2000);
    }

}
```

每个2秒显示各任务情况。
