#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "blocks.h"
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

using namespace std;
#define H 20
#define W 15
char board[H][W] = {};
int x, y, b;
int score = 0;
int sleepTime = 500;
blocks* currentBlock = nullptr;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void spawnBlock() {
    x = 5; y = 1;
    b = rand() % 7;
    if (currentBlock != nullptr) {
        delete currentBlock;
    }
    switch (b) {
    case 0: currentBlock = new block_I(); break;
    case 1: currentBlock = new block_O(); break;
    case 2: currentBlock = new block_T(); break;
    case 3: currentBlock = new block_S(); break;
    case 4: currentBlock = new block_Z(); break;
    case 5: currentBlock = new block_J(); break;
    case 6: currentBlock = new block_L(); break;
    }
}

bool canMove(int dx, int dy, char checkBlock[4][4]){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (checkBlock[i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W-1 || yt >= H-1 ) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}
void block2Board(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (currentBlock->shape[i][j] != ' ')
                board[y+i][x+j] = currentBlock->shape[i][j];
}
void boardDelBlock(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (currentBlock->shape[i][j] != ' ')
                board[y+i][x+j] = ' ';
}
void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if (i == 0 || i == H-1 || j ==0 || j == W-1) board[i][j] = '#';
            else board[i][j] = ' ';
}
void draw(){
    gotoxy(0, 0);
    for (int i = 0 ; i < H ; i++) {
        for (int j = 0 ; j < W ; j++) {
            if (board[i][j] == '#') cout << "\x1b[38;5;242m▓▓\x1b[0m";       
            else if (board[i][j] != ' ') {
                switch (board[i][j]) {
                    case 'O': cout << "\x1b[38;5;226m"; break; // Yellow O
                    case 'I': cout << "\x1b[38;5;51m"; break;  // Cyan I
                    case 'Z': cout << "\x1b[38;5;46m"; break;  // Green Z
                    case 'S': cout << "\x1b[38;5;196m"; break; // Red S
                    case 'L': cout << "\x1b[38;5;214m"; break; // Orange L
                    case 'J': cout << "\x1b[38;5;213m"; break; // Pink J
                    case 'T': cout << "\x1b[38;5;129m"; break; // Violet/Purple T
                }
                cout << "■ \x1b[0m";
            } else cout << "  ";                        
        }
        cout << endl;
    }
    cout << "\x1b[0m";
    gotoxy(W * 2 + 5, 4); 
    cout << "SCORE: " << score << " ";
}

void removeLine() {
    int i, j;
    int linesCleared = 0; // Biến đếm số hàng xóa được cùng lúc

    for (i = H - 2; i > 0; i--) {
        for (j = 1; j < W - 1; j++) // Chỉ quét khoảng trống bên trong viền (1 đến W-2)
            if (board[i][j] == ' ') break;

        if (j == W - 1) { // Nếu dòng đầy
            for (int ii = i; ii > 1; ii--)
                for (int jj = 1; jj < W - 1; jj++)
                    board[ii][jj] = board[ii - 1][jj];

            // Làm rỗng dòng cao nhất (ngay dưới viền) để dọn chỗ trống
            for (int jj = 1; jj < W - 1; jj++)
                board[1][jj] = ' ';

            i++; // Giữ nguyên index để check lại dòng vừa bị kéo xuống
            linesCleared++; // Tăng biến đếm khi xóa thành công 1 hàng
        }
    }

    // Tính điểm
    if (linesCleared == 1) score += 100;
    else if (linesCleared == 2) score += 300;
    else if (linesCleared == 3) score += 500;
    else if (linesCleared >= 4) score += 800;

    // Tăng độ khó bằng cách giảm thời gian rơi của block nếu có dòng bị xóa
    if (linesCleared > 0) {
        sleepTime -= 20;
        if (sleepTime < 50) sleepTime = 50;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    hideCursor();
    srand(time(0));
    initBoard();
    spawnBlock();

    // Lưu thời điểm cuối cùng block tự động rơi
    DWORD lastDropTime = GetTickCount();

    // Vẽ khung hình đầu tiên trước khi vào vòng lặp
    block2Board();
    draw();

    while (1){
        bool stateChanged = false; // Biến chỉ vẽ lại khi có sự thay đổi để mượt hơn
        boardDelBlock();

        if (kbhit()){
            int c = getch();
            if (c == 224 || c == 0) {
                c = getch();
                if (c == 75 && canMove(-1, 0, currentBlock->shape)) { x--; stateChanged = true; } // TRÁI
                if (c == 77 && canMove( 1, 0, currentBlock->shape)) { x++; stateChanged = true; } // PHẢI
                if (c == 80 && canMove( 0, 1, currentBlock->shape)) { y++; score += 1; stateChanged = true; } // XUỐNG
                if (c == 72) { currentBlock->rotateBlock(); stateChanged = true; }                // XOAY
            }
            else {
                if ((c == 'a' || c == 'A') && canMove(-1, 0, currentBlock->shape)) { x--; stateChanged = true; }
                if ((c == 'd' || c == 'D') && canMove( 1, 0, currentBlock->shape)) { x++; stateChanged = true; }
                if ((c == 's' || c == 'S') && canMove( 0, 1, currentBlock->shape)) { y++; score += 1; stateChanged = true; }
                if (c == 'w' || c == 'W') { currentBlock->rotateBlock(); stateChanged = true; }
                if (c == 'q' || c == 'Q') break;
            }
        }

        DWORD currentTime = GetTickCount();
        if (currentTime - lastDropTime >= (DWORD)sleepTime) {
            if (canMove(0, 1, currentBlock->shape)) {
                y++;
                stateChanged = true; // Block đã di chuyển xuống -> vẽ lại
            } else {
                block2Board();
                removeLine();
                spawnBlock();
                if (!canMove(0, 0, currentBlock->shape)) break;
                stateChanged = true; // Block mới xuất hiện -> vẽ lại
            }
            lastDropTime = currentTime;
        }
        block2Board();
        if (stateChanged) {
            draw();
        }
        Sleep(20);
    }
    return 0;
}
