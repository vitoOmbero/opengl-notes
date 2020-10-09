#include <cstdlib>

#include "demo.h"
#include "utils/macro_as_text.inc"
#include "utils/terminal.h"

void AtExit()
{
    Terminal::PrintBorderedText('+', "Bye-bye!");
}

#ifndef OPENGL_DEMO
#define OPENGL_DEMO opengl demo
#endif

int main(/*int argc, char* argv[]*/)
{
    Terminal::Init();
    Terminal::PrintBorderedText('*',
                                "Welcome to the " MacroAsText(OPENGL_DEMO) "!");

    const int result = std::atexit(AtExit);

    if (result != 0)
    {
        Terminal::ReportErr("AtExit registration failed\n");
        return EXIT_FAILURE;
    }

    const char *title = MacroAsText(OPENGL_DEMO);

    Demo::Init(title);
    Demo::Start();

    Terminal::ReportMsg("Demo is finished.");
    std::exit(EXIT_SUCCESS);
}
