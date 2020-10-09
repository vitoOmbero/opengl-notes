#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>

void PrintBorderedText(const char filler, std::string_view msg)
{
    const int WIDTH = 50;

    auto Line = [filler]() {
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout.fill(filler);
        std::cout << std::setw(WIDTH) << filler << std::endl;
    };
    auto Empty = [filler]() {
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.fill(' ');
        std::cout << filler << std::setw(WIDTH - 1) << filler << std::endl;
    };

    auto Text = [filler](std::string_view msg) {
        if (msg.length() > WIDTH - 4)
            std::cout << filler << ' ' << msg.substr(0, 46) << ' ' << filler
                      << std::endl;
        else
        {
            const int shift = msg.length() & 1 ? 1 : 0;
            std::cout.fill(' ');
            auto empty = WIDTH / 2 - shift - msg.length() / 2;
            std::cout.setf(std::ios::left, std::ios::adjustfield);
            std::cout << std::setw(empty) << filler;
            std::cout << msg;
            std::cout.setf(std::ios::right, std::ios::adjustfield);
            std::cout << std::setw(empty + shift) << filler;
            std::cout << std::endl;
        }
    };

    Line();
    Empty();
    Text(msg);
    Empty();
    Line();
}

void AtExit()
{
    PrintBorderedText('+', "Bye-bye!");
}

int main(/*int argc, char* argv[]*/)
{
#define OPENGL_DEMO opengl demo
#define ASTEXT(x) #x
#define MacroAsText(x) ASTEXT(x)

    // setup stdout
    PrintBorderedText('*', "Welcome to the " MacroAsText(OPENGL_DEMO) "!");

    const int result = std::atexit(AtExit);

    if (result != 0)
    {
        std::cerr << "AtExit registration failed\n";
        return EXIT_FAILURE;
    }

    // setup cfg
    const char* TITLE      = MacroAsText(OPENGL_DEMO);
    const int   WIN_WIDTH  = 800;
    const int   WIN_HEIGHT = 600;

    // try to do smth useful
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* win =
        glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, TITLE, nullptr, nullptr);

    if (nullptr == win)
    {
        std::cerr << "Window initialization failed" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    // BEHOLD the MAIN LOOP
    while (!glfwWindowShouldClose(win))
    {
        glfwPollEvents();

        glClearColor(.24, .12, .24, 1.);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win);
    }

    // thx capt!
    std::cout << "Window is closed" << std::endl;

    // normal exit
    glfwTerminate();
    return EXIT_SUCCESS;
}
