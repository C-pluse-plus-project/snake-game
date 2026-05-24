#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

const int SIZE = 21;
extern int map[SIZE][SIZE];
inline const int gate_arr[5][2][2]{ {{5,5},{7,7}} };  //존재하는 게이트 쌍 저장

class snake_logic {
    public:
        steady_clock::time_point tick = steady_clock::now();
        int body[362][2]{ {6,6},{7,6},{8,6} }; //초기 위치 설정 필요 -> 일단 임의로 설정(6,6) (x,y)로 설정
        int speed{ 100 }; //움직이는 간격 100ms단위
        int length{ 3 }; //뱀 길이
        int max_length{ 361 }; // 스테이지마다 설정 필요
        char dir{ 'L' }; // 방향
        random_device rd;
        mt19937 gen{ rd() };
        uniform_int_distribution<int> slip_dist{ 0, 1 };

        void to_loc(int& x1,int& y1,int x2,int y2);
        void move();
        void turn();
        void getApple(int apple,int& eat);
        void getGate(int& x,int& y);
        void getSlip(int& x1,int& y1,int x2,int y2);





};

#endif