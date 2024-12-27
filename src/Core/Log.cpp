//
// Created by colli on 12/26/2024.
//

#include "Core/Log.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace Log {
    Log& Log::Instance() {
        static Log instance;
        return instance;
    }

    Log::Log() {
        // Open the log file
        file.open("Log.txt", std::ofstream::out | std::ofstream::trunc);
        if(!file.is_open()) {
            std::cerr << "Failed to open log file." << std::endl;
        }
    }

    Log::~Log() {
        if(file.is_open()) {
            file.close();
        }
    }

    void Log::Output(Severity severity, const std::string &message) {
        // Let only one thread write at a time
        std::lock_guard<std::mutex> lock(mutex);

        // Severity step
        std::string severity_str;
        std::string color_str = "\033[37m";
        switch(severity) {
            case Severity::HAPPY:
                color_str = "\033[32m";
                severity_str = "HAPPY";
                break;
            case Severity::MESSAGE:
                // color stays white
                severity_str = "MESSAGE";
                break;
            case Severity::WARNING:
                color_str = "\033[33m";
                severity_str = "WARNING";
                break;
            case Severity::ERROR:
                color_str = "\033[31m";
                severity_str = "ERROR";
                break;
        }

        // Timestamp step
        const std::string timestamp_str = GetTime();

        // Final string
        std::string log_str = "[" + timestamp_str + "][" + severity_str + "] " + message;

        // Debug mode only: print to console
        #ifdef DEBUG_MODE
            std::cout << color_str << log_str << std::endl;
        #endif

        // write to file
        if(file.is_open()) {
            file << log_str << std::endl;
        }
    }

    std::string Log::GetTime() {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t now_t = system_clock::to_time_t(now);

        std::tm local_time;
        #ifdef _WIN32
            localtime_s(&local_time, &now_t);
        #else
            localtime_r(&now_t, &local_time);
        #endif

        // format
        // YYYY-MM-DD HH:MM:SS
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
        return std::string(buffer);
    }

} // Log