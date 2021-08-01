/*****************************************************************
 * Description log systeam
 * Email huxiaoheigame@gmail.com
 * Created on 2021/07/24
 * Copyright (c) 2021 虎小黑
 ****************************************************************/

#include "log.h"

#include <stdarg.h>
#include <time.h>

namespace tigerkin {

LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line,
                   uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time)
    : m_logger(logger), m_level(level), m_file(file), m_line(line), m_elapse(elapse), m_threadId(threadId), m_fiberId(fiberId), m_time(time) {
}

void LogEvent::format(const char *fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char *fmt, va_list al) {
    char *buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if (len != -1) {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

LogEventWrap::LogEventWrap(LogEvent::ptr e)
    : m_event(e) {
}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

std::stringstream &LogEventWrap::getSS() {
    return m_event->getSS();
}

const std::string LogLevel::toString(LogLevel::Level level) {
    switch (level) {
#define XX(name)         \
    case LogLevel::name: \
        return #name;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}

LogLevel::Level LogLevel::fromString(const std::string &str) {
#define XX(level, v)            \
    if (str == #v) {            \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);

    return LogLevel::DEBUG;
#undef XX
}

class MessageFormatItem : public LogFormatter::FormatItem {
   public:
    MessageFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
   public:
    LevelFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << LogLevel::toString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
   public:
    ElapseFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem {
   public:
    NameFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << logger->getName();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
   public:
    ThreadIdFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << event->getThreadId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
   public:
    ThreadNameFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << event->getThreadName();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
   public:
    FiberIdFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger,
                LogLevel::Level level, LogEvent::ptr event) {
        os << event->getFiberId();
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
   public:
    DateTimeFormatItem(const std::string &fmt = "%Y-%m-%d %H:%M:%S")
        : m_format(fmt) {
        if (m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream &os, Logger::ptr logger,
                LogLevel::Level level, LogEvent::ptr event) {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }

   private:
    std::string m_format;
};

class FileNameFormatItem : public LogFormatter::FormatItem {
   public:
    FileNameFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger,
                LogLevel::Level level, LogEvent::ptr event) {
        os << event->getFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
   public:
    LineFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger,
                LogLevel::Level level, LogEvent::ptr event) {
        os << event->getLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
   public:
    NewLineFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << std::endl;
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
   public:
    StringFormatItem(const std::string &fmt = "")
        : m_string(fmt) {}
    void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) {
        os << m_string;
    }

   private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
   public:
    TabFormatItem(const std::string &fmt = "") {}
    void format(std::ostream &os, Logger::ptr logger,
                LogLevel::Level level, LogEvent::ptr event) {
        os << "\t";
    }

   private:
    std::string m_string;
};

LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern) {
    init();
}

void LogFormatter::init() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        if ((i + 1) < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                ++i;
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;
        std::string str;
        std::string fmt;
        while (n < m_pattern.size()) {
            if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' &&
                                m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1;
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if (fmt_status == 1) {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if (n == m_pattern.size()) {
                if (str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }

        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        } else if (fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << " - "
                      << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string &str)>> s_format_items = {
#define XX(str, C)                                                               \
    {                                                                            \
#str, [](const std::string &fmt) { return FormatItem::ptr(new C(fmt)); } \
    }

        XX(m, MessageFormatItem),     // m:消息
        XX(p, LevelFormatItem),       // p:日志级别
        XX(r, ElapseFormatItem),      // r:累计毫秒数
        XX(c, NameFormatItem),        // c:日志名称
        XX(t, ThreadIdFormatItem),    // t:线程id
        XX(n, NewLineFormatItem),     // n:换行
        XX(d, DateTimeFormatItem),    // d:时间
        XX(f, FileNameFormatItem),    // f:文件名
        XX(l, LineFormatItem),        // l:行号
        XX(T, TabFormatItem),         // T:Tab
        XX(F, FiberIdFormatItem),     // F:协程id
        XX(N, ThreadNameFormatItem),  // N:线程名称
#undef XX
    };

    for (auto &i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(
                FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(
                    "<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
    }
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger,
                                 LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto &i : m_items) {
        i->format(ss, logger, level, event);
    }
    return ss.str();
}

std::ostream &LogFormatter::format(std::ostream &ofs,
                                   std::shared_ptr<Logger> logger,
                                   LogLevel::Level level, LogEvent::ptr event) {
    for (auto &i : m_items) {
        i->format(ofs, logger, level, event);
    }
    return ofs;
}

void StdOutLogAppend::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                          LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(logger, level, event);
    }
}

FileLogAppend::FileLogAppend(const std::string &filename)
    : m_filename(filename) {
    if (!reopen())
        std::cerr << "open log file error: " << filename << std::endl;
}

bool FileLogAppend::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

void FileLogAppend::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                        LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

Logger::Logger(const std::string &name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S} %t %N %F [%p] [%c] %f:%l %m%n"));
}

void Logger::addAppender(LogAppender::ptr appender) {
    if (!appender->getFormate()) {
        appender->setFormate(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    auto it = m_appenders.begin();
    while (it != m_appenders.end()) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
        it++;
    }
}

void Logger::log(LogLevel::Level level, const LogEvent::ptr event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        for (auto &i : m_appenders) {
            i->log(self, level, event);
        }
    }
}

void Logger::debug(const LogEvent::ptr event) {
    log(LogLevel::Level::DEBUG, event);
}

void Logger::info(const LogEvent::ptr event) {
    log(LogLevel::Level::INFO, event);
}

void Logger::warn(const LogEvent::ptr event) {
    log(LogLevel::Level::WARN, event);
}

void Logger::error(const LogEvent::ptr event) {
    log(LogLevel::Level::ERROR, event);
}

void Logger::fatal(const LogEvent::ptr event) {
    log(LogLevel::Level::FATAL, event);
}

}  // namespace tigerkin