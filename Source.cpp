#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include "color.h"
#include <thread>
#include <atomic>
using namespace std;
bool on_off = false;
bool ADS = true;
int x = 0;
int randomizer_str = 3;
bool randomizer = false;

std::atomic<bool> running(false);
thread RandomizerThread;
void show_state()
{
    system("cls");
    if (on_off) {
        cout << dye::green("Macro enabled!") << " (Front Side Mouse Button)" << endl;
    }
    else {
        cout << dye::red("Macro disabled!") << " (Front Side Mouse Button)" << endl;
    }
    if (randomizer) {
        cout << dye::aqua("Randomizer enabled!") << " (Page UP Button)" << endl;
    }
    else {
        cout << dye::red("Randomizer disabled!") << " (Page UP Button)" << endl;
    }
    if (ADS) {

        cout << dye::green("ADS enabled!") << " (Back Side Mouse Button)" << endl;
    }
    else {
        cout << dye::red("ADS disabled!") << " (Back Side Mouse Button)" << endl;
    }
    cout << "\n";
    cout << dye::yellow("Downforce: ") << x << "  (+ Numpad8, - Numpad2)" << endl;
    cout << dye::aqua("Randomizer force: ") << randomizer_str << "  (+ Numpad9, - Numpad3)" << endl;
};
int getRandomY(int min, int max) {
    return min + std::rand() % (max - min + 1);
}
void SendMouseMove(int xOffset, int yOffset) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = xOffset;  
    input.mi.dy = yOffset;  
    input.mi.dwFlags = MOUSEEVENTF_MOVE;  
    SendInput(1, &input, sizeof(INPUT)); 
}

void SetRandomOffset() {
    while (running) {
        if (on_off) {
            if (ADS) {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                if (GetAsyncKeyState(0x01) && GetAsyncKeyState(0x02)) {
                    SendMouseMove(act_str, 0); 
                }
            }
            else {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                if (GetAsyncKeyState(0x01)) {
                    SendMouseMove(act_str, 0);  
                }
            }
        }
        Sleep(10);
    }
}

void SetRandomOffset3()
{
    while (running)
    {
        if (on_off)
        {
            if (ADS)
            {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                POINT p;
                GetCursorPos(&p);
                if (GetAsyncKeyState(0x01) && GetAsyncKeyState(0x02))
                {
                    SetCursorPos(p.x + act_str, p.y);
                }
                Sleep(1);
            }
            else if (!ADS)
            {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                POINT p;
                GetCursorPos(&p);
                if (GetAsyncKeyState(0x01))
                {
                    SetCursorPos(p.x + act_str, p.y);
                }
                Sleep(1);

            }
            else
            {
                break;
            }



        }

    }

}
void SetRandomOffset2()
{
    while (running)
    {
        if (on_off)
        {
            if (ADS)
            {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                if (GetAsyncKeyState(0x01) && GetAsyncKeyState(0x02))
                {
                    mouse_event(MOUSEEVENTF_MOVE_NOCOALESCE, act_str, 0, 0, 0);
                }
                Sleep(1);
            }
            else if (!ADS)
            {
                int act_str = getRandomY(-randomizer_str, randomizer_str);
                if (GetAsyncKeyState(0x01))
                {
                    mouse_event(MOUSEEVENTF_MOVE_NOCOALESCE, act_str, 0, 0, 0);
                }
                Sleep(1);

            }
            else
            {
                break;
            }



        }

    }

}
void StartRandomizerThread() {
    if (!running) {
        running = true;
        RandomizerThread = std::thread(SetRandomOffset);
        HANDLE hThread = RandomizerThread.native_handle();
        SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
    }
}
void StopRandmizerThread()
{
    if (running)
    {
        running = false;
        if (RandomizerThread.joinable())
        {
            RandomizerThread.join();
        }
    }
}
int main() {
    SetConsoleTitleA("!Recoil");
    show_state();
    

    while (true) {


        if (GetAsyncKeyState(0x21) & 1)
        {
            randomizer = !randomizer;
            if (randomizer)
            {
                StartRandomizerThread();
            }
            else
            {
                StopRandmizerThread();
            }


            show_state();
        }
        if (GetAsyncKeyState(0x69) & 1)
        {
            randomizer_str += 1;
            show_state();
        }
        if (GetAsyncKeyState(0x63) & 1)
        {
            if (randomizer_str - 1 <= 0)
            {
                continue;
            }
            randomizer_str -= 1;

            show_state();
        }

        if (GetAsyncKeyState(0x06) & 1)
        {
            on_off = !on_off;
            show_state();
        }
        if (GetAsyncKeyState(0x68) & 1)
        {

            x += 1;
            show_state();
        }
        if (GetAsyncKeyState(0x62) & 1)
        {
            x -= 1;
            show_state();
        }
        if (GetAsyncKeyState(0x05) & 1)
        {
            ADS = !ADS;
            show_state();
        }

        if (on_off)
        {
            if (ADS)
            {
                if (GetAsyncKeyState(0x02) && GetAsyncKeyState(0x01))
                {
                    mouse_event(MOUSEEVENTF_MOVE, 0, x, 0, 0);
                }
            }
            if (!ADS)
            {
                if (GetAsyncKeyState(0x01))
                {
                    mouse_event(MOUSEEVENTF_MOVE, 0, x, 0, 0);
                }
            }
        }



        Sleep(10);
    }

    return 0;
}
