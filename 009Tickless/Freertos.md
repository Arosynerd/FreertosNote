# Tickless

开启低功耗模式。

设置preprocess函数，进入低功耗模式前执行的操作：关闭USART1时钟

设置postprocess函数，退出低功耗模式后执行的操作：开启USART1时钟

## 系统裁剪----开启低功耗功能和设置两个函数

```
#define configUSE_TICKLESS_IDLE                         1
#include "main.h"
#define configPRE_SLEEP_PROCESSING( x )         PRE_SLEEP_PROCESSING()
#define configPOST_SLEEP_PROCESSING( x )        POST_SLEEP_PROCESSING()
```



## 两个函数的实现

```
void PRE_SLEEP_PROCESSING(void){
  __HAL_RCC_USART1_CLK_DISABLE();
}
void POST_SLEEP_PROCESSING(void){
  __HAL_RCC_USART1_CLK_ENABLE();
}
```











