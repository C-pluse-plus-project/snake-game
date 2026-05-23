#include <conio.h>
#include <chrono>
//#include <iostream>
using namespace std;
using namespace std::chrono;

const int SIZE = 21;

extern int map[SIZE][SIZE]; //map 소스파일에서 가져오기
 

class snake_logic {
    public:
        steady_clock::time_point tick = steady_clock::now();
        int body[362][2]{ {6,6},{7,6},{8,6} }; //초기 위치 설정 필요 -> 일단 임의로 설정(6,6) (x,y)로 설정
        int speed{ 100 }; //움직이는 간격 100ms단위
        int length{ 3 }; //뱀 길이
        int max_length{ 361 }; // 스테이지마다 설정 필요
        char dir{ 'L' }; // 방향
        
        
        void move() { // 뱀 이동 함수
            if (duration_cast<milliseconds>(steady_clock::now() - tick).count() >= speed) { //이동로직 구현 -> (현재 틱- 이전 틱) >= 설정된 틱 간격인지 확인
                //이동로직 시작
                int front[2] {};
                if (dir == 'R') { front[0] = body[0][0]+1; front[1] = body[0][1]; } //진행방향 앞의 물질 확인을 위한 변수 할당 
                else if (dir == 'L') { front[0] = body[0][0]-1; front[1] = body[0][1]; }
                else if (dir == 'U') { front[0] = body[0][0]; front[1] = body[0][1]-1; }
                else if (dir == 'D') { front[0] = body[0][0]; front[1] = body[0][1]+1; }

                if ((map[front[1]][front[0]] == '1') || (map[front[1]][front[0]] == '2') || (map[front[1]][front[0]] == '4')) { //앞이 벽이나 뱀의 몸통인 경우
                    //게임 종료 함수 호출
                }
                else if ((map[front[1]][front[0]] == '5')) { //앞이 gate인 경우 / 일단 게이트가 하나만 존재한다고 상정 
                    gate(front[1],front[0]); //아직 미구현
                }
                else { //앞이 벽이 아닐 때 
                    int eat {};
                    getApple(map[front[1]][front[0]],eat); //앞에 사과가 있는지를 확인
                    
                    for(int i = length; i > 0; i--){ //뱀의 이동 구현
                        body[i][0] = body[i-1][0];
                        body[i][1] = body[i-1][1];
                    }
                    body[0][0] = front[0];
                    body[0][1] = front[1];
                    for(int i = 0; i <= length; i++){
                        if((body[i][1] != 0)&&(body[i][0] != 0))
                            if (i == 0) map[body[i][1]][body[i][0]] = 3;
                            else if (i == length&&(!eat)) map[body[i][1]][body[i][0]] = 0;
                            else map[body[i][1]][body[i][0]] = 4;
                    }
                }

                tick = steady_clock::now(); // 틱을 현재 틱으로 초기화
            }
        }
        void turn() { // 뱀 방향 전환 함수
            if (_kbhit()) { // 키보드 입력이 존재할 때 키보드 입력 저장
                char temp = _getch();
                if ((temp == 'R' && dir == 'L') || (temp == 'L' && dir == 'R') || (temp == 'U' && dir == 'D') || (temp == 'D' && dir == 'U')) { // 진행방향과 정반대로 입력한 경우
                    //게임 종료 함수 호출
                    return;
                }
                dir = temp; // 입력된 방향 반영
            }
        }

        void getApple(int apple, int& eat) { // 사과 반영 함수
            if (apple == 6) { length++; eat = 1; } // 앞에 사과가 있는 경우
            else if (apple == 7) { // 앞에 독사과가 있는 경우
                body[length - 1][0] = { 0 };
                body[length - 1][1] = { 0 };
                length--;
            }
            if (length < 3) {/*게임 종료 함수 호출*/ } //최소 길이보다 작아진 경우
            else if (length >= max_length) { // 최대 길이에 도달한 경우
            //게임 클리어
            //게임 종료 함수 호출
            }
        }
        void gate(int y, int x){ //게이트 이동 함수 / 아직 미구현
            int UDLR[4]{};



        }
        
};

//블럭 정리 1: 벽, 2: 무조건 벽, 3: 뱀 머리, 4: 뱀 몸통, 5: 게이트, 6: 사과, 7: 독사과
