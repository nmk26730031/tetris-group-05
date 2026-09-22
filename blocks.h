#pragma once
#define H 20
#define W 15


extern int x;
extern int y;
extern char board[H][W];
bool canMove(int dx, int dy, char checkBlock[4][4]);


class blocks {
public:
    char shape[4][4];

    blocks();
    virtual void rotateBlock();
    virtual ~blocks();
};
