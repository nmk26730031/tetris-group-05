#include "blocks.h"

// ---------------- CÀI ĐẶT LỚP CƠ SỞ ----------------
blocks::blocks() {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            shape[i][j] = ' ';
}

void blocks::rotateBlock() {
    char temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][3-i] = shape[i][j];
        }
    }
    if (canMove(0, 0, temp)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }
}

blocks::~blocks() {}

// ---------------- CÀI ĐẶT CÁC LỚP CON ----------------
block_I::block_I() {
    shape[0][1] = 'I'; shape[1][1] = 'I'; shape[2][1] = 'I'; shape[3][1] = 'I';
}

block_O::block_O() {
    shape[1][1] = 'O'; shape[1][2] = 'O'; 
    shape[2][1] = 'O'; shape[2][2] = 'O';
}
void block_O::rotateBlock() {
    // Ghi đè để không xoay khối O
}

block_T::block_T() {
    shape[1][1] = 'T'; 
    shape[2][0] = 'T'; shape[2][1] = 'T'; shape[2][2] = 'T';
}

block_S::block_S() {
    shape[1][1] = 'S'; shape[1][2] = 'S'; 
    shape[2][0] = 'S'; shape[2][1] = 'S';
}

block_Z::block_Z() {
    shape[1][0] = 'Z'; shape[1][1] = 'Z'; 
    shape[2][1] = 'Z'; shape[2][2] = 'Z';
}

block_J::block_J() {
    shape[1][0] = 'J'; 
    shape[2][0] = 'J'; shape[2][1] = 'J'; shape[2][2] = 'J';
}

block_L::block_L() {
    shape[1][2] = 'L'; 
    shape[2][0] = 'L'; shape[2][1] = 'L'; shape[2][2] = 'L';
}