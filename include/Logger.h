/**
 * @file
 *
 * Declaration of the Logger class
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <experimental/filesystem>

class EvoBeeModel;
class Environment;


/**
 * The Logger class ...
 */
class Logger {

public:
    Logger(EvoBeeModel* pModel);
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
    std::experimental::filesystem::path m_MainLogFile;
    std::string m_strFilePrefix;
    std::string m_strConfigFileSuffix;
    std::string m_strMainLogFileSuffix;

    EvoBeeModel* m_pModel;
    Environment* m_pEnv;
};

#endif /* _LOGGER_H */
