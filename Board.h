#ifndef _Board_H
#define _Board_H

struct Board{
    int blank;
    int a[3][3];
    Board();
    Board(int n);
    Board moveLeft();
    Board moveRigh();
    Board moveUp();
    Board moveDown();
    int getID();
};
#endif // _Board_H
