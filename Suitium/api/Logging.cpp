#include "Logging.hpp"

#include <cstdio>
#include <sstream>

#include "../Addresses.hpp"

#if _WIN32
#include <Windows.h>  
#endif

namespace api
{

    Logger::Logger(const std::string &name, const std::string &decoration)
    {
        this->_name = name;
        this->_decoration = decoration;
    }

    void Logger::LogText(const std::string &text) const
    {
        std::stringstream stream;
        stream << this->_decoration << '[' << this->_name << "]:<reset> " << text << std::endl;
        InternalLog(stream.str());
    }

    void Logger::InternalLog(const std::string &content)
    {
        if (!*addresses::IsDedicated)
            return; // TODO: Add logging for the client

#if _WIN32
        static HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(stdOutput, &consoleInfo);
        WORD originalAttributes = consoleInfo.wAttributes;

        WORD attributes = 0;
        bool rainbowEnabled = false;
#endif
    
        std::size_t printCount = 0;
        for (auto it = content.begin(); it != content.end(); ++it)
        {
#if _WIN32
            SetConsoleTextAttribute(stdOutput, 0 ? originalAttributes : attributes);
            if (rainbowEnabled)
            {
                WORD colors[] = {
                    FOREGROUND_RED,
                    FOREGROUND_RED | FOREGROUND_GREEN,
                    FOREGROUND_GREEN,
                    FOREGROUND_BLUE,
                    FOREGROUND_RED | FOREGROUND_BLUE
                };
                SetConsoleTextAttribute(stdOutput, colors[printCount % (sizeof(colors) / sizeof(colors[0]))]);
            }
#endif

            if (*it != '<')
            {
                std::putc(*it, stdout);
                printCount++;
                continue;
            }

            bool okTag = false;
            std::stringstream tagStream;
            
            std::string remainingContent = std::string(it, content.end());
            remainingContent.erase(remainingContent.begin()); // erase the first character which is <
            while (1)
            {
                if (remainingContent.size() <= 0)
                    break;
                if (remainingContent.front() == '>')
                {
                    okTag = true;
                    break;
                }
                tagStream << remainingContent.front();
                remainingContent.erase(remainingContent.begin());
            }

            if (!okTag)
                continue;

            std::string tag = tagStream.str();
            it += tag.size() + 1;

            if (tag == "reset")
            {
#if _WIN32
                attributes = originalAttributes;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "red")
            {
#if _WIN32
                attributes |= FOREGROUND_RED;
                attributes &= ~FOREGROUND_GREEN;
                attributes &= ~FOREGROUND_BLUE;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "green")
            {
#if _WIN32
                attributes &= ~FOREGROUND_RED;
                attributes |= FOREGROUND_GREEN;
                attributes &= ~FOREGROUND_BLUE;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "blue")
            {
#if _WIN32
                attributes &= ~FOREGROUND_RED;
                attributes &= ~FOREGROUND_GREEN;
                attributes |= FOREGROUND_BLUE;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "yellow")
            {
#if _WIN32
                attributes |= FOREGROUND_RED;
                attributes |= FOREGROUND_GREEN;
                attributes &= ~FOREGROUND_BLUE;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "cyan")
            {
#if _WIN32
                attributes &= ~FOREGROUND_RED;
                attributes |= FOREGROUND_GREEN;
                attributes |= FOREGROUND_BLUE;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "bright")
            {
#if _WIN32
                attributes |= FOREGROUND_INTENSITY;
                rainbowEnabled = false;
#endif
            }
            else if (tag == "/bright")
            {
#if _WIN32
                attributes &= ~FOREGROUND_INTENSITY;
                rainbowEnabled = false;
#endif
            }
            if (tag == "rainbow")
            {
#if _WIN32
                rainbowEnabled = true;
#endif
            }
            else if (tag == "/rainbow")
            {
#if _WIN32
                rainbowEnabled = false;
#endif
            }
        }

#if _WIN32
        SetConsoleTextAttribute(stdOutput, originalAttributes);
#endif
    }
}