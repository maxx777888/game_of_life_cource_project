#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <Windows.h>//Замена с библиотеки <unistd.h>, которая почему-то не нашлась. 
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
//Проверка на наличие файла
bool checkFile()
{
    ifstream check_file("zone.txt");
    if (check_file.is_open()) {
        return true;
    }
    else {
        // На случай если файл будет не найден, выдет сообщение об этом. 
        cout << "Ошибка!!! Файл не найден" << endl;
        return false;
    }
}

//Функция котороя подсчитывает количество строк в файле и возвращает начальное кол-во живых клеток. 
int numLifeCellsStart()
{
    ifstream start_file("zone.txt");//Создаем переменную типа std::ifstream
    string smr;
    int num_life = 0;
    //Проверка на наличие файла
    if (checkFile())
    {
        //Счетает количество оставщихся строк в файле
        while (getline(start_file, smr)) {
            num_life++;
        }
        start_file.close();
        num_life -= 1; //Убераем верхнюю строчку. 
        return num_life;
    }
    else
    {
        return 0;
    }
}
//Функция создает двумерные массивы. 
template<typename T>
T** create2arr(const int ROWS, const int COLUMNS)
{
    T** arr2d = new T * [ROWS];
    for (int i = 0; i < ROWS; i++)
    {
        arr2d[i] = new T[COLUMNS];
    }
    return arr2d;
}
//Функция копирует двумерные массивы. 
template<typename T>
T** copyZone(T** board, const int ROWS, const int COLS)
{
    T** arr2d = create2arr<int>(ROWS, COLS);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            arr2d[i][j] = board[i][j];
        }
    }
    return arr2d;
}

//Функция, которая принимая массив, кол-во строк, колонок, номер поколения и кол-во живых клеток выводит значения в массиве на экран
template<typename T>
void showZone(T** board, const int ROWS, const int COLS, int g, int live_cells)
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
//Функция очищает динамическую память, удаляя двумерный массив. 
template<typename T>
void delete2dArr(T** arr2d, const int ROWS)//Функция стерает двумерный массив
{
    for (int i = 0; i < ROWS; i++)
    {
        delete[] arr2d[i];
    }
    delete[] arr2d;
    arr2d = nullptr;
}


int main() {
    system("cls");
    //Начало. Считываем заданные параметры игры из файла
    ifstream file("zone.txt");//Создаем переменную типа std::ifstream  
    //string str_z_size_g; //Поле по горизонтале, кол-во строчек в массиве
    //string str_z_size_v; //Поле по вертикате, кол-во колонок в массиве  
    int int_size_z_g = 0;
    int int_size_z_v = 0;

    if (checkFile()) //Проверка на наличие файла.
    { //Открываем файл и берем файлы для работы    
        file >> int_size_z_g;
        file >> int_size_z_v;
        //int_size_z_g = std::stoi(str_z_size_g); 
        //int_size_z_v = std::stoi(str_z_size_v);
        int g = 1; //Поколение
        int l_cells = 0; //Общее кол-во живых клеток
        //Создание двумерного массива с помощью функции. 
        int** play_zone = create2arr<int>(int_size_z_g, int_size_z_v);
        //Заполнение массива мертвыми клетками
        for (int i = 0; i < int_size_z_g; i++)
        {
            for (int j = 0; j < int_size_z_v; j++)
            {
                play_zone[i][j] = 0;
            }
        }
        int num_life = numLifeCellsStart();//Начальное кол-во живых клеток
        //Проверка на наличие в файле координат с живыми клетками, если нет конец игре. 
        if (num_life == 0)
        {
            showZone(play_zone, int_size_z_g, int_size_z_v, g, 0);
            cout << "All cells are dead. Game over!" << endl;
            file.close();
            delete2dArr(play_zone, int_size_z_g);//Очищаем память у главного массива, конец игры.  
        }
        else
            //Создаем и заполняем динамический массив для координат живых клеток взятый из файла.
        {
            int** st_life_arr = create2arr<int>(num_life, 2);
            //Заполняем массив координатами из файла
            for (int i = 0; i < num_life; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    string str;
                    file >> str;
                    st_life_arr[i][j] = std::stoi(str);
                }
            }
            file.close();//Закрываем файл, он больше не понадобиться
            //Вводим координаты живых клеток в массив таблицы игры
            for (int i = 0; i < num_life; ++i)
            {
                int x, y;
                x = st_life_arr[i][0];
                y = st_life_arr[i][1];
                play_zone[x][y] = 1;
            }
            l_cells = num_life;

            delete2dArr(st_life_arr, num_life);//Очищаем память от массива с координатами, он больше не нужен
            showZone(play_zone, int_size_z_g, int_size_z_v, g, l_cells);//Первоначальный вывод игровой таблицы

            /////////////////////---Логика игры---/////////////////////////
            bool isGame = true;
            do {
                Sleep(1000); //Замена предыдущей функции из библиотеки <unistd.h>, которая не найдена sleep() на Sleep()
                system("cls");
                int** last_zone = copyZone(play_zone, int_size_z_g, int_size_z_v); //Копируем предыдущее состояние игры. 
                //Запускаем логику игры, проверяем соседей и меняем статус клетки подходящий по условию
                for (int i = 0; i < int_size_z_g; i++)
                {
                    for (int j = 0; j < int_size_z_v; j++)
                    {
                        int n = 0;
                        //Делаем проверку на валидность координат, если коодинаты угловые или находятся с краю вселенной, делаем корректировку на видимость ближайщих соседей.
                        if (get_g(i - 1, int_size_z_g) < 0 || get_v(j, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i - 1, int_size_z_g)][get_v(j, int_size_z_v)]; }

                        if (get_g(i - 1, int_size_z_g) < 0 || get_v(j - 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i - 1, int_size_z_g)][get_v(j - 1, int_size_z_v)]; }

                        if (get_g(i - 1, int_size_z_g) < 0 || get_v(j + 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i - 1, int_size_z_g)][get_v(j + 1, int_size_z_v)]; }

                        if (get_g(i, int_size_z_g) < 0 || get_v(j - 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i, int_size_z_g)][get_v(j - 1, int_size_z_v)]; }

                        if (get_g(i, int_size_z_g) < 0 || get_v(j + 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i, int_size_z_g)][get_v(j + 1, int_size_z_v)]; }

                        if (get_g(i + 1, int_size_z_g) < 0 || get_v(j - 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i + 1, int_size_z_g)][get_v(j - 1, int_size_z_v)]; }

                        if (get_g(i + 1, int_size_z_g) < 0 || get_v(j, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i + 1, int_size_z_g)][get_v(j, int_size_z_v)]; }

                        if (get_g(i + 1, int_size_z_g) < 0 || get_v(j + 1, int_size_z_v) < 0) { n += 0; }
                        else { n += last_zone[get_g(i + 1, int_size_z_g)][get_v(j + 1, int_size_z_v)]; }
                        //Обновление игрового поля в зависимости от состояний и кол-ва соседей
                        if (last_zone[i][j] == 0 && n == 3)
                        {
                            play_zone[i][j] = 1;
                            //} //else if (last_zone[i][j] == 1 && (n == 2 || n == 3))
                              //{
                                //play_zone[i][j] = 1;
                        }
                        else if (n < 2 || n > 3)
                        {
                            play_zone[i][j] = 0;
                        }
                    }
                }

                g++;//Обновляем поколение
                l_cells = lifeCellsCount(play_zone, int_size_z_g, int_size_z_v);//Обновляем кол-во живых клеток
                showZone(play_zone, int_size_z_g, int_size_z_v, g, l_cells);

                //Проверка на наличие изменений в игре
                bool isRessation = true;
                for (int i = 0; i < int_size_z_g; i++)
                {
                    for (int j = 0; j < int_size_z_v; j++)
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
                    isGame = false;

                }
                if (l_cells == 0)
                {
                    cout << "All cells are dead. Game over!" << endl;
                    isGame = false;
                }
                if (!isGame)
                {
                    delete2dArr(last_zone, int_size_z_g); //Освобождаем память удаляя вспомогательный динамический массив.
                }
            } while (isGame); //Цикл который будет продолжаться пока не поймает рецессию или пока не останется живых клеток (что по-сути тоже рецесиия).  
            delete2dArr(play_zone, int_size_z_g);//Очищаем память у главного массива, конец игры. 
        }
    }
    else
    {
        // На случай если файл будет не найден, выдет сообщение об этом. 
        std::cout << "Проверьте наличие постановочного файла!!!" << std::endl;
    }
}




