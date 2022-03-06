#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>

#include"Board.h"
#include"Sol.h"

using namespace std;

int N=3;
int SCREEN_WIDTH=600;
int SCREEN_HEIGHT=600;
string WINDOW_TITLE = "QuickQuick";

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
SDL_Texture* texture=NULL;
TTF_Font* font=NULL;
int start;

void logSDLError(ostream& cout, string st, bool ok)
    {
        cout<<st<<"Error: "<<SDL_GetError()<<'\n';
        if (ok){SDL_Quit(); exit(1);}
    }
void initSDL()
    {
        SDL_Init(SDL_INIT_VIDEO);
        //if (SDL_Init(SDL_INIT_EVERYTHING)) logSDLError(std::cout,"SDL_Init",1);

        window=SDL_CreateWindow(WINDOW_TITLE.c_str(),SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        if (window == NULL) logSDLError(cout,"CreateWindow",1);

        renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);

        if (TTF_Init()){
            cout<<TTF_GetError()<<endl;
            exit(1);
        }
    }
void quitSDL()
    {
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyTexture(texture); texture=NULL;
        SDL_DestroyRenderer(renderer); renderer=NULL;
        SDL_DestroyWindow(window); window=NULL;
        SDL_Quit();
    }
void waitUntilKeyPress()
    {
        SDL_Event W;
        while (1 < 2){
            if (SDL_WaitEvent(&W)&&((W.type == SDL_KEYDOWN)||(W.type == SDL_QUIT))) return;
            SDL_Delay(100);
        }
    }
/*********************************************************************************************/
string convertToString(int X)
    {
        if (X <= 0) return "0";
        string rs="";
        while (X > 0) rs=rs+char(X%10 + '0'), X/=10;
        reverse(rs.begin(),rs.end());
        return rs;
    }
SDL_Rect grid(int id)
    {
        int x=(id%N) * (SCREEN_WIDTH/N);
        int y=(id/N) * (SCREEN_HEIGHT/N);
        return SDL_Rect{x,y,SCREEN_WIDTH/N,SCREEN_HEIGHT/N};
    }
void show_board(Board B)
    {
        SDL_RenderClear(renderer);
        SDL_Rect srcR,destR;
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++){
                if (B.a[i][j] == 1) continue;
                srcR=grid(B.a[i][j]-1);
                destR=grid(i*N + j);
                SDL_RenderCopy(renderer,texture,&srcR,&destR);
            }
        SDL_RenderPresent(renderer);
    }
void gap_move(Board A, Board B, SDL_Rect moving)
    {
        SDL_RenderClear(renderer);
        SDL_Rect srcR,destR;
        for (int i=0; i < N; i++)
            for (int j=0; j < N; j++){
                srcR=grid(A.a[i][j]-1);
                destR=grid(i*N + j);
                if (A.a[i][j] == 1) continue;
                if (B.a[i][j] == 1){
                    destR=moving;
                }
                SDL_RenderCopy(renderer,texture,&srcR,&destR);
            }
        SDL_RenderPresent(renderer);
    }
void moveBoard(Board &A, Board B)
    {
        int step=20, delay=4;
        SDL_Rect srcR=grid(B.blank), destR=grid(A.blank);

        while (abs(srcR.x-destR.x) >= step){
            int ki=(srcR.x-destR.x)/abs(srcR.x-destR.x);
            srcR.x-=ki*step;
            gap_move(A,B,srcR);
            SDL_Delay(delay);
        }
        while (abs(srcR.y-destR.y) >= step){
            int ki=(srcR.y-destR.y)/abs(srcR.y-destR.y);
            srcR.y-=ki*step;
            gap_move(A,B,srcR);
            SDL_Delay(delay);
        }
        A=B;
    }
void write(const char *t, const SDL_Color &color, SDL_Rect *pos)
    {
        SDL_Surface *message=TTF_RenderText_Solid(font,t,color);
        pos->w=message->w;
        pos->h=message->h;
        SDL_Texture *text=SDL_CreateTextureFromSurface(renderer,message);
        SDL_RenderCopy(renderer,text,NULL,pos);
        SDL_DestroyTexture(text);
        SDL_FreeSurface(message);
    }
void score_board()
    {
        SDL_Event event;
        while (SDL_WaitEvent(&event))
            switch (event.type){
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_b: return;
                    }
                default:{
                    SDL_RenderClear(renderer);
                    int time=SDL_GetTicks()-start;
                    int s=time/1000;
                    int m=s/60;
                    s=s%60;
                    SDL_Color color={222,237,39,255};
                    SDL_Rect pos={180,100,0,0};
                    string t="Current time:  "+convertToString(m)+" min "+convertToString(s)+" sec.";
                    write(t.c_str(),color,&pos);

                    color={186,74,202,255};
                    pos={200,175,0,0};
                    t="~~SCOREBOARD~~";
                    write(t.c_str(),color,&pos);

                    ifstream sbfile("score.txt");
                    color={243,75,33,255};
                    for (int i=1; i <= 3; i++){
                        sbfile>>m>>s;
                        pos.x=185;
                        pos.y+=50;
                        t="TOP  "+convertToString (i)+" :   "+convertToString(m)+" min "+convertToString(s)+" sec.";
                        write(t.c_str(),color,&pos);
                    }
                    sbfile.close();
                    SDL_RenderPresent(renderer);
                    break;
                }
            }
    }
void update_score()
    {
        int time=SDL_GetTicks()-start;
        int cur_s=time/1000;
        int cur_m=cur_s/60;
        cur_s=cur_s%60;
        vector<pair<int, int> > V; V.clear();

        ifstream sbfile("score.txt");
        for (int i=0; i < 3; i++){
            int m,s; sbfile>>m>>s;
            V.push_back({m,s});
        }
        sbfile.close();
        V.push_back({cur_m,cur_s});

        ofstream _sbfile("score.txt");
        sort(V.begin(),V.end());
        for (int i=0; i < 3; i++)
            _sbfile<<V[i].first<<' '<<V[i].second<<'\n';
        _sbfile.close();
    }
void show_instruction()
    {
        SDL_Event event;
        while (SDL_WaitEvent(&event)){
            SDL_RenderClear(renderer);
            string t;
            SDL_Color color={98,22,254,255};
            SDL_Rect pos;

            pos={195,100,0,0};
            t="->INSTRUCTION<-";
            write(t.c_str(),color,&pos);
            pos={180,150,0,0};
            t="~[S] to get solution!";
            write(t.c_str(),color,&pos);
            pos={180,200,0,0};
            t="~[R] to reset!";
            write(t.c_str(),color,&pos);
            pos={180,250,0,0};
            t="~[B] to see score!";
            write(t.c_str(),color,&pos);
            pos={180,300,0,0};
            t="~[ESC] to quit!";
            write(t.c_str(),color,&pos);
            color={247,30,63,255};
            pos={155,450,0,0};
            t="Press [SPACE] to continue...";
            write(t.c_str(),color,&pos);

            SDL_SetRenderDrawColor(renderer,114,56,220,100);
            pos={170,80,240,280};
            SDL_RenderDrawRect(renderer,&pos);
            SDL_RenderPresent(renderer);
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            switch (event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE: return;
                        case SDLK_ESCAPE: exit(0);
                    }
                break;
            }
        }
    }
void start_game(int level)
    {
        BFS();
        show_instruction();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,texture,NULL,NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(4000);
        start=SDL_GetTicks();
        Board B=Board(rand(level));
        show_board(B);

        SDL_Event event;
        bool sol=0;
        while (SDL_WaitEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_UP:{
                            moveBoard(B,B.moveDown());
                            break;
                        }
                        case SDLK_DOWN:{
                            moveBoard(B,B.moveUp());
                            break;
                        }
                        case SDLK_LEFT:{
                            moveBoard(B,B.moveRigh());
                            break;
                        }
                        case SDLK_RIGHT:{
                            moveBoard(B,B.moveLeft());
                            break;
                        }
                        case SDLK_s:{
                            while (B.getID() != 123456789){
                                int direct=trace(B.getID());
                                if (direct == 1) moveBoard(B,B.moveDown());
                                if (direct == 2) moveBoard(B,B.moveUp());
                                if (direct == 3) moveBoard(B,B.moveRigh());
                                if (direct == 4) moveBoard(B,B.moveLeft());
                                //cerr<<direct<<' '<<B.getID()<<'\n';
                                SDL_Delay(40);
                            }
                            //SDL_Delay(1500);
                            sol=1;
                            break;
                        }

                        case SDLK_r:{
                            B=Board(rand(level));
                            show_board(B);
                            start=SDL_GetTicks();
                            break;
                        }
                        case SDLK_ESCAPE: return;
                        case SDLK_b:{
                            score_board();
                            show_board(B);
                            break;
                        }
                    }
                break;
            }
            if (B.getID() == 123456789){
                cerr<<"GAME OVER!!!"<<'\n';
                if (!sol) update_score();
                break;
            }
        }
    }
int chooseLevel()
    {
        string t="CHOOSE LEVEL FROM [1] TO [5]";
        SDL_Color color={255,21,21,255};
        SDL_Rect pos={130,200,0,0};

        SDL_RenderClear(renderer);
        write(t.c_str(),color,&pos);

        SDL_SetRenderDrawColor(renderer,200,50,50,100);
        pos={120,190,385,53};
        SDL_RenderDrawRect(renderer,&pos);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderPresent(renderer);

        SDL_Event event;
        int rs=0;
        while (SDL_WaitEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:{
                    switch (event.key.keysym.sym){
                        case SDLK_1: rs=1; break;
                        case SDLK_2: rs=2; break;
                        case SDLK_3: rs=3; break;
                        case SDLK_4: rs=4; break;
                        case SDLK_5: rs=5; break;
                    }
                    break;
                }
            }
            if (rs) break;
        }
        pos={143,200,0,0};
        t="OKAY , LEVEL ";
        t+=char(rs+'0');
        t+=" IS LOADING...";
        SDL_RenderClear(renderer);
        write(t.c_str(),color,&pos);
        SDL_RenderPresent(renderer);
        return rs;
    }
int main(int argc, char* argv[])
{
    srand(time(NULL));
    initSDL();

    int pic=random(0,5);
    //string path=string("img/girls/picture") + convertToString(pic) + ".bmp"; /*0-10*/
    string path=string("img/test/picture") + convertToString(pic) + ".bmp"; /*0-5*/
    //string path=string("img/picture") + convertToString(pic) + ".bmp";/*0-3*/
    SDL_Surface *image=SDL_LoadBMP(path.c_str());
    texture=SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);

    font=TTF_OpenFont("font.ttf",24);

    int level=chooseLevel();
    cerr<<"LOADING... \n";

    start_game(level);

    image=SDL_LoadBMP(path.c_str());
    texture=SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    ///*
    SDL_Color color={0,255,0,255};
    SDL_Rect pos={0,0,100,200};
    string t="DONE";
    write(t.c_str(),color,&pos);
    SDL_RenderPresent(renderer);
    //*/

    waitUntilKeyPress();
    quitSDL();
    return 0;
}
