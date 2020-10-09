#ifndef MAINLOOP_H
#define MAINLOOP_H

class Demo
{
public:
    static void Init(const char* title, bool fullscreen = false);
    static void Terminate();

    static void Start();

    ~Demo();

private:
    Demo();
};

#endif // MAINLOOP_H
