# Freertos

## 分配内存执行操作

```
void timer60s_callback(TimerHandle_t xTimer){
  if(++sixtys_count == 10){
    sixtys_count = 0;
    // 60秒到了，执行相应的操作
    // 例如打印消息或执行其他逻辑
    printf("60 seconds elapsed!\r\n");
    //筛选正在运行或处于就绪队列的所有任务
  TaskStatus_t *pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  uint32_t ulTotalRunTime;

  // 获取任务数量
  uxArraySize = uxTaskGetNumberOfTasks();

  // 为任务状态数组分配内存
  pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

  if (pxTaskStatusArray != NULL) {
    // 获取所有任务的状态信息
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

    // 定义一个数组存放筛选出来的任务句柄
    TaskHandle_t filteredTaskHandles[uxArraySize];
    uint32_t filteredTaskCount = 0;

    // 遍历任务状态数组，筛选出状态为阻塞或就绪的任务
    for (x = 0; x < uxArraySize; x++) {
      if (pxTaskStatusArray[x].eCurrentState == eBlocked || pxTaskStatusArray[x].eCurrentState == eReady) {
      printf("Task Name: %s, State: %s\r\n", pxTaskStatusArray[x].pcTaskName,
         (pxTaskStatusArray[x].eCurrentState == eBlocked) ? "Blocked" : "Ready");
      // 将任务句柄存入数组
      filteredTaskHandles[filteredTaskCount++] = pxTaskStatusArray[x].xHandle;
      }
    }
    // 遍历筛选出来的任务句柄数
    for (uint32_t i = 0; i < filteredTaskCount; i++) {
      for(uint8_t j = 0; j < sizeof(taskHandles)/sizeof(taskHandles[0]); j++){
        if (filteredTaskHandles[i] == *taskHandles[j]) {
          printf("Matching Task Name: %s\r\n", pxTaskStatusArray[j].pcTaskName);
          
          break;
        }
      }
    }

    // 释放任务状态数组的内存
    vPortFree(pxTaskStatusArray);
  } else {
    printf("Failed to allocate memory for task status array.\r\n");
  }

  }
  printf("count = %d\r\n",sixtys_count); 
}
```

