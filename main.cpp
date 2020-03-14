#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sys/select.h>
#include <queue>
#include <cstdio>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <list>


using namespace std;
bool g_game_over = false;
const int max_size = 101;
const int max_m_num = 10;
int line,col;
char g_map[max_size][max_size];
const char obstacle = 'x';
const char monster = 'm';
const char person = 'p';
struct pos{
    int x;
    int y;
};


pos player;
list<pos *> monst;


void init_map(string &file_name){  //初始化map
    ifstream in(file_name.c_str());
    char ch;

    if(!in.is_open()) {
        cout << "error map" <<endl;
        exit(0);
    }

    string str;
    getline(in,str);
    stringstream ss(str);
    ss >> line >> ch >> col;
    getline(in,str);
    stringstream obs(str);

    int x,y;
    while(obs >> x >> ch >> y >> ch){
        g_map[x][y] = obstacle;
    }

}

void init_person(){
    srand((int)time(0));

    do{
        player.x = rand() % line;
        player.y = rand() % col;
    }while(g_map[player.x][player.y] == obstacle);
    cout << "you are in the location of (" << player.x << "," << player.y << "),let's start of it!\n";
}

bool can_move_or_not(int x,int y,bool user){
    if(x < 0 || x >=line || y <0 || y > col){
        if(user)cout << "you current in edge of map,can't move successfully!\n";
        return false;
    }

    if(g_map[x][y] == obstacle){
        if(user)cout << "can't move successfully,because obstacle are there!\n";
        return false;
    }

    if(g_map[x][y] == monster){
        if(user)cout << "you are eat by monster,gameover!\n";
        return false;
    }
    return true;
}

void move_to(int x,int y){
    player.x = x;
    player.y = y;
}



inline void display()
{
    for(int i=0;i<col;++i) cout <<'-';cout <<endl;
    for(int i=0;i<line;++i){
        for(int j=0;j<col;++j){
            if(i == player.x && j == player.y) cout << person;
            else if(g_map[i][j] == char(0))cout << ' ';
            else cout << g_map[i][j];
        }
        cout <<endl;
    }
    for(int i=0;i<col;++i) cout <<'-';cout <<endl;
}


void init_monster(int num){
    srand((int)time(0));

    while(num--) {
        pos * m = new pos;
        do {
            m->x = rand() % line;
            m->y = rand() % col;
        }while(g_map[m->x][m->y] == obstacle || g_map[m->x][m->y] == monster);
        g_map[m->x][m->y] = monster;
        monst.push_back(m);
    }
}



bool monster_move(){

    for(auto iter = monst.begin();iter != monst.end();++iter){
        pos p;
        p.x = (*iter)->x;
        p.y = (*iter)->y;
        bool move_res = false;
        //srand(0);
        int direction = 0;
        switch(direction){
            case 0:
                move_res=false;
                move_res = can_move_or_not(p.x-1,p.y,false);
                if(move_res) {
                    g_map[p.x][p.y] = char(0);
                    g_map[p.x - 1][p.y] = monster;
                    (*iter)->x = p.x - 1;
                    break;
                }
            case 1:
                move_res=false;
                move_res = can_move_or_not(p.x+1,p.y,false);
                if(move_res){
                    g_map[p.x][p.y] = char(0);
                    g_map[p.x+1][p.y] = monster;
                    (*iter)->x = p.x+1;
                    break;
                }
            case 2:
                move_res = can_move_or_not(p.x,p.y-1,false);
                if(move_res){
                    g_map[p.x][p.y] = char(0);
                    g_map[p.x][p.y-1] = monster;
                    (*iter)->y = p.y-1;
                    break;
                }
            case 3:
                move_res = can_move_or_not(p.x,p.y+1,false);
                if(move_res){
                    g_map[p.x][p.y] = char(0);
                    g_map[p.x][p.y+1] = monster;
                    (*iter)->y = p.y+1;
                }
                break;

        }
        direction = (direction + rand()) % 4;
        if((*iter)->x == player.x && (*iter)->y == player.y){
            cout << "you are eat by monster,gameover!\n";
            return false;
        }
    }
    return true;
}

void monster_in_map(){
    while(true){
        if(monster_move());
        else {
            g_game_over = true;
            return;
        }
        struct timespec ts, ts1;

        ts.tv_nsec = 0;    // 1500ms
        ts.tv_sec = 1;
        nanosleep(&ts,&ts1);
        //display();
    }
}


int main()
{
    string map_name = "./map";
    init_map(map_name);
    init_person();
    int monster_num = rand() % max_m_num;
    init_monster(monster_num);
    thread monster_p(monster_in_map);
    monster_p.detach();

    while (!g_game_over) {
        char direction = getchar();
        getchar();
        bool res = false;
        switch (direction) {
            //向上
            case 'W':
                res = can_move_or_not(player.x - 1, player.y,true);
                if (res)move_to(player.x - 1, player.y);
                break;
                //向下
            case 'S':
                res = can_move_or_not(player.x + 1, player.y,true);
                if (res) move_to(player.x + 1, player.y);
                break;
                //向左
            case 'A':
                res = can_move_or_not(player.x, player.y - 1,true);
                if (res) move_to(player.x, player.y - 1);
                break;
                //向右
            case 'D':
                res = can_move_or_not(player.x, player.y + 1,true);
                if (res) move_to(player.x, player.y + 1);
                break;
            case 'J':
                display();
                break;
            case 'K':
                cout << "your location:(" << player.x << "," << player.y << ")";
                break;
            default:
                cout << "you are not moved! stay in old location\n";
                break;

        }
    }
    return 0;
}