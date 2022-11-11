#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-s-d.lib")
#pragma comment(lib,"sfml-audio-s-d.lib")
#pragma comment(lib,"sfml-system-s-d.lib")
#pragma comment(lib,"sfml-window-s-d.lib")
#pragma comment(lib,"sfml-network-s-d.lib")
#else
#pragma comment(lib,"sfml-graphics-s.lib")
#pragma comment(lib,"sfml-audio-s.lib")
#pragma comment(lib,"sfml-system-s.lib")
#pragma comment(lib,"sfml-window-s.lib")
#pragma comment(lib,"sfml-network-s.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib")

#include<iostream>

int tetris();
int doodle_jump();
int arkanoid();
int snake();
int minesweeper();
int fifteen_puzzle();

using namespace std;
int main()
{
    char key;
    while (true) {
        cout << "Choose a game. Enter the uppercase key in game name :\n";
        cout << "============================================\n";
        cout << "Tetris \n";
        cout << "Doodle jump\n";  
        cout << "Arkanoid\n";
        cout << "Snake\n";
        cout << "Minesweeper\n";
        cout << "Fifteen puzzle\n";


        cin >> key;

        switch (key) {
            case 'T':
            case 't':
                tetris();
                break;
            case 'D':
            case 'd':
                doodle_jump();
                break;
            case 'A':
            case 'a':
                arkanoid();
                break;
            case 'S':
            case 's':
                snake();
                break;
            case 'M':
            case 'm':
                minesweeper();
                break;
            case 'F':
            case 'f':
                fifteen_puzzle();
                break;
        }

    }
    //tetris();
    doodle_jump();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
