#include<bits/stdc++.h>
#include"Board.h"

using namespace std;

Board::Board(int n)
    {
        int len=8;
        while (n > 0){
            a[len/3][len%3]=n%10;
            n/=10;
            len--;
        }
        for (int i=0; i < 3; i++)
            for (int j=0; j < 3; j++)
                if (a[i][j] == 1)
                    blank=i*3 + j;
    }
Board Board::moveUp()
    {
        if (blank < 3) return Board(getID());
        Board newBoard=*this;
        swap(newBoard.a[blank/3][blank%3],newBoard.a[blank/3-1][blank%3]);
        newBoard.blank-=3;
        return newBoard;
    }
Board Board::moveDown()
    {
        if (blank > 5) return Board(getID());
        Board newBoard=*this;
        swap(newBoard.a[blank/3][blank%3],newBoard.a[blank/3+1][blank%3]);
        newBoard.blank+=3;
        return newBoard;
    }
Board Board::moveLeft()
    {
        if (blank%3 == 0) return Board(getID());
        Board newBoard=*this;
        swap(newBoard.a[blank/3][blank%3],newBoard.a[blank/3][blank%3-1]);
        newBoard.blank--;
        return newBoard;
    }
Board Board::moveRigh()
    {
        if (blank%3 == 2) return Board(getID());
        Board newBoard=*this;
        swap(newBoard.a[blank/3][blank%3],newBoard.a[blank/3][blank%3+1]);
        newBoard.blank++;
        return newBoard;
    }
int Board::getID()
    {
        int P=0;
        for (int i=0; i < 3; i++)
            for (int j=0; j < 3; j++)
                P=P*10 + a[i][j];
        return P;
    }
