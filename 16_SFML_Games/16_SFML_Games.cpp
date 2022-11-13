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
int racing();
int outrun();
int xonix();
int bejeweled();
int netwalk();
int mahjong();
int tron();
int chess();
int volleyball() {/* TODO add box2D*/ return 0; }
int asteroids();


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
        cout << "Racing game (Micro-Machines)\n";
        cout << "Outrun\n";
        cout << "Xonix\n";
        cout << "Bejeweled\n";
        cout << "Netwalk (Pipes game)\n";
        cout << "mahJong solitare\n";
        cout << "tron U\n";
        cout << "Chess\n";
        cout << "Volleyball\n";
        cout << "astEroids\n";



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
            case 'r':
            case 'R':
                racing();
                break;
            case 'O':
            case 'o':
                outrun();
                break;
            case 'X':
            case 'x':
                xonix();
                break;    
            case 'b':
            case 'B':
                bejeweled();
                break;
            case 'n':
            case 'N':
                netwalk();
                break;
            case 'J':
            case 'j':
                mahjong();
                break;
            case 'u':
            case 'U':
                tron();
                break;
            case 'c':
            case 'C':
                chess();
                break;
            case 'v':
            case 'V':
                volleyball();
                break;
            case 'e':
            case 'E':
                asteroids();
                break;
        }

    }

}

