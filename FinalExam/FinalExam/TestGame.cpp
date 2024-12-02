#include <stdio.h>
#include <conio.h> // Windows 환경에서 getch 사용을 위한 헤더파일
#include <stdlib.h>
#include <time.h>

void GameLoop() {
    Initialize();

    clock_t start_time = clock(); // 현재 시간을 기록
    while (1) {
        Update(); // Update는 계속 호출하여 키 입력을 처리
        // 현재 시간과 시작 시간의 차이를 확인하여 1초마다 렌더링
        if ((clock() - start_time) >= CLOCKS_PER_SEC) { // CLOCKS_PER_SEC는 1초
            Render();
            start_time = clock(); // 시간을 초기화하여 다음 1초를 기다림
        }
    }
}

int main() {
    GameLoop();
    return 0;
}