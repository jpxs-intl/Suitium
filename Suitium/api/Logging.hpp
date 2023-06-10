#pragma once

#include <fmt/format.h>
#include <string>
#include <type_traits>

namespace api
{
    class Logger
    {
    public:
        Logger() = delete;
        Logger(const std::string &name, const std::string &decoration = ""); // Empty decoration will give a random color
        Logger(const Logger &) = delete;
        ~Logger() noexcept = default;

    private:
        std::string _name;
        std::string _decoration;

    public:
        void LogText(const std::string &text) const;

        template<typename... Args>
        void Log(const std::string &format, Args&&... args)
        {
            this->LogText(fmt::format(format, std::forward<Args>(args)...));
        }

    private:
        static void InternalLog(const std::string &content);
    };
}