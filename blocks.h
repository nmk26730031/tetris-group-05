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

class block_I : public blocks { public: block_I(); };
class block_O : public blocks { public: block_O(); void rotateBlock() override; };
class block_T : public blocks { public: block_T(); };
class block_S : public blocks { public: block_S(); };
class block_Z : public blocks { public: block_Z(); };
class block_J : public blocks { public: block_J(); };
class block_L : public blocks { public: block_L(); };
