//
// Created by colli on 12/26/2024.
//

#ifndef LOG_HPP
#define LOG_HPP
#include <fstream>
#include <mutex>
#include <string>

namespace Log {

    // Severity levels enum
    enum class Severity {
        HAPPY,
        MESSAGE,
        WARNING,
        ERROR
    };

class Log {
public:
    // Retrieve instance
    static Log& Instance();

    // Log message with severity level
    void Output(Severity severity = Severity::MESSAGE, const std::string& message = "ERR");

    // Delete, copy, and move constructors/assignments
    // Ensures singleton
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;
private:
    Log();
    ~Log();

    // Returns string timestamp
    std::string GetTime();

    // Output file stream
    std::ofstream file;

    // Ensures thread safe operations
    std::mutex mutex;
};

    inline void Output(Severity severity = Severity::MESSAGE, const std::string& message = "ERR") {
        Log::Instance().Output(severity, message);
    }

} // Log

#endif //LOG_HPP
