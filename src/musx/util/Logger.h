/*
 * Copyright (C) 2024, Robert Patterson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <functional>
#include <iostream>
#include <string>

namespace musx {
namespace util {

/**
 * @brief A utility class for handling logging with customizable callbacks.
 *
 * The `Logger` class provides a centralized mechanism for logging messages 
 * with different severity levels. By default, messages are sent to `std::cerr`,
 * but a custom logging callback can be registered to handle messages in other ways
 * (e.g., writing to a file, console, or network).
 */
class Logger {
public:
    /**
     * @brief Enum class representing the severity levels of log messages.
     */
    enum class LogLevel {
        Info,       ///< Informational messages for general program operation.
        Warning,    ///< Warning messages indicating potential issues.
        Error,      ///< Error messages indicating critical issues.
        Verbose     ///< Informational messages that should only displayed when verbose logging is requested.
    };

    /**
     * @brief Type alias for the logging callback function.
     *
     * The callback receives a log level and a message string.
     */
    using LogCallback = std::function<void(Logger::LogLevel, const std::string&)>;

    /**
     * @brief Sets a custom callback for handling log messages.
     *
     * @param callback A function that takes a log level and message string.
     * The callback will be invoked whenever a message is logged.
     * If no callback is provided, messages will default to `std::cerr`.
     */
    static void setCallback(LogCallback callback) {
        getInstance().m_callback = callback;
    }

    /**
     * @brief Retrieves the currently set logging callback.
     *
     * @return The current logging callback function or null if not set.
     */
    static LogCallback getCallback() {
        return getInstance().m_callback;
    }

    /**
     * @brief Logs a message with a specific severity level.
     *
     * @param level The severity level of the message.
     * @param message The message string to log.
     *
     * If a custom callback is registered, it will handle the message. Otherwise,
     * the default behavior writes the message to `std::cerr`.
     */
    static void log(LogLevel level, const std::string& message) {
        if (getInstance().m_callback) {
            getInstance().m_callback(level, message);
        } else {
            std::cerr << message << std::endl;
        }
    }

private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    Logger() = default;

    /**
     * @brief Retrieves the singleton instance of the Logger class.
     *
     * @return A reference to the singleton `Logger` instance.
     */
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    /// The logging callback function.
    LogCallback m_callback;
};

} // namespace util
} // namespace musx
