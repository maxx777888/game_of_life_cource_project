#include <iostream>
#include <Windows.h>
#include <ctime> //Можно добавлять если используется случайное число
#include <string>
#include <cstdlib>


using namespace std;

const char life_c = '*';
const char dead_c = '-';

//Функция которая подсчитывает кол-во живых клеток.
int lifeCellsCount(int** board, const int ROWS, const int COLS)
{
    int life_num = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == 1)
            {
                life_num++;
            }
        }
    }
    return life_num;
}

//Проверяет и отсекает все ненужные i
int get_g(int i, const int ROWS) {
    if (i < 0 || i >= ROWS) { return -1; }
    return i;
}
//Проверяет и отсекает все ненужные j
int get_v(int j, const int COLS) {
    if (j < 0 || j >= COLS) { return -1; }
    return j;
}

//Функция создает двумерные массивы. 
int** create2arr(const int ROWS, const int COLUMNS)
{
    int** arr2d = new int* [ROWS];
    for (int i = 0; i < ROWS; i++)
    {
        arr2d[i] = new int[COLUMNS];
    }
    return arr2d;
}
//Функция копирует двумерные массивы. 
int** copyZone(int** board, const int ROWS, const int COLS)
{
    int** arr2d = create2arr(ROWS, COLS);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            arr2d[i][j] = board[i][j];
        }
    }
    return arr2d;
}

//Функция, которая принимая массив, кол-во строк, колонок, 
//номер поколения и кол-во живых клеток выводит значения в массиве на экран
void showZone(int** board, const int ROWS, const int COLS, int g, int live_cells)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j]) { cout << life_c << " "; }
            else { cout << dead_c << " "; }
        }
        cout << endl;
    }
    cout << "Generation: " << g << ".  Alive cells: " << live_cells << endl;
}
//Перегруженный метод выводит в конце начальное положение игровой зоны
void showZone(int** board, const int ROWS, const int COLS)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j]) { cout << life_c << " "; }
            else { cout << dead_c << " "; }
        }
        cout << endl;
    }
}
//Функция очищает динамическую память, удаляя двумерный массив. 
void delete2dArr(int** arr2d, const int ROWS)//Функция стерает двумерный массив
{
    for (int i = 0; i < ROWS; i++)
    {
        delete[] arr2d[i];
    }
    delete[] arr2d;
    arr2d = nullptr;
}

int main()
{
    //Строка для создания случайных чисел
    srand(time(0));

    int width_bord; //Ширина игрального поля
    int height_bord;//Высота игрального поля

    int game_members = 0;//Кол-во живых клеток, генерируется случайным образом
    int min_members = 3;//Минимальное кол-во живых клеток

    cout << "Enter the width of the game field (minimum 3)" << '\n';
    cin >> width_bord;
    cout << "Enter the height of the game field (minimum 3)" << '\n';
    cin >> height_bord;


    //Создаем кол-во живых клеток, минимум должно быть 3
    do {
        game_members = (rand() % (width_bord * height_bord)) + 1;
    } while (game_members < min_members);

    //Создаем двумерный динамический массив с координатами живых клеток
    int** arr2dMembers = create2arr(game_members, 2);

    //Массив нужен для того чтобы координаты живых клеток не повторялись
    int* arr1dCheck = new int[game_members]();

    //Заполняем массив случайными координатами на игровой площадке
    for (int i = 0; i < game_members; i++)
    {
        int star_w = 0; //Координата по оси Х
        int star_h = 0; //Координата по оси У

        if (i == 0) //Если первый номер, то проскакиваем проверку и идем дальше 
        {
            star_w = rand() % width_bord;
            star_h = rand() % height_bord;

        }
        else
        {
            bool no_match = false;
            while (no_match == false)
            {
                star_w = rand() % width_bord;
                star_h = rand() % height_bord;
                //Создаем число, которое будет копировать случайно созданные координаты и
                //будет сравнивать с числом которое записано в массиве куда сохраняются уже созданные координаты
                int compare = star_w * 10 + star_h;
                //Флаг для проверки совпадения новых координат с уже созданными
                int match_compare = false;
                //Пробегаем по массиву уже созданных координат
                for (int k = 0; k < i; k++)
                {
                    if (compare == arr1dCheck[k])
                    {
                        match_compare = true;
                    }
                }

                if (match_compare == false)
                {
                    no_match = true;
                }
            }
        }

        //Заносим координаты в массив для координат живых клеток
        for (int j = 0; j < 2; j++)
        {

            if (j == 0)
            {
                arr2dMembers[i][j] = star_w;
            }
            else if (j == 1)
            {
                arr2dMembers[i][j] = star_h;
            }

        }
        //Дублируем координаты в массив для последующей проверки на совпадение
        arr1dCheck[i] = star_w * 10 + star_h;

    }

    delete[] arr1dCheck; //Очищаем динамическую память от проверочного массива 

    ///////////////////////////////////////////////////////
    //Начало самой игры
    system("cls");
    int g = 1; //Поколение
    int l_cells = 0; //Общее кол-во живых клеток
    //Создание двумерного массива с помощью функции. 
    int** play_zone = create2arr(width_bord, height_bord);
    //Заполнение массива мертвыми клетками
    for (int i = 0; i < width_bord; i++)
    {
        for (int j = 0; j < height_bord; j++)
        {
            play_zone[i][j] = 0;
        }
    }

    //Вводим координаты живых клеток в массив игровой зоны
    for (int i = 0; i < game_members; ++i)
    {
        int x, y;
        x = arr2dMembers[i][0];
        y = arr2dMembers[i][1];
        play_zone[x][y] = 1;
    }
    l_cells = game_members;


    showZone(play_zone, width_bord, height_bord, g, l_cells);//Первоначальный вывод на консоль игровой таблицы
    int** starting_game_point = copyZone(play_zone, width_bord, height_bord); //Копируем начальное состояние игры. 

    /////////////////////---Логика игры---/////////////////////////
    bool isGame = true;
    //Цикл который будет продолжаться пока не поймает рецессию или 
    //пока не останется живых клеток (что по-сути тоже рецесиия).  
    do {
        Sleep(1000); //Замена предыдущей функции из библиотеки <unistd.h>, которая не найдена sleep() на Sleep()
        system("cls");
        int** last_zone = copyZone(play_zone, width_bord, height_bord); //Копируем предыдущее состояние игры. 
        //Запускаем логику игры, проверяем соседей и меняем статус клетки подходящий по условию
        for (int i = 0; i < width_bord; i++)
        {
            for (int j = 0; j < height_bord; j++)
            {
                int n = 0;
                //Делаем проверку на валидность координат, если коодинаты угловые или находятся с краю вселенной, делаем корректировку на видимость ближайщих соседей.
                if (get_g(i - 1, width_bord) < 0 || get_v(j, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i - 1, width_bord)][get_v(j, height_bord)]; }

                if (get_g(i - 1, width_bord) < 0 || get_v(j - 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i - 1, width_bord)][get_v(j - 1, height_bord)]; }

                if (get_g(i - 1, width_bord) < 0 || get_v(j + 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i - 1, width_bord)][get_v(j + 1, height_bord)]; }

                if (get_g(i, width_bord) < 0 || get_v(j - 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i, width_bord)][get_v(j - 1, height_bord)]; }

                if (get_g(i, width_bord) < 0 || get_v(j + 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i, width_bord)][get_v(j + 1, height_bord)]; }

                if (get_g(i + 1, width_bord) < 0 || get_v(j - 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i + 1, width_bord)][get_v(j - 1, height_bord)]; }

                if (get_g(i + 1, width_bord) < 0 || get_v(j, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i + 1, width_bord)][get_v(j, height_bord)]; }

                if (get_g(i + 1, width_bord) < 0 || get_v(j + 1, height_bord) < 0) { n += 0; }
                else { n += last_zone[get_g(i + 1, width_bord)][get_v(j + 1, height_bord)]; }
                //Обновление игрового поля в зависимости от состояний и кол-ва соседей
                if (last_zone[i][j] == 0 && n == 3)
                {
                    play_zone[i][j] = 1;

                }
                else if (n < 2 || n > 3)
                {
                    play_zone[i][j] = 0;
                }
            }
        }

        g++;//Обновляем поколение
        l_cells = lifeCellsCount(play_zone, width_bord, height_bord);//Обновляем кол-во живых клеток
        showZone(play_zone, width_bord, height_bord, g, l_cells);

        //Проверка на наличие изменений в игре
        bool isRessation = true;
        for (int i = 0; i < width_bord; i++)
        {
            for (int j = 0; j < height_bord; j++)
            {
                if (last_zone[i][j] != play_zone[i][j])
                {
                    isRessation = false;
                }
            }
        }
        //Проверка на рецессию или на смерть всех клеток
        if (isRessation)
        {
            cout << "The world has stagnated. Game over!" << endl;
            cout << "The initial number of living cells was - " << game_members << endl;
            isGame = false;

        }
        if (l_cells == 0)
        {
            cout << "All cells are dead. Game over!" << endl;
            cout << "The initial number of living cells was - " << game_members << endl;
            isGame = false;
        }
        if (!isGame)
        {
            delete2dArr(last_zone, width_bord); //Освобождаем память удаляя вспомогательный динамический массив.
        }
    } while (isGame);

    //Вывод начальной игровой зоны чтобы увидеть конечные изменения
    showZone(starting_game_point, width_bord, height_bord);
    delete2dArr(starting_game_point, width_bord);//Очищаем память у начального массива, конец игры.
    delete2dArr(play_zone, width_bord);//Очищаем память у главного массива, конец игры. 
    delete2dArr(arr2dMembers, game_members);
}













