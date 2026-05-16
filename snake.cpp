#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Размеры поля
const int WIDTH = 20;
const int HEIGHT = 15;

// Структура для координат
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false;
    cursor.dwSize = 100;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

    srand(time(0));

    bool gameOver = false;
    bool restart = true;

    while (restart) {

        vector<Point> snake;
        snake.push_back(Point(WIDTH/2, HEIGHT/2));     // Голова
        snake.push_back(Point(WIDTH/2 - 1, HEIGHT/2)); // Тело
        snake.push_back(Point(WIDTH/2 - 2, HEIGHT/2)); // Хвост

        // Направление: 0-влево, 1-вправо, 2-вверх, 3-вниз
        int dir = 1; // начинаем вправо
        int score = 0;
        int speed = 150;

        // Генерация еды
        Point food;
        bool validPosition;
        do {
            validPosition = true;
            food.x = rand() % WIDTH;
            food.y = rand() % HEIGHT;
            for (const auto& segment : snake) {
                if (segment.x == food.x && segment.y == food.y) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        gameOver = false;


        while (!gameOver) {
            // ОТРИСОВКА
            system("cls");

            cout << "Счет: " << score << "   Скорость: " << 1000/speed << " кадров/сек\n";
            cout << "Управление: WASD  Выход: X\n\n";


            cout << "+";
            for (int i = 0; i < WIDTH; i++) cout << "-";
            cout << "+\n";

            // Игровое поле
            for (int y = 0; y < HEIGHT; y++) {
                cout << "|";
                for (int x = 0; x < WIDTH; x++) {
                    bool isSnake = false;

                    // Рисуем змейку
                    for (size_t i = 0; i < snake.size(); i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            isSnake = true;
                            if (i == 0) {
                                cout << "[]";  // Голова
                            } else {
                                cout << "¦";   // Тело
                            }
                            break;
                        }
                    }

                    // Рисуем еду или пустоту
                    if (!isSnake) {
                        if (food.x == x && food.y == y) {
                            cout << "0";  // Еда
                        } else {
                            cout << " ";  // Пустота
                        }
                    }
                }
                cout << "|\n";
            }

            // Нижняя граница
            cout << "+";
            for (int i = 0; i < WIDTH; i++) cout << "-";
            cout << "+\n";

            // УПРАВЛЕНИЕ
            if (_kbhit()) {
                char key = _getch();
                switch(key) {
                    case 'a': case 'A':
                        if (dir != 1) dir = 0; break; // Влево
                    case 'd': case 'D':
                        if (dir != 0) dir = 1; break; // Вправо
                    case 'w': case 'W':
                        if (dir != 3) dir = 2; break; // Вверх
                    case 's': case 'S':
                        if (dir != 2) dir = 3; break; // Вниз
                    case 'x': case 'X':
                        gameOver = true;
                        break;
                }
            }


            Point newHead = snake[0]; // Новая голова

            switch(dir) {
                case 0: newHead.x--; break; // Влево
                case 1: newHead.x++; break; // Вправо
                case 2: newHead.y--; break; // Вверх
                case 3: newHead.y++; break; // Вниз
            }

            // Проверка столкновения со стенами
            if (newHead.x < 0 || newHead.x >= WIDTH ||
                newHead.y < 0 || newHead.y >= HEIGHT) {
                gameOver = true;
                break;
            }

            // Проверка, съели ли еду
            bool ateFood = (newHead.x == food.x && newHead.y == food.y);

            // Добавляем новую голову
            snake.insert(snake.begin(), newHead);

            if (ateFood) {
                score += 10;
                // Увеличиваем скорость каждые 50 очков
                if (score % 50 == 0 && speed > 50) {
                    speed -= 10;
                }

                // Генерируем новую еду
                do {
                    validPosition = true;
                    food.x = rand() % WIDTH;
                    food.y = rand() % HEIGHT;
                    for (const auto& segment : snake) {
                        if (segment.x == food.x && segment.y == food.y) {
                            validPosition = false;
                            break;
                        }
                    }
                } while (!validPosition);
            } else {
                // Удаляем хвост
                snake.pop_back();

                // Проверка столкновения с собой
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                        gameOver = true;
                        break;
                    }
                }
            }

            // Задержка для управления скоростью
            Sleep(speed);
        }

        // КОНЕЦ ИГРЫ
        system("cls");
        cout << "\n\n";
        cout << "   *** ИГРА ОКОНЧЕНА! ***\n\n";
        cout << "   Ваш счет: " << score << "\n\n";
        cout << "   Нажмите R для рестарта\n";
        cout << "   Любую другую клавишу для выхода...\n\n";

        char choice = _getch();
        if (choice != 'r' && choice != 'R') {
            restart = false;
        }
    }

    cout << "Спасибо за игру!\n";
    return 0;
}
