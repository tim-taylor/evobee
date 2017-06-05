/**
 * @file
 *
 * Declaration of the Logger class
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <experimental/filesystem>

/**
 * The Logger class ...
 */
class Logger {

public:
    Logger();
    ~Logger();
    
    /**
     *
     */
    void logExptSetup();

    /**
     *
     */
    void update();

private:
    std::experimental::filesystem::path m_LogDir;
    std::string     m_strLogFilePrefix;
};

#endif /* _LOGGER_H */
