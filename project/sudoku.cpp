#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include<fstream>
#include <cstring>
using namespace std;

const int BLANK[3] = {30, 40, 50};//不同等级的挖空数

int Sudoku[9][9];//数独终局
int Game[9][9];//数独游戏
int Prob[9][9];//待求解数独

int solution_num = 0;

// int Row[9][9], Column[9][9], Block[3][3][9];//行 列 块 是否可放置的依据

/*
-c : sudoku.exe -c 20 
表示生成20个数独终盘 
agrc=3 argv[1]="-c"
*/
void cmd_c(int suduku_final_num);//输出终盘到Sudoku_Final_Set.txt
/*
-s : sudoku.exe -s game.txt 
读取从game.txt中所有数独游戏，求解，输出到suduku.txt 
agrc=3 argv[1]="-s"
*/
void cmd_s(const char* filename);//从filename读数独 解答 输出到sudoku.txt
/*
-n : sudoku.exe -n 1000 
表示生成1000个数独游戏 
agrc=3 argv[1]="-n"
-m: sudoku.exe -n 1000 -m 1 
表示生成游戏难度见简单，m和n必须一起使用，否则报错
-r:sudoku.exe -n 20 -r 20~55 
表示生成挖空数在20到55，r和n必须一起使用，否则报错
-u:sudoku.exe -n 20 -u 
表示生成解唯一，u和n必须一起使用，否则报错
有-n时-m、-u; -r、-u可以一起使用 其中-m、-r均决定blank_num;-u决定only 
*/
void cmd_n(int game_num, int blank_num, bool only, int bottom, int top);//输出游戏到Game.txt


//生成终局
bool generate_final(int row, int col);
//生成游戏
bool generate_game(int blank_num, bool only);
//输出到终端
void print_in_terminal(int arr[9][9]);
//判断(row,col)是否可填num
bool isValid(int arr[9][9], int row, int col, int num);
//求解，并返回是否有解
bool Solve();
//判断是否求解已完成
bool isSolved(int& row, int& col);
//检查挖去[row][col]后是否有唯一解
bool Check_Only(int row, int col);


void cmd_c(int suduku_final_num)
{
    ofstream outfile;
	outfile.open("Sudoku_Final_Set.txt");

    int i = 0;
    while(i<suduku_final_num)
    {
        ::memset(Sudoku, 0 ,sizeof(Sudoku)); 
        // generate_final(0,0);
        while(1){
            ::memset(Sudoku, 0 ,sizeof(Sudoku)); 
            if (generate_final(0,0))
                break;
        }
        // print_in_terminal(Sudoku); 

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) 
            {
                if (j!=8) outfile << Sudoku[i][j] << "&";
                else outfile << Sudoku[i][j];
            }
            outfile<<endl;
        }
        outfile<<endl;
        i++;       
    }
    outfile.close();
}

void cmd_s(const char* filename){
    ifstream infile;
    infile.open(filename);

    ofstream outfile;
    outfile.open("sudoku.txt");

    int count = 0; 
    string line; 
    while (1) 
    {
        int cur_number;
        char ch;
        ::memset(Game, 0 ,sizeof(Game));
        ::memset(Prob, 0 ,sizeof(Prob));

        // for (int i = 0; i < 9; i++) 
        //     for(int j = 0; j < 9; j++){
        //         if(infile >> cur_number) {
        //             Game[i][j] = cur_number;
        //             Prob[i][j] = cur_number;
        //         }
        //         else {
        //             infile.close();
        //             outfile.close();
        //             return;
        //         }
        //     } 
           
        for (int i = 0; i < 9; i++) 
            for(int j = 0; j < 9; j++){
                if(j !=8){
                    if(infile >> cur_number) {
                        Game[i][j] = cur_number;
                        Prob[i][j] = cur_number;
                        infile >> ch;
                    }
                    else {
                        infile.close();
                        return;
                    }
                }
                else{
                    if(infile >> cur_number) {
                        Game[i][j] = cur_number;
                        Prob[i][j] = cur_number;
                    }
                    else {
                        infile.close();
                        return ;
                    }                    
                }
            } 

        if (Solve()){
            // print_in_terminal(Prob);
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) 
                {
                    if (j!=8) outfile << Game[i][j] << "&";
                    else outfile << Game[i][j];
                }
                outfile<<" | ";

                for (int j = 0; j < 9; j++) 
                {
                    if (j!=8) outfile << Prob[i][j] << "&";
                    else outfile << Prob[i][j];
                }
                
                outfile<<endl;
            }
        }
        else {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++)
                {
                    if (j!=8) outfile << Game[i][j] << "&";
                    else outfile << Game[i][j];
                }
                
                if(i == 4) outfile<< "  No solution exists" ;
                
                outfile<<endl; 
            }            
        }

        outfile<<endl;               
    }

}    

void cmd_n(int game_num, int blank_num, bool only, int bottom = 0, int top = 0){
    ofstream outfile;
	outfile.open("Game.txt");

    int i = 0;
    bool rand_blank = false;

    if(blank_num == 0){
        if(bottom ==0) blank_num = 30;
        else rand_blank = true;   
    }
    while(i<game_num)
    {
        if(rand_blank)       
            blank_num = rand() % (top - bottom + 1) + bottom; 
        ::memset(Sudoku, 0 ,sizeof(Sudoku));

        while(1){
            ::memset(Sudoku, 0 ,sizeof(Sudoku)); 
            if (generate_final(0,0))
                break;
        }
        // cout<<"Sudoku"<<endl;
        // print_in_terminal(Sudoku);

        while(1){
            if (generate_game(blank_num,only))
                break;
        }

        // print_in_terminal(Game);

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) 
            {
                if (j!=8) outfile << Game[i][j] << "&";
                else outfile << Game[i][j];
            }
            outfile<<endl;
        }
        outfile<<endl;

        i++;       
    }
    outfile.close();    
}

void print_in_terminal(int arr[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
}

bool isValid(int arr[9][9], int row, int col, int num)
{
    // 检查行
    for (int cur_col = 0; cur_col < 9; cur_col++)
        if (arr[row][cur_col] == num)
            return false;
    // 检查列
    for (int cur_row = 0; cur_row < 9; cur_row++)
        if (arr[cur_row][col] == num)
            return false;
    // 检查小宫格
    int row_id = row - row % 3;
    int col_id = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (arr[i + row_id][j + col_id] == num)
                return false;
    return true;
}

bool generate_final(int row, int col)
{
    //全部生成
    if(row == 9) return true;
    //如果当前位置已生成
    if(Sudoku[row][col] != 0)
    {
        if (col == 8) {
            return generate_final(row + 1, 0);
        } else {
            return generate_final(row, col + 1);
        }
    }
    //乱序1~9
    int Integer[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    for (int i = 0; i < 9; i++) {
        int j = rand() % 9;
        swap(Integer[i], Integer[j]);
    }

    //乱序放置1~9
    for(int i = 0; i< 9; i++)
    {
        int num = Integer[i];
        if(isValid(Sudoku, row, col, num))
        {
            Sudoku[row][col] = num;

            if (col == 8) {
                if (generate_final(row + 1, 0)) return true;
            } else {
                if (generate_final(row, col + 1)) return true;
            }

            Sudoku[row][col] = 0;         
        }
    }
    return false;   
}

bool Check_Only(int row, int col){
    int num = Game[row][col];

    for(int i = 1; i <= 9; i++){
        if(i != num)
        {
            ::memset(Prob, 0 ,sizeof(Prob)); 

            for (int i = 0; i < 9; i++) 
                for (int j = 0; j < 9; j++) 
                    Prob[i][j] = Game[i][j];
            
            if(isValid(Prob, row, col, i)) Prob[row][col] = i;
            else {
                continue;
            }
            if(Solve())
            {
                return false;
            }
        }
    }
    return true;
}

bool generate_game(int blank_num, bool only)
{
    //Suduku终局
    //Game挖空后的游戏
    //Prob用于求解
    ::memset(Game, 0, sizeof(Game));  
    for (int i = 0; i < 9; i++) 
        for (int j = 0; j < 9; j++) 
            Game[i][j] = Sudoku[i][j];

    
    int cur_num = 0;

    while(cur_num < blank_num)
    {
        int r = rand() % 9;
        int c = rand() % 9;

        if(Game[r][c] == 0) continue;
        else{
            if(!only){
                Game[r][c] = 0;
                cur_num++;
            }
            else{
                if(Check_Only(r, c)){
                    Game[r][c] = 0;
                    cur_num++;                    
                }
                else{
                    return false;
                }
            }
            
        }
    } 

    return true;       
      
    

}


bool isSolved(int& row, int& col)
{
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (Prob[row][col] == 0)
                return false;//row col记录待求解位置
    return true;
}

bool Solve()
{
    int row, col;
    //检查是否已完成求解,并获取待求解位置
    if (isSolved(row, col))
        return true;

    for (int num = 1; num <= 9; num++)
    {
        if (isValid(Prob, row, col, num))//尝试填充
        // if (isValid(row, col, num))//尝试填充
        {
            Prob[row][col] = num;


            if (Solve())
                return true;

            Prob[row][col] = 0;       
        }
    }

    return false;
}

void Hint(){
    //TODO:打印提示信息
    cout << "Hint : "<<endl;
    cout << "-c: Generate Sudoku Final "<<endl<<"      sudoku.exe -c Number_of_Finals(1-1000000)" << endl; 
    cout << "-s: Solving Sudoku "<<endl<<"      sudoku.exe -s Path_of_Sudoku_Chessboard(Absolute or relative path) "<< endl; 
    cout << "-n: Generate Sudoku Games "<<endl<<"      sudoku.exe -n Number_of_games(1-10000)" << endl; 
    cout << "-m: Set the difficulty level of the game (Must be used with - n) "<<endl<<"      sudoku.exe -n Number_of_games -m difficulty_level(1-3)" << endl; 
    cout << "-r: Set the number of hollows in the game (Must be used with - n) "<<endl<<"      sudoku.exe -n Number_of_games -r range_of_number(connected by ~)" << endl; 
    cout << "-u: Generate unique solutions for games (Must be used with - n) "<<endl<<"      sudoku.exe -n Number_of_games -u" << endl; 
    
    exit(0);
}
//
bool hasNonDigits(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return true;
        }
    }
    return false;
}
bool contains_non_digit(string str)
{
    for (char c : str) {
        if (!isdigit(c)) {
            return true;
        }
    }
    return false;
}
int main(int argc, char** argv){
    srand(time(0) * time(0) - 0x5e2d6aa * rand() + time(0) * 338339 + time(0));//复杂的种子值

    if(strcmp(argv[0], "sudoku.exe") != 0) {
        Hint();
    }
    
    if(argc == 3){
        if(strcmp(argv[1], "-c") == 0){
            // sudoku.exe -c 20
            //TODO:-c参数检验 argv[2]：1-1000000整数
            if(hasNonDigits(argv[2])){
                Hint();
            }
            else{
                int final_num = atoi(argv[2]);
                if(final_num <1 || final_num>1000000) Hint();
                else cmd_c(final_num);
            }

        }
        else if(strcmp(argv[1], "-s") == 0)
        {
            // sudoku.exe -s game.txt
            char filename[100];
            //TODO:-s参数检验 argv[2]：文件名
            sprintf(filename, argv[2]);
            cmd_s(filename);
        }
        else if(strcmp(argv[1], "-n") == 0){
            // sudoku.exe -n 1000 
            //TODO:-n参数检验 argv[2]：1-10000整数
            if(hasNonDigits(argv[2])){
                Hint();
            }
            else{
            int Game_num = atoi(argv[2]);
                if(Game_num <1 || Game_num>10000) Hint();
                else cmd_n(Game_num, 30,false);
            }
        }
    }
    else{
        bool _n = false;//是否有-n
        int level = 0;//难度等级
        int bottom = 0;
        int top = 0;//空格范围
        int blank_num ;//空格数
        bool isOnly = false;//是否唯一解
        int game_num = 0;//游戏局数 
        for(int i=1;i<argc;i++)
        {
            if(strcmp(argv[i], "-n") == 0){
                i++;
                _n = true;
                //TODO：-n参数检验 argv[i]：1-10000整数
                if(hasNonDigits(argv[i])){
                    Hint();
                }
                game_num = atoi(argv[i]);
                if(game_num <1 || game_num>10000) Hint();
            }
            else if(strcmp(argv[i], "-m") == 0){
                i++;
                //TODO:-m参数检验 argv[i]：1 or 2 or 3
                if(hasNonDigits(argv[i])){
                    Hint();
                }
                level = atoi(argv[i]);
                if(level != 1 && level != 2 && level != 3) Hint();
                blank_num = BLANK[level-1];

            }
            else if(strcmp(argv[i], "-r") == 0){
                i++;
                //TODO:-r参数检验 argv[i]：integer1~integer2 20<=integer1<=integer2<=50
                string s(argv[i]);
                size_t pos = s.find('~');
                if(pos == 0) Hint();
                if(!contains_non_digit(s.substr(0, pos))) Hint();
                if(!contains_non_digit(s.substr(pos + 1))) Hint();
                bottom = stoi(s.substr(0, pos));
                top = stoi(s.substr(pos + 1));  
                if(bottom >= top|| bottom < 20 || top > 55) Hint();
            }
            else if(strcmp(argv[i], "-u") == 0){
                isOnly = true;
            }
            else{
                Hint();
            }
        }
        if(!_n) {
            Hint();
        } 
        cmd_n(game_num, blank_num, isOnly, bottom, top);
    } 
        
    exit(0);   
}

