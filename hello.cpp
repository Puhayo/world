#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define PI 3.14159265359

// kbhit() 구현
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// getch() 구현
char getch(void) {
    char ch;
    struct termios old_settings, new_settings;

    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    return ch;
}

// 화면 지우기 함수
void clearScreen() {
    system("clear");
}

// 3x3 행렬 구조체
struct Matrix3x3 {
    float m[3][3];
    
    Matrix3x3() {
        // 단위행렬로 초기화
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
};

// 점 구조체
struct Point {
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

// 행렬 곱셈
Matrix3x3 MultiplyMatrix(const Matrix3x3& a, const Matrix3x3& b) {
    Matrix3x3 result;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            result.m[i][j] = 0;
            for(int k = 0; k < 3; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

// 회전 행렬 생성
Matrix3x3 CreateRotationMatrix(float angle) {
    Matrix3x3 rot;
    rot.m[0][0] = cos(angle);
    rot.m[0][1] = -sin(angle);
    rot.m[1][0] = sin(angle);
    rot.m[1][1] = cos(angle);
    return rot;
}

// 이동 행렬 생성
Matrix3x3 CreateTranslationMatrix(float x, float y) {
    Matrix3x3 trans;
    trans.m[0][2] = x;
    trans.m[1][2] = y;
    return trans;
}

// 스케일 행렬 생성
Matrix3x3 CreateScaleMatrix(float sx, float sy) {
    Matrix3x3 scale;
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;
    return scale;
}

// 점과 행렬의 곱셈
Point TransformPoint(const Point& p, const Matrix3x3& m) {
    Point result;
    result.x = p.x * m.m[0][0] + p.y * m.m[0][1] + m.m[0][2];
    result.y = p.x * m.m[1][0] + p.y * m.m[1][1] + m.m[1][2];
    return result;
}

class ScreenBuffer {
    char buffer[24][160];
public:
    void Clear() {
        memset(buffer, ' ', sizeof(buffer));
    }
    
    void SetPixel(int x, int y, const char* ch) {
        if(x >= 0 && x < 80 && y >= 0 && y < 24) {
            buffer[y][x] = ch[0];
            if (ch[1]) buffer[y][x+1] = ch[1];
        }
    }
    
    void Draw() {
        clearScreen();
        for(int y = 0; y < 24; y++) {
            for(int x = 0; x < 160; x++) {
                std::cout << buffer[y][x];
            }
            std::cout << std::endl;
        }
    }
};

// 행성 그리기 함수들
void DrawSun(ScreenBuffer& buffer, const Matrix3x3& transform) {
    // 삼각형 그리기
    Point vertices[3] = {
        Point(0, -5),  // 위
        Point(-4.33f, 2.5f),  // 좌하단
        Point(4.33f, 2.5f)   // 우하단
    };
    
    for(int i = 0; i < 3; i++) {
        Point p1 = TransformPoint(vertices[i], transform);
        Point p2 = TransformPoint(vertices[(i+1)%3], transform);
        
        int x1 = (int)p1.x, y1 = (int)p1.y;
        int x2 = (int)p2.x, y2 = (int)p2.y;
        
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        while(true) {
            buffer.SetPixel(x1, y1, "SS");
            if(x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if(e2 > -dy) { err -= dy; x1 += sx; }
            if(e2 < dx) { err += dx; y1 += sy; }
        }
    }
}

void DrawEarth(ScreenBuffer& buffer, const Matrix3x3& transform) {
    Point vertices[4] = {
        Point(-3, -3),
        Point(3, -3),
        Point(3, 3),
        Point(-3, 3)
    };
    
    for(int i = 0; i < 4; i++) {
        Point p1 = TransformPoint(vertices[i], transform);
        Point p2 = TransformPoint(vertices[(i+1)%4], transform);
        
        int x1 = (int)p1.x, y1 = (int)p1.y;
        int x2 = (int)p2.x, y2 = (int)p2.y;
        
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        while(true) {
            buffer.SetPixel(x1, y1, "EE");
            if(x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if(e2 > -dy) { err -= dy; x1 += sx; }
            if(e2 < dx) { err += dx; y1 += sy; }
        }
    }
}

void DrawMoon(ScreenBuffer& buffer, const Matrix3x3& transform) {
    for(float angle = 0; angle < 2*PI; angle += 0.1f) {
        Point p(1.5f * cos(angle), 1.5f * sin(angle));
        p = TransformPoint(p, transform);
        buffer.SetPixel((int)p.x, (int)p.y, "MM");
    }
}

int main() {
    ScreenBuffer buffer;
    bool isRunning = true;
    int state = 0;  // 0: 대기, 1: 정렬, 2: 움직임
    
    auto startTime = std::chrono::system_clock::now();
    
    while(isRunning) {
        buffer.Clear();
        
        if(state == 0) {
            // 학번과 이름 출력
            const char* info = "20190629 조경민";
            int pos = 30;
            for(size_t i = 0; i < strlen(info); ) {
                if((info[i] & 0x80) == 0) {
                    char temp[2] = {info[i], '\0'};
                    buffer.SetPixel(pos++, 12, temp);
                    i++;
                } else {
                    char temp[3] = {info[i], info[i+1], '\0'};
                    buffer.SetPixel(pos, 12, temp);
                    pos += 2;
                    i += 2;
                }
            }
        }
        else {
            auto currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            
            // 태양 변환 행렬
            Matrix3x3 sunRotation = CreateRotationMatrix(-2*PI*elapsedSeconds/3.0f);
            Matrix3x3 sunTranslation = CreateTranslationMatrix(20, 12);
            Matrix3x3 sunTransform = MultiplyMatrix(sunTranslation, sunRotation);
            
            // 지구 변환 행렬
            float earthOrbitAngle = 2*PI*elapsedSeconds/3.0f;
            float earthRotationAngle = 2*PI*elapsedSeconds/2.0f;
            Matrix3x3 earthRotation = CreateRotationMatrix(earthRotationAngle);
            Matrix3x3 earthOrbitTranslation = CreateTranslationMatrix(12*cos(earthOrbitAngle), 12*sin(earthOrbitAngle));
            Matrix3x3 earthTransform = MultiplyMatrix(sunTranslation, 
                                     MultiplyMatrix(earthOrbitTranslation, earthRotation));
            
            // 달 변환 행렬
            float moonOrbitAngle = -2*PI*elapsedSeconds/2.0f;
            float moonRotationAngle = 2*PI*elapsedSeconds;
            Matrix3x3 moonRotation = CreateRotationMatrix(moonRotationAngle);
            Matrix3x3 moonOrbitTranslation = CreateTranslationMatrix(4*cos(moonOrbitAngle), 4*sin(moonOrbitAngle));
            Matrix3x3 moonTransform = MultiplyMatrix(earthTransform, 
                                    MultiplyMatrix(moonOrbitTranslation, moonRotation));
            
            if(state == 1) {
                // 일렬 배치
                sunTransform = CreateTranslationMatrix(20, 12);
                earthTransform = CreateTranslationMatrix(32, 12);
                moonTransform = CreateTranslationMatrix(36, 12);
            }
            
            DrawSun(buffer, sunTransform);
            DrawEarth(buffer, earthTransform);
            DrawMoon(buffer, moonTransform);
        }
        
        buffer.Draw();
        
        if(kbhit()) {
            char key = getch();
            if(key == 27) {  // ESC
                isRunning = false;
            }
            else if(key == ' ') {
                state = (state + 1) % 3;
                if(state == 2) {
                    startTime = std::chrono::system_clock::now();
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return 0;
}