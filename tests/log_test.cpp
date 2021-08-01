/*****************************************************************
 * Description 
 * Email huxiaoheigame@gmail.com
 * Created on 2021/07/24
 * Copyright (c) 2021 虎小黑
 ****************************************************************/

#include "../src/log.h"

#include <iostream>
#include <thread>

#include "../src/util.h"

int main(int argc, char **argv) {
    std::cout << "test tigerkin log start" << std::endl;
    
    tigerkin::Logger::ptr logger(new tigerkin::Logger("logger test"));
    
    tigerkin::LogAppender::ptr stdAppender(new tigerkin::StdOutLogAppend());
    tigerkin::LogFormatter::ptr formater(new tigerkin::LogFormatter("%d{%Y-%m-%d %H:%M:%S} %t %N %F [%p] [%c] %f:%l %m%n"));
    stdAppender->setFormate(formater);
    logger->addAppender(stdAppender);

    tigerkin::LogAppender::ptr fileAppender(new tigerkin::FileLogAppend("./log.txt"));
    fileAppender->setFormate(formater);
    logger->addAppender(fileAppender);
    
    TIGERKIN_LOG_DEBUG(logger) << "Hello World";
    TIGERKIN_LOG_INFO(logger) << "Hello World";
    TIGERKIN_LOG_WARN(logger ) << "Hello World";
    TIGERKIN_LOG_ERROR(logger) << "Hello World";
    TIGERKIN_LOG_FATAL(logger) << "Hello World";

    TIGERKIN_LOG_FMT_DEBUG(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_INFO(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_WARN(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_ERROR(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_FATAL(logger, "fmt Hello %s", "World");
    
    std::cout << "test tigerkin log end" << std::endl;

    return 0;
}


