/*****************************************************************
 * Description 
 * Email huxiaoheigame@gmail.com
 * Created on 2021/08/01
 * Copyright (c) 2021 虎小黑
 ****************************************************************/

#include "util.h"

namespace tigerkin {

pid_t getThreadId() {
    return syscall(SYS_gettid);
}

uint32_t getFiberId() {
    return 0;
}

}