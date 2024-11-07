#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <string>

const int WIDTH = 40;    // 터미널 출력 너비
const int HEIGHT = 20;   // 터미널 출력 높이
const double PI = 3.14159265359;

// 2D 벡터 구조체
struct Vector2 {
    double x, y;
    Vector2(double x = 0, double y = 0) : x(x), y(y) {}
    
    Vector2 rotate(double angle) {
        double rad = angle * PI / 180.0;
        return Vector2(
            x * cos(rad) - y * sin(rad),
            x * sin(rad) + y * cos(rad)
        );
    }
};

// 화면 지우기 함수
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void drawRectangle() {
    static double angle = 0.0;
    std::vector<std::vector<char>> screen(HEIGHT, std::vector<char>(WIDTH, ' '));
    
    // 사각형의 네 꼭지점 정의 (5x11 비율 유지)
    Vector2 points[4] = {
        Vector2(-2.5, -5.5),  // 좌하단
        Vector2(2.5, -5.5),   // 우하단
        Vector2(2.5, 5.5),    // 우상단
        Vector2(-2.5, 5.5)    // 좌상단
    };
    
    // 각 꼭지점을 회전
    for (int i = 0; i < 4; i++) {
        points[i] = points[i].rotate(angle);
    }
    
    // 화면 중앙으로 이동 및 스케일 조정
    for (int i = 0; i < 4; i++) {
        points[i].x = points[i].x * 2 + WIDTH/2;
        points[i].y = points[i].y + HEIGHT/2;
    }
    
    // 사각형 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            bool inside = false;
            for (int i = 0, j = 3; i < 4; j = i++) {
                if (((points[i].y > y) != (points[j].y > y)) &&
                    (x < (points[j].x - points[i].x) * (y - points[i].y) /
                    (points[j].y - points[i].y) + points[i].x)) {
                    inside = !inside;
                }
            }
            screen[y][x] = inside ? '*' : ' ';
        }
    }
    
    // 화면 출력
    clearScreen();
    for (const auto& row : screen) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }
    
    // 각도 업데이트 (3초에 한 바퀴)
    angle += 3.0;
    if (angle >= 360.0) angle -= 360.0;
}

int main() {
    while (true) {
        drawRectangle();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 약 20fps
    }
    return 0;
} 