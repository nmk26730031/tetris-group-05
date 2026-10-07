#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "blocks.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

using namespace std;
#define H 20
#define W 15
char board[H][W] = {};
int x, y, b;
int score = 0;
string playerName;
bool isPaused = false;
int sleepTime = 500;
blocks* currentBlock = nullptr;
blocks* nextBlock = nullptr;
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
    if (currentBlock != nullptr) {
        delete currentBlock;
    }
    currentBlock = nextBlock; 
    // Sinh ra khối mới cho lần tiếp theo
    int random_b = rand() % 7;
    switch (random_b) {
        case 0: nextBlock = new block_I(); break;
        case 1: nextBlock = new block_O(); break;
        case 2: nextBlock = new block_T(); break;
        case 3: nextBlock = new block_S(); break;
        case 4: nextBlock = new block_Z(); break;
        case 5: nextBlock = new block_J(); break;
        case 6: nextBlock = new block_L(); break;
    }
}

bool canMove(int dx, int dy, char checkBlock[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (checkBlock[i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W - 1 || yt >= H - 1) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}
void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock->shape[i][j] != ' ')
                board[y + i][x + j] = currentBlock->shape[i][j];
}
void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock->shape[i][j] != ' ')
                board[y + i][x + j] = ' ';
}
void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1) board[i][j] = '#';
            else board[i][j] = ' ';
}
void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
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
                cout << "■ \x1b[0m"; // reset block color
            } else cout << "  ";                        
        }
        cout << endl;
    }
    cout << "\x1b[0m";
    // In tên ra màn hình
    gotoxy(W * 2 + 5, 2); 
    cout << "PLAYER: " << playerName;
    
    // In điểm ra màn hình
    gotoxy(W * 2 + 5, 4);
    cout << "SCORE: " << score << " ";
  
    // In PAUSED ra màn hình
    gotoxy(W * 2 + 5, 6);
    if (isPaused) cout << ">>> PAUSED <<<";
    else cout << "              ";
    
    // In khối tiếp theo ra màn 
    gotoxy(W * 2 + 5, 9); 
    cout << "NEXT BLOCK:";
    for(int i = 0; i < 4; i++) {
        gotoxy(W * 2 + 9, 10 + i); 
        for(int j = 0; j < 4; j++) {
            char c = nextBlock->shape[i][j];
            if (c != ' ') {
                switch (c) {
                    case 'O': cout << "\x1b[38;5;226m"; break; // Yellow O
                    case 'I': cout << "\x1b[38;5;51m"; break;  // Cyan I
                    case 'Z': cout << "\x1b[38;5;46m"; break;  // Green Z
                    case 'S': cout << "\x1b[38;5;196m"; break; // Red S
                    case 'L': cout << "\x1b[38;5;214m"; break; // Orange L
                    case 'J': cout << "\x1b[38;5;213m"; break; // Pink J
                    case 'T': cout << "\x1b[38;5;129m"; break; // Violet/Purple T
                }
                cout << "■ \x1b[0m"; // reset block color
            } else {
                cout << "  "; 
            }
        }
    }
    
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

    // Yêu cầu nhập tên trước khi vào game
    cout << "============= TETRIS =============\n";
    cout << "Nhap ten cua ban: ";
    getline(cin, playerName);
    system("cls"); // Xóa toàn bộ màn hình để vẽ game

    // Ẩn con trỏ chuột để không bị nhấp nháy
    hideCursor();
    srand(time(0));

    // Khởi tạo bảng
    initBoard();

    // Phát nhạc nền
    PlaySound(TEXT("theme_tetris.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // Sinh khối ngẫu nhiên trước
    int initial_b = rand() % 7;
    switch (initial_b) {
        case 0: nextBlock = new block_I(); break;
        case 1: nextBlock = new block_O(); break;
        case 2: nextBlock = new block_T(); break;
        case 3: nextBlock = new block_S(); break;
        case 4: nextBlock = new block_Z(); break;
        case 5: nextBlock = new block_J(); break;
        case 6: nextBlock = new block_L(); break;
    }
    // Sinh khối
    spawnBlock();

    // Lưu thời điểm cuối cùng block tự động rơi
    DWORD lastDropTime = GetTickCount();

    // Vẽ khung hình đầu tiên trước khi vào vòng lặp
    block2Board();
    draw();

    while (1) {
        bool stateChanged = false; // Biến chỉ vẽ lại khi có sự thay đổi để mượt hơn
        boardDelBlock();

        if (_kbhit()) {
            int c = _getch();
            if (c == 224 || c == 0) {
                c = _getch();
                // Chỉ cho phép di chuyển khi KHÔNG tạm dừng
                if (!isPaused) {
                    if (c == 75 && canMove(-1, 0, currentBlock->shape)) { x--; stateChanged = true; } // TRÁI
                    if (c == 77 && canMove(1, 0, currentBlock->shape)) { x++; stateChanged = true; } // PHẢI
                    if (c == 80 && canMove(0, 1, currentBlock->shape)) { y++; score += 1; stateChanged = true; } // XUỐNG
                    if (c == 72) { currentBlock->rotateBlock(); stateChanged = true; }                // XOAY
                }
            }
            else {
                // Nhấn P để dừng game
                if (c == 'p' || c == 'P') {
                    isPaused = !isPaused;
                    stateChanged = true; // Gọi để in PAUSED bên phải màn hình
                }
                else if (c == 'q' || c == 'Q') break;
                // Chỉ cho phép di chuyển khi KHÔNG tạm dừng
                else if (!isPaused) {
                    if ((c == 'a' || c == 'A') && canMove(-1, 0, currentBlock->shape)) { x--; stateChanged = true; }
                    if ((c == 'd' || c == 'D') && canMove(1, 0, currentBlock->shape)) { x++; stateChanged = true; }
                    if ((c == 's' || c == 'S') && canMove(0, 1, currentBlock->shape)) { y++; score += 1; stateChanged = true; }
                    if (c == 'w' || c == 'W') { currentBlock->rotateBlock(); stateChanged = true; }
                }
            }
        }

        DWORD currentTime = GetTickCount();
        if (isPaused) {
            // Đóng băng mốc thời gian
            lastDropTime = currentTime;
        }
        else if (currentTime - lastDropTime >= (DWORD)sleepTime) {
            // Nếu di chuyển được thì tăng y
            if (canMove(0, 1, currentBlock->shape)) {
                y++;
                stateChanged = true; // Block đã di chuyển xuống -> vẽ lại
            }
            // Nếu không di chuyển được thì gán block, kiểm tra xóa dòng và sinh block mới
            else {
                block2Board();
                removeLine();
                spawnBlock();
                // Nếu sinh khối mới bị kẹt thì kết thúc game
                if (!canMove(0, 0, currentBlock->shape)) break;
                stateChanged = true; // Block mới xuất hiện -> vẽ lại
            }
            lastDropTime = currentTime;
        }
        // Gán block vào bảng và vẽ lại nếu có sự thay đổi
        block2Board();
        if (stateChanged) {
            draw();
        }
        // Ngủ 20ms để giảm tải CPU và tránh nhấp nháy màn hình
        Sleep(20);
    }
    // Giải phóng bộ nhớ
    if (currentBlock != nullptr) delete currentBlock;
    if (nextBlock != nullptr) delete nextBlock;
    return 0;

}
