#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <vector>

bool gameOver;
std::vector<int> scores;

class Screen;
class Fruit;
class Snake;

class GameObject{
    public:
    int m_x, m_y;
    virtual char Draw_Tile() const = 0;
};

class Screen
{
private:
    int m_height{};
    int m_width{};

public:
    Screen()
    {
        m_height = 18;
        m_width = 22;
    }
    ~Screen()
    {
    }
    friend void Draw( Fruit &, Snake &, Screen &);
    friend bool StartOver(Screen &, Snake &, int highest);
    friend class Snake;
    friend class Fruit;
    void SetupScreen(){
        system("cls");
        system("COLOR E2");
        gameOver = false;
    }
    void SetScreenSize(int a, int b){
        m_width = a;
        m_height = b;
    }

};

class Fruit : public GameObject
{
    public:
    Fruit()
    {
        m_x = 0;
        m_y = 0;
    }
    ~Fruit()
    {
    }
    char Draw_Tile() const override {return '@';}
    friend class Screen;
    friend class Snake;
    friend void Draw( Fruit &, Snake &, Screen &);
    void CreateFruit(const Screen &screen){
        srand(time(0));
        m_x = rand() % screen.m_width;
        m_y = rand() % screen.m_height;
    }
};

class Snake : public GameObject
{
private:
    enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
    int m_nTail, m_score;
    int m_tailX[100], m_tailY[100];
    eDirection m_dir;

public:
    Snake()
    {
        m_score = 0;
        m_nTail = 0;
        m_dir = STOP;
    }
    ~Snake()
    {
    }
    char Draw_Tile() const override {return 'O';}
    friend class Screen;
    friend void Draw( Fruit &, Snake &, Screen &);
    friend int HighestScore(Snake &, int i);
    friend bool StartOver(Screen &, Snake &, int highest);
    void SetupSnake(const Screen &screen){
        m_x = screen.m_width/2;
        m_y = screen.m_height/2;
    }
    void SnakeReset(const Screen &screen){
        m_nTail = 0;
        m_dir = STOP;
        m_score = 0;
        m_dir = STOP;
    }
    void SnakeCollide(const Screen &screen){
        if (m_x < 0 || m_x > screen.m_width || m_y < 0 || m_y > screen.m_height){
        gameOver = true;
        }
        for (int i = 0; i < m_nTail; i++){
            if(m_tailX[i] == m_x && m_tailY[i] == m_y){
                gameOver = true;
            }
        }
    }
    int GetScore(){
        return m_score;
    }
    bool SnakeEats(const Fruit &fruit){
        if(m_x == fruit.m_x && m_y == fruit.m_y){
            m_nTail ++;
            m_score += 10;
            return true;
        }
        else{
            return false;
        }
    }
    void InputSnake()
    {
        if (_kbhit())
    {
        switch(_getch())
        {
        case 'a':
            m_dir = LEFT;
            break;
        case 'd' :
            m_dir = RIGHT;
            break;
        case 'w' :
            m_dir = UP;
            break;
        case 's' :
            m_dir = DOWN;
            break;
        case 'x' :
            gameOver = true;
            break;
        }
    }
    }

    void MoveSnake(){
        for (int i = m_nTail; i > 0; i--){
        m_tailX[i] = m_tailX[i-1];
        m_tailY[i] = m_tailY[i-1];
        }
        m_tailX[0] = m_x;
        m_tailY[0] = m_y;
        switch (m_dir)
        {
        case LEFT:
            Sleep(10);
            m_x--;
            break;
        case RIGHT:
            Sleep(10);
            m_x++;
            break;
        case UP:
            Sleep(40);
            m_y--;
            break;
        case DOWN:
            Sleep(40);
            m_y++;
            break;
        default:
            break;
        }
        }
};

void Draw( Fruit &fruit, Snake &snake, Screen &screen){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
    CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = false;
    char snake_head = snake.Draw_Tile();
    char fruit_tile = fruit.Draw_Tile();
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    for (int i = 0; i < screen.m_width + 2; i++){
        std::cout<< "#";
    }
    std::cout << '\n';
    for (int i = 0; i < screen.m_height; i++){
        for (int j = 0; j < screen.m_width; j++){
            if ( j == 0){
                std::cout << "#";
            }
            if ( j == snake.m_x && i == snake.m_y){
                std::cout << snake_head;
            }
            else if (i == fruit.m_y && j == fruit.m_x){
                std::cout << fruit_tile;
            }
            else{
                bool print = false;
                for (int k = 0; k < snake.m_nTail; k++){
                    if(snake.m_tailX[k] == j && snake.m_tailY[k] == i){
                        std::cout << "o";
                        print = true;
                    }
                }
                if (!print){
                    std::cout << " ";
                }
            }
            if  (j == screen.m_width -1){
                std::cout << "#";
                std::cout << '\n';
            }
        }
    }
    for (int i = 0; i < screen.m_width + 2; i++){
        std::cout<< "#";
    }
    std::cout << '\n';
    std::cout << "SCORE: " << snake.m_score <<'\n';
}
bool StartOver(Screen &screen, Snake &snake, int highest)
{
    system("cls");
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});

    for (int i = 0; i < 12; i++){
    if(i == 6 || i == 10){
        for (int j = 0; j < screen.m_width + 2; j++){
            std::cout << "=";
        }
        std::cout << std::endl;
    }
    else if (i == 8){
        std::cout << "       GAME  OVER"<< std::endl;
        std::cout << "     final score: " << snake.m_score << std::endl;
        std::cout << "   highest score: " << highest << '\n';
    }
    else{
        std::cout << std:: endl;
    }
    }
    std::cout << "     if you want to" << std::endl << "  play again press 'y'!";

    //play again
    while(gameOver){
        if(_kbhit()){
        switch(_getch())
        {
        case 'y':
            return true;
        default:
            return false;
        }
    }
    }
}
void GameLoop(Snake &snake, Fruit &fruit, Screen &screen)
{
    screen.SetupScreen();
    snake.SetupSnake(screen);
    fruit.CreateFruit(screen);
    while(!gameOver){
        Draw(fruit, snake, screen);
        snake.InputSnake();
        snake.MoveSnake();
        snake.SnakeCollide(screen);
        if(snake.SnakeEats(fruit)){
            fruit.CreateFruit(screen);
        }
    }
    Sleep(1000);
}
int HighestScore(Snake &snake, int i){
    int highest = 0; 
    int score;
    score = snake.GetScore();
    scores.resize(i+1);
    scores.at(i) = score;
    for (int number : scores){
        if ( number > highest){
            highest = number;
        }
    }
    return highest;
}

int main(int argc, char *argv[]){
    Screen screen{};
    Snake snake{};
    Fruit fruit{};
    int i = 0;
    int score;

    do{
        snake.SnakeReset(screen);
        GameLoop(snake, fruit, screen);
        score = HighestScore(snake, i);
        i++;
    }while(StartOver(screen, snake, score));
    
    system("cls");
    system("COLOR 0F");
    return 0;
}