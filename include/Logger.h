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
     * Log summary details of pollinators at the end of each foraging phase
     * Designated by 'p' in log-flags param in JSON config file
     */
    void logPollinatorsInterPhaseSummary();

    /**
     * Log full details of pollinators every log-update-period steps within each foraging phase
     * Designated by 'Q' in log-flags param in JSON config file
     */
    void logPollinatorsIntraPhaseFull();

    /**
     * Log full details of flowers at the end of each foraging phase
     * Designated by 'F' in log-flags param in JSON config file
     */
    void logFlowersInterPhaseFull();

    /**
     * Log summary details of flowers at the end of each foraging phase
     * Designated by 'f' in log-flags param in JSON config file
     */
    void logFlowersInterPhaseSummary();

    /**
     * Log summary details of flowers every log-update-period steps within each foraging phase
     * Designated by 'g' in log-flags param in JSON config file
     */
    void logFlowersIntraPhaseSummary();

    /**
     *
     */
    void transferFilesToFinalDir();

private:

    std::ofstream openLogFile(); // a private helper method

    std::experimental::filesystem::path m_LogDir;
    std::experimental::filesystem::path m_MainLogFilePath;
    std::experimental::filesystem::path m_ConfigFilePath;
    std::experimental::filesystem::path m_RunInfoFilePath;

    std::string m_strFilePrefix;

    std::string m_strConfigFileSuffix;
    std::string m_strMainLogFileSuffix;
    std::string m_strRunInfoFileSuffix;

    std::string m_strConfigFilename;
    std::string m_strMainLogFilename;
    std::string m_strRunInfoFilename;

    EvoBeeModel* m_pModel;
    Environment* m_pEnv;
};

#endif /* _LOGGER_H */
