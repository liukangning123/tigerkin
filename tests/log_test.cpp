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

void defaultLogTest() {
    TIGERKIN_LOG_INFO(TIGERKIN_LOG_ROOT()) << "default log";
    TIGERKIN_LOG_FMT_INFO(TIGERKIN_LOG_ROOT(), "fmt default %s", "log");
}

void simpleLogTest() {

    tigerkin::Logger::ptr logger(new tigerkin::Logger("SimpleLogTest"));
    tigerkin::LogAppender::ptr stdAppender(new tigerkin::StdOutLogAppend());
    tigerkin::LogFormatter::ptr formater(new tigerkin::LogFormatter("%d{%Y-%m-%d %H:%M:%S} %t %N %F [%p] [%c] %f:%l %m%n"));
    stdAppender->setFormate(formater);
    logger->addAppender(stdAppender);

    tigerkin::LogAppender::ptr fileAppender(new tigerkin::FileLogAppend("./simpleLog.txt"));
    fileAppender->setFormate(formater);
    logger->addAppender(fileAppender);

    TIGERKIN_LOG_DEBUG(logger) << "Hello World";
    TIGERKIN_LOG_INFO(logger) << "Hello World";
    TIGERKIN_LOG_WARN(logger) << "Hello World";
    TIGERKIN_LOG_ERROR(logger) << "Hello World";
    TIGERKIN_LOG_FATAL(logger) << "Hello World";

    TIGERKIN_LOG_FMT_DEBUG(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_INFO(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_WARN(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_ERROR(logger, "fmt Hello %s", "World");
    TIGERKIN_LOG_FMT_FATAL(logger, "fmt Hello %s", "World");
}

void managerLogTest() {
    tigerkin::SingletonLoggerMgr::getInstance()->addLoggers("/home/liuhu/tigerkin/conf/log.yml", "logs");
    
    TIGERKIN_LOG_DEBUG(TIGERKIN_LOG_NAME(system)) << "I am system logger debug";
    TIGERKIN_LOG_INFO(TIGERKIN_LOG_NAME(system)) << "I am system logger info";
    TIGERKIN_LOG_WARN(TIGERKIN_LOG_NAME(system)) << "I am system logger warn";
    
    TIGERKIN_LOG_DEBUG(TIGERKIN_LOG_NAME(hall)) << "I am hall logger debug";
    TIGERKIN_LOG_INFO(TIGERKIN_LOG_NAME(hall)) << "I am hall logger info";
    TIGERKIN_LOG_WARN(TIGERKIN_LOG_NAME(hall)) << "I am hall logger warn";

    TIGERKIN_LOG_DEBUG(TIGERKIN_LOG_NAME(game)) << "I am game logger debug";
    TIGERKIN_LOG_INFO(TIGERKIN_LOG_NAME(game)) << "I am game logger info";
    TIGERKIN_LOG_WARN(TIGERKIN_LOG_NAME(game)) << "I am game logger warn";

    tigerkin::Logger::ptr logger = tigerkin::SingletonLoggerMgr::getInstance()->getLogger("game");
    tigerkin::SingletonLoggerMgr::getInstance()->deleteLogger(logger);
    TIGERKIN_LOG_DEBUG(TIGERKIN_LOG_NAME(game)) << "I am game logger debug";
    TIGERKIN_LOG_INFO(TIGERKIN_LOG_NAME(game)) << "I am game logger info";
    TIGERKIN_LOG_WARN(TIGERKIN_LOG_NAME(game)) << "I am game logger warn";
}

int main(int argc, char **argv) {
    std::cout << "test tigerkin log start" << std::endl;
    defaultLogTest();
    simpleLogTest();
    managerLogTest();
    std::cout << "test tigerkin log end" << std::endl;
    return 0;
}
