#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "blocks.h"

using namespace std;
#define H 20
#define W 15
char board[H][W] = {};
int x, y, b;
int sleepTime = 500;
blocks* currentBlock = nullptr;
char blocks[][4][4] ={
    {{' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}}, // I
    {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}}, // O
    {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}}, // T
    {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}}, // S
    {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}}, // Z
    {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}}, // J
    {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}}  // L
};

void spawnBlock() {
    x = 5; y = 0; 
    b = rand() % 7;
    if (currentBlock != nullptr) {
        delete currentBlock;
    }
    switch(b) {
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
    system("cls");
    for (int i = 0 ; i < H ; i++) {
        for (int j = 0 ; j < W ; j++) {
            if (board[i][j] == '#') cout << "##";       // Vẽ viền
            else if (board[i][j] != ' ') cout << "[]";  // Vẽ block
            else cout << "  ";                        // Vẽ khoảng trống
        }
        cout << endl;
    }
}
void removeLine() {
    int i, j;
    bool lineRemoved = false;
    for (i = H - 2; i > 0; i--) {
        for (j = 0; j < W; j++)
            if (board[i][j] == ' ') break;
        if (j == W) { // Nếu dòng đầy
            for (int ii = i; ii > 0; ii--)
                for (int jj = 0; jj < W; jj++)
                    board[ii][jj] = board[ii - 1][jj];
            i++; // Giữ nguyên index để check lại dòng vừa rớt xuống
            lineRemoved = true;
            draw();
            Sleep(200);
        }
    }
    if (lineRemoved) {
        sleepTime -= 20;
        if (sleepTime < 50) sleepTime = 50; // Giới hạn tốc độ tối đa
    }
}

void rotateBlock() {
    char temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][3-i] = currentBlock[i][j];
        }
    }
    if (canMove(0, 0, temp)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                currentBlock[i][j] = temp[i][j];
    }
}

int main()
{
    srand(time(0));
    initBoard();
    spawnBlock();
    
    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c == 'a' && canMove(-1, 0, currentBlock->shape)) x--;
            if (c == 'd' && canMove( 1, 0, currentBlock->shape)) x++;
            if (c == 's' && canMove( 0, 1, currentBlock->shape)) y++; 
            if (c == 'w') currentBlock->rotateBlock();                       
            if (c == 'q') break;
        }
        if (canMove(0, 1, currentBlock->shape)) y++;
        else{
            block2Board();
            removeLine();
            spawnBlock();
            if (!canMove(0, 0, currentBlock->shape)) break; 
        }
        block2Board();
        draw();
        Sleep(sleepTime); 
    }
    return 0;
}
