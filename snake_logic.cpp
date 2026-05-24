#include <conio.h>
#include <chrono>
//#include <snake_logic.h>
using namespace std;
using namespace std::chrono;

const int SIZE = 21;

extern int map[SIZE][SIZE]; //map 소스파일에서 가져오기
int gate_arr[5][2][2]{ {{5,5},{7,7}} };
 

class snake_logic {
    public:
        steady_clock::time_point tick = steady_clock::now();
        int body[362][2]{ {6,6},{7,6},{8,6} }; //초기 위치 설정 필요 -> 일단 임의로 설정(6,6) (x,y)로 설정
        int speed{ 100 }; //움직이는 간격 100ms단위
        int length{ 3 }; //뱀 길이
        int max_length{ 361 }; // 스테이지마다 설정 필요
        char dir{ 'L' }; // 방향
        
        
        void to_loc(int& x1,int& y1,int x2,int y2) {
            if (dir == 'R') { x1 = x2 + 1; y1 = y2; } //진행방향 앞의 물질 확인을 위한 변수 할당 
            else if (dir == 'L') { x1 = x2 - 1; y1 = y2; }
            else if (dir == 'U') { x1 = x2; y1 = y2 - 1; }
            else if (dir == 'D') { x1 = x2; y1 = y2 + 1; }

        }
        void move() { // 뱀 이동 함수
            if (duration_cast<milliseconds>(steady_clock::now() - tick).count() >= speed) { //이동로직 구현 -> (현재 틱- 이전 틱) >= 설정된 틱 간격인지 확인
                //이동로직 시작
                int front[2] {};
                to_loc(front[0], front[1], body[0][0], body[0][1]);
                

                if ((map[front[1]][front[0]] == 1) || (map[front[1]][front[0]] == 2) || (map[front[1]][front[0]] == 4)) { //앞이 벽이나 뱀의 몸통인 경우
                    //게임 종료 함수 호출
                }
                else { //앞이 벽이 아닐 때 
                    int eat {};
                    if (map[front[1]][front[0]] == 5) getGate(front[0],front[1]); // 앞이 게이트 일 때
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
        void getGate(int& x, int& y){ //게이트 이동 함수 
            int door[2] = {};
            int front[2];
            bool is_found = false;
            for(int i = 0; i < 5; i++){
                for (int j = 0; j < 2; j++) {
                    if ((x == gate_arr[i][j][0]) && (y == gate_arr[i][j][1])){
                        is_found = true;
                        door[0] = i;
                        if (j == 1) door[1] = 0;
                        else door[1] = 1;
                        break;
                    }

                }
                if (is_found) break;
            }
            for (int i = 0; i < 4; i++) {
                to_loc(front[0],front[1],gate_arr[door[0]][door[1]][0],gate_arr[door[0]][door[1]][1]);//진행방향 앞의 물질 확인
                if ((map[front[1]][front[0]] == 1)||( map[front[1]][front[0]] == 2)){
                    if (dir == 'R') {
                        switch (i) {
                            case 0:
                                dir = 'U';
                                break;
                            case 1:
                                dir = 'L';
                                break;
                            case 2:
                                dir = 'U';
                                break;

                        }
                    }
                    else if (dir == 'L'){
                        switch (i) {
                            case 0:
                                dir = 'D';
                                break;
                            case 1:
                                dir = 'R';
                                break;
                            case 2:
                                dir = 'D';
                                break;

                        }
                    }                            
                    else if (dir == 'U') {
                        switch (i) {
                        case 0:
                            dir = 'R';
                            break;
                        case 1:
                            dir = 'D';
                            break;
                        case 2:
                            dir = 'R';
                            break;

                        }
                    }
                    else if (dir == 'D') {
                        switch (i) {
                        case 0:
                            dir = 'L';
                            break;
                        case 1:
                            dir = 'U';
                            break;
                        case 2:
                            dir = 'L';
                            break;

                        }
                    }
                    
                }
                else if (map[front[1]][front[0]] == 5){
                    //게임 종료 함수 호출
                    break;
                }
                else{
                    x = front[0];
                    y = front[1];
                    break;
                }


            }

        }
        
};

//블럭 정리 1: 벽, 2: 무조건 벽, 3: 뱀 머리, 4: 뱀 몸통, 5: 게이트, 6: 사과, 7: 독사과, 8: 스피드사과, 9: 미끄러짐 벽
