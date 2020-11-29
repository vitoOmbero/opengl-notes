#include <cstdlib>
#include <iostream>
#include <stdlib.h>

#include "main_loop.h"
#include "utils/macro_as_text.inc"
#include "utils/terminal.h"

void AtExit()
{
    Terminal::PrintBorderedText('+', "Bye-bye!");
}

int main(/*int argc, char* argv[]*/)
{
    Terminal::Init();
    Terminal::PrintBorderedText('*',
                                "Welcome to the " MacroAsText(OPENGL_DEMO) "!");

    const int result = std::atexit(AtExit);

    if (result != 0)
    {
        std::cerr << "AtExit registration failed\n";
        return EXIT_FAILURE;
    }

    const char *title = MacroAsText(OPENGL_DEMO);

    MainLoop::Init(title);
    MainLoop::Start();

    std::cout << "Exit from MainLoop" << std::endl;

    std::exit(EXIT_SUCCESS);
}
