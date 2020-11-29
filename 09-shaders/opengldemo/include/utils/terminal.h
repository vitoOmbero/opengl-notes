#ifndef TERMINAL_H
#define TERMINAL_H

#include <string_view>

class Terminal
{
public:
    static void Init()
    {
        if (!is_inited)
            // NOTE: class has no state yet
            Terminal();
        else
            ReportErr("Terminal is already initialized.");
    };
    static void PrintBorderedText(const char filler, std::string_view msg);
    static void ReportErr(std::string_view msg);
    static void ReportErr(void* msg);
    static void ReportMsg(std::string_view msg);

private:
    Terminal();

    static bool      is_inited;
    static const int WIDTH = 50;
};

#endif // TERMINAL_H
