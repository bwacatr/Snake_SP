// Snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

class SnakeField
{
    char field[10][10];

    Snake snake = Snake();

    void GameMenu()
    {
        cout << "\n\n 1. Новая игра \n2. Загрузить игру\n3. Таблица рекордов\n4.Выход" << endl;
        int menuChoice;
        cin >> menuChoice;

        switch (menuChoice)
        {
        case 1:
            Game();
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        default:
            system("cls");
            break;
        }
    }

    void Game()
    {
        system("cls");
        
        bool leave = false
            ;
        while (leave == false)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {


                    if (snake.head->fieldCoords[i][j] == 1)
                    {
                        cout << "@";

                        if (snake.direction == 'r') // если змея направлена вправо
                        {
                            snake.head->fieldCoords[i][j] == 0;

                            if (j != 9 || j != 0)
                            {
                                snake.head->fieldCoords[i][j + 1] == 1;
                            }
                            else
                            {
                                snake.death();
                            }

                        }
                        else if (snake.direction == 'l') // если змея направлена влево
                        {
                            snake.head->fieldCoords[i][j] == 0;

                            if (j != 9 || j != 0)
                            {
                                snake.head->fieldCoords[i][j - 1] == 1;
                            }
                            else
                            {
                                snake.death();
                            }

                        }
                        else if (snake.direction == 'u') // если змея направлена наверх
                        {
                            snake.head->fieldCoords[i][j] == 0;

                            if (i != 9 || i != 0)
                            {
                                snake.head->fieldCoords[i + 1][j] == 1;
                            }
                            else
                            {
                                snake.death();
                            }

                        }
                        else if (snake.direction == 'd') // если змея направлена вниз
                        {
                            snake.head->fieldCoords[i][j] == 0;

                            if (i != 9 || i != 0)
                            {
                                snake.head->fieldCoords[i + 1][j] == 1;
                            }
                            else
                            {
                                snake.death();
                            }

                        }
                    }
                    else
                    {
                        if (i == 9 || j == 9)
                        {
                            cout << "#";
                        }
                    }
                }
            }

            if (_kbhit)
            {
                char directionChar = _getch();
                if (directionChar == 'r')
                {
                    snake.direction = 'r';
                }
                else if (directionChar == 'l')
                {
                    snake.direction = 'l';
                }
                else if (directionChar == 'u')
                {
                    snake.direction = 'u';
                }
                else if (directionChar == 'd')
                {
                    snake.direction = 'd';
                }
                else if (directionChar == 'm')
                {
                    snake.death();
                }
            }

            Sleep(1000);

        }
    }


};

struct Node
{
    int fieldCoords[10][10];
    Node* next = nullptr;

    Node()
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                fieldCoords[i][j] = 0;
            }
        }
        fieldCoords[4][4] = 1;
    }
};

class Snake
{
public:
    Node* head = nullptr; //Начало змеи
    Node* tail = nullptr; //Конец змеи
    char direction = 'r';


    Snake() 
    {

        head = new Node();
        tail = new Node();
    }

    void death()
    {
        if (head != nullptr)
        {
            cout << "\n \n змея проиграла. \n" << endl;

            Node* node = head;
            Node* temp = head;
            while (node != nullptr)
            {
                temp = node;
                if (node->next != nullptr)
                {
                    node = node->next;
                }


                delete temp;

            }
            exit(0);
        }
        else
        {
            cout << "\n \n змея пуста. \n" << endl;
        }
    }

    Node* push_front() //добавление в начало змеи
    {
        Node* ptr = new Node(); //выделение памяти под структуру Node

        Node* temp = nullptr;

        if (head == nullptr)
        {
            head = ptr;
        }
        else
        {
            temp = head;
            ptr->next = temp;
            head = ptr;
        }

        if (tail == nullptr)
        {
            tail = ptr;
        }


        return ptr;

    }

    

    
};

int main()
{
    std::cout << "Hello World!\n";
}


