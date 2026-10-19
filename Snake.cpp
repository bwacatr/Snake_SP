#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>


using namespace std;

struct Player
{
    char nickname[50];
    int apples;
};


struct Node
{
    int x;
    int y;
    Node* next = nullptr;

    Node(int x, int y) : x(x), y(y), next(nullptr) {}
};

class Snake
{
public:
    Node* head = nullptr;
    int appleLocationX = -1;
    int appleLocationY = -1;
    int length = 4;
    int apples = 0;




    char direction = 'r';


    Snake()
    {

        head = nullptr;

    }

    bool SavePlayer(int apples)
    {
        cout << "\n\nВведите имя пользователя для таблицы рекордов\n" << endl;
        Player player;
        cin >> player.nickname;
        

        player.apples = apples;

        std::ofstream file("players.bin", ios::binary | ios::app);

        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&player), sizeof(player));

            file.close();
            return true;
        }
        else
        {
            
            return false;
        }
        
    }

    bool LoadLeaderboard()
    {
        std::ifstream file("players.bin", std::ios::binary);

        if (file.is_open())
        {
            
            system("cls");

            
            Player player;

            while (file.read(reinterpret_cast<char*>(&player), sizeof(player)))
            {
                std::cout << player.nickname << " --> " << player.apples << "\n";
            }

            

            file.close();
            return true;
        }
        else
        {
            std::cout << "Файл таблицы рекордов отсутствует\n";
            
            return false;
        }
    }

    void clearSnake()
    {
        while (head != nullptr)
        {
            Node* chainLink = head; // по цепочке к концу
            head = head->next;
            delete chainLink;
        }
    }

    Node* push_back(int x, int y)
    {
        Node* ptr = new Node(x, y);

        if (head == nullptr)
        {
            head = ptr;
            return ptr;
        }
        else
        {
            Node* chainLink = head;

            while (chainLink->next != nullptr) // идем по цепочке в самый конец змеи
            {
                chainLink = chainLink->next;
            }
            chainLink->next = ptr;

            return ptr;
        }




    }

    void CreateSnake(char field[20][20])
    {
        int y = 10;
        for (int i = 0; i < this->length; i++)
        {
            int x = 10;

            Node node = Node(x, y);

            field[y][x] = '@';
            push_back(x, y);
            y++;
        }
    }

    void ShowField(char field[20][20])
    {
        system("cls");
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                std::cout << field[i][j];
            }
            std::cout << std::endl;


        }
        std::cout << direction << endl;
    }

    void CreateApple(char field[20][20])
    {
        int x = 1 + rand() % 18;
        int y = 1 + rand() % 18;

        while (field[y][x] != ' ') // if the coordinates point at a wall, they will be reassigned at random
        {
            x = 1 + rand() % 18;
            y = 1 + rand() % 18;
        }

        appleLocationX = x;
        appleLocationY = y;

        field[y][x] = 'A';
    }

    bool Save()
    {
        std::ofstream file("save.bin", std::ios::binary);

        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&apples), sizeof(apples));
            file.write(reinterpret_cast<const char*>(&direction), sizeof(direction));
            file.write(reinterpret_cast<const char*>(&appleLocationX), sizeof(appleLocationX));
            file.write(reinterpret_cast<const char*>(&appleLocationY), sizeof(appleLocationY));

            int count = 0;
            for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
            {
                count++;
            }

            file.write(reinterpret_cast<const char*>(&count), sizeof(count));

            for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
            {
                file.write(reinterpret_cast<const char*>(&ptr->x), sizeof(ptr->x));
                file.write(reinterpret_cast<const char*>(&ptr->y), sizeof(ptr->y));
            }

            file.close();
        }
        else
        {
            file.close();
            return false;
        }

        
    }

    bool Load()
    {
        std::ifstream file("save.bin", std::ios::binary);

        if (file.is_open())
        {
            clearSnake();

            file.read(reinterpret_cast<char*>(&apples), sizeof(apples));
            file.read(reinterpret_cast<char*>(&direction), sizeof(direction));
            file.read(reinterpret_cast<char*>(&appleLocationX), sizeof(appleLocationX));
            file.read(reinterpret_cast<char*>(&appleLocationY), sizeof(appleLocationY));

            int count = 0;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));

            for (int i = 0; i < count; i++)
            {
                int x;
                int y;
                file.read(reinterpret_cast<char*>(&x), sizeof(x));
                file.read(reinterpret_cast<char*>(&y), sizeof(y));



                push_back(x, y);
            }
            
            file.close();
            return true;
        }
        else
        {
            file.close();
            return false;
        }

        
    }

    void Game(char field[20][20], bool SaveFileExists)
    {
        for (int i = 0; i < 20; i++) // расстановка пустого места
        {
            for (int j = 0; j < 20; j++)
            {
                field[i][j] = ' ';
            }
        }

        for (int i = 0; i < 20; i++) // расстановка стенок
        {
            field[i][0] = '#';
            field[i][19] = '#';
        }

        for (int j = 0; j < 20; j++)
        {
            field[0][j] = '#';
            field[19][j] = '#';
        }

        if (SaveFileExists == false)
        {
            clearSnake();

            apples = 0;         //сброс всех данных
            appleLocationX = -1;
            appleLocationY = -1;
            direction = 'u';

            CreateSnake(field);
            CreateApple(field);
        }
        else
        {
            field[appleLocationY][appleLocationX] = 'A';
        }

        ShowField(field);

        bool gameEnd = false;
        int count = 0;
        while (gameEnd == false) //movement logic
        {


            if (_kbhit())
            {
                char directionChar = _getch(); // r - right, l - left, u - up, d - down // e - save game
                if (directionChar == 'd' && direction != 'l')
                {
                    direction = 'r';
                }
                else if (directionChar == 'a' && direction != 'r')
                {
                    direction = 'l';
                }
                else if (directionChar == 'w' && direction != 'd')
                {
                    direction = 'u';
                }
                else if (directionChar == 's' && direction != 'u')
                {
                    direction = 'd';
                }
                else if (directionChar == 'e')
                {
                    Save();
                }
                else
                {
                    if (direction == 'd')
                    {
                        direction = 'd';
                    }
                    else
                    {
                        direction = 'u';
                    }
                    
                }
            }

            bool appleTaken = false;


            count++;
            if (count > 3) //при превышении пяти повторений змейка начинает движение
            {

                count = 0;

                if (direction == 'r')
                {
                    Node* node = head;

                    int oldX = node->x;
                    int oldY = node->y;

                    int oldTailX;
                    int oldTailY;

                    Node* oldTail = head;
                    while (oldTail->next != nullptr)
                    {
                        oldTail = oldTail->next;
                    }

                    oldTailX = oldTail->x;
                    oldTailY = oldTail->y;

                    head->x = head->x + 1;

                    if (field[head->y][head->x] == ' ')
                    {
                        field[head->y][head->x] = '@';
                    }
                    else if (field[head->y][head->x] == 'A')
                    {
                        appleTaken = true;
                        apples++;
                        appleLocationX = -1;
                        appleLocationY = -1;
                        length++;


                    }
                    else
                    {
                        gameEnd = true;
                    }



                    while (node != nullptr)
                    {

                        node = node->next;


                        if (node != nullptr)
                        {
                            int tempX = node->x;
                            int tempY = node->y;

                            node->x = oldX;
                            node->y = oldY;

                            oldX = tempX;
                            oldY = tempY;

                            field[node->y][node->x] = '@';

                        }
                        else if (appleTaken)
                        {
                            push_back(oldTailX, oldTailY);
                            field[oldTailY][oldTailX] = '@';
                        }
                        else
                        {
                            field[oldTailY][oldTailX] = ' ';
                        }



                    }
                }
                else if (direction == 'l')
                {
                    Node* node = head;

                    int oldX = node->x;
                    int oldY = node->y;

                    int oldTailX;
                    int oldTailY;

                    Node* oldTail = head;
                    while (oldTail->next != nullptr)
                    {
                        oldTail = oldTail->next;
                    }

                    oldTailX = oldTail->x;
                    oldTailY = oldTail->y;

                    head->x = head->x - 1;

                    if (field[head->y][head->x] == ' ')
                    {
                        field[head->y][head->x] = '@';
                    }
                    else if (field[head->y][head->x] == 'A')
                    {
                        appleTaken = true;
                        apples++;
                        appleLocationX = -1;
                        appleLocationY = -1;
                        length++;


                    }
                    else
                    {
                        gameEnd = true;
                    }



                    while (node != nullptr)
                    {

                        node = node->next;


                        if (node != nullptr)
                        {
                            int tempX = node->x;
                            int tempY = node->y;

                            node->x = oldX;
                            node->y = oldY;

                            oldX = tempX;
                            oldY = tempY;

                            field[node->y][node->x] = '@';

                        }
                        else if (appleTaken)
                        {
                            push_back(oldTailX, oldTailY);
                            field[oldTailY][oldTailX] = '@';
                        }
                        else
                        {
                            field[oldTailY][oldTailX] = ' ';
                        }



                    }
                }
                else if (direction == 'u')
                {

                    Node* node = head;

                    int oldX = node->x;
                    int oldY = node->y;

                    int oldTailX;
                    int oldTailY;

                    Node* oldTail = head;
                    while (oldTail->next != nullptr)
                    {
                        oldTail = oldTail->next;
                    }

                    oldTailX = oldTail->x;
                    oldTailY = oldTail->y;

                    head->y = head->y - 1;

                    if (field[head->y][head->x] == ' ')
                    {
                        field[head->y][head->x] = '@';
                    }
                    else if (field[head->y][head->x] == 'A')
                    {
                        appleTaken = true;
                        apples++;
                        appleLocationX = -1;
                        appleLocationY = -1;
                        length++;


                    }
                    else
                    {
                        gameEnd = true;
                    }



                    while (node != nullptr)
                    {

                        node = node->next;


                        if (node != nullptr)
                        {
                            int tempX = node->x;
                            int tempY = node->y;

                            node->x = oldX;
                            node->y = oldY;

                            oldX = tempX;
                            oldY = tempY;

                            field[node->y][node->x] = '@';

                        }
                        else if (appleTaken)
                        {
                            push_back(oldTailX, oldTailY);
                            field[oldTailY][oldTailX] = '@';
                        }
                        else
                        {
                            field[oldTailY][oldTailX] = ' ';
                        }



                    }
                }
                else if (direction == 'd')
                {
                    Node* node = head;

                    int oldX = node->x;
                    int oldY = node->y;

                    int oldTailX;
                    int oldTailY;

                    Node* oldTail = head;
                    while (oldTail->next != nullptr)
                    {
                        oldTail = oldTail->next;
                    }

                    oldTailX = oldTail->x;
                    oldTailY = oldTail->y;

                    head->y = head->y + 1;

                    if (field[head->y][head->x] == ' ')
                    {
                        field[head->y][head->x] = '@';
                    }
                    else if (field[head->y][head->x] == 'A')
                    {
                        appleTaken = true;
                        apples++;
                        appleLocationX = -1;
                        appleLocationY = -1;
                        length++;


                    }
                    else
                    {
                        gameEnd = true;
                        
                        
                    }



                    while (node != nullptr)
                    {

                        node = node->next;


                        if (node != nullptr)
                        {
                            int tempX = node->x;
                            int tempY = node->y;

                            node->x = oldX;
                            node->y = oldY;

                            oldX = tempX;
                            oldY = tempY;

                            field[node->y][node->x] = '@';

                        }
                        else if (appleTaken)
                        {
                            push_back(oldTailX, oldTailY);
                            field[oldTailY][oldTailX] = '@';
                        }
                        else
                        {
                            field[oldTailY][oldTailX] = ' ';
                        }



                    }
                }



                if (appleTaken)
                {
                    CreateApple(field);
                }

                ShowField(field);

            }

            Sleep(100);

        }
        SavePlayer(apples);

        
        




    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    Snake snake;

    char field[20][20];

    bool skip = false;

    while (skip == false)
    {
        std::cout << "\n1. Новая игра\n"
            << "2. Загрузить игру\n"
            << "3. Таблица рекордов\n"
            << "4. Выход\n" << endl;

        std::cout << "Выберите пункт меню: ";
        int choice;
        std::cin >> choice;
        bool load;
        switch (choice)
        {
        case 1:
            snake.Game(field, false);
            break;
        case 2:
            load = snake.Load();
            if (load)
            {
                snake.Game(field, true);
            }
            break;
        case 3:
            snake.LoadLeaderboard();
            break;
        
        default:
            skip = true;
            break;
        }

    }
}