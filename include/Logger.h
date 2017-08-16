/**
 * @file
 *
 * Declaration of the Logger class
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <iostream>
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
    void logPollinatorsFull();

     /**
     *
     */
    void logFlowersFull();

    /**
     *
     */
    void logFlowersSummary();

    /**
     *
     */
    void transferFilesToFinalDir();

private:

    std::ofstream openLogFile(); // a private helper method

    std::experimental::filesystem::path m_LogDir;
    std::experimental::filesystem::path m_MainLogFilePath;
    std::experimental::filesystem::path m_ConfigFilePath;
    std::string m_strFilePrefix;
    std::string m_strConfigFileSuffix;
    std::string m_strMainLogFileSuffix;
    std::string m_strConfigFilename;
    std::string m_strMainLogFilename;

    EvoBeeModel* m_pModel;
    Environment* m_pEnv;
};

#endif /* _LOGGER_H */
