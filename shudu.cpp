#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include<fstream>
#include <cstring>
using namespace std;

int sudoku[10][10];
int problem[10][10];
int row[10][10], column[10][10], block[4][4][10];
int known[10][10];
int f = 0;
int cnt = 0;
const int colour[10] = { 0,3,4,13,5,11,6,14,2,9 };
void init() {
	for (int i = 1;i <= 9;i++) {
		for (int j = 1;j <= 9;j++) {
			if (sudoku[i][j]) {
				row[i][sudoku[i][j]] = 1;
				column[j][sudoku[i][j]] = 1;
				block[(i - 1) / 3 + 1][(j - 1) / 3 + 1][sudoku[i][j]] = 1;
			}
		}
	}
}
void print(int sudoku[][10]) {
	cout <<endl;
	for (int i = 1;i <= 9;i++) {
		for (int j = 1;j <= 9;j++) {
			if (j % 3 != 0) cout << sudoku[i][j] << " ";//非九宫格边界
			else cout << sudoku[i][j] << " ";//九宫格边界
		}
		cout << endl;
		//if (i % 3 != 0)cout << "   -------------------------------------  " << endl;//非九宫格边界
		//else       cout << "   =====================================  " << endl;//九宫格边界
	}
}

void print1(int sudoku[][10]) {
//#include <iostream>
//#include<fstream>
//
//	using namespace std;
//
//	int main() {
//		ofstream outfile;
//		outfile.open("aaa123.txt");
//		outfile << "输出的内容" << endl;
//		outfile << "输出" << endl;
//		outfile.close();
//		return 0;
//	}
	
	//outfile << endl;
	//for (int i = 1;i <= 9;i++) {
	//	for (int j = 1;j <= 9;j++) {
	//		if (j % 3 != 0) outfile << sudoku[i][j] << " ";//非九宫格边界
	//		else outfile << sudoku[i][j] << " ";//九宫格边界
	//	}
	//	outfile << endl;
	//}
	//outfile.close();
}

void search(int x, int y) {
	if (f) return;
	if (known[x][y]) {
		search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
	}
	else {
		if (x == 10 && y == 1) {
			f = 1;
			cnt++;
		}
		for (int j = 1;j < 40;j++) {
			int i = rand() % 9 + 1;
			if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0) {
				sudoku[x][y] = i;
				row[x][i] = 1;
				column[y][i] = 1;
				block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
				search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
				if (!f) {
					sudoku[x][y] = 0;
					row[x][i] = 0;
					column[y][i] = 0;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
				}
			}
		}
	}
}
void search2(int x, int y) {
	if (x != 10 && known[x][y]) {
		search2((9 * x + y - 9) / 9 + 1, y % 9 + 1);
	}
	else {
		if (x == 10 && y == 1) {
			cnt++;
		}
		else
			for (int i = 1;i <= 9;i++) {
				if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0) {
					sudoku[x][y] = i;
					row[x][i] = 1;
					column[y][i] = 1;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
					search2((9 * x + y - 9) / 9 + 1, y % 9 + 1);
					sudoku[x][y] = 0;
					row[x][i] = 0;
					column[y][i] = 0;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
				}
			}
	}
}

int main() {
	srand(time(0) * time(0) - 0x5e2d6aa * rand() + time(0) * 338339);//复杂的种子值
	search(1, 1);////生成一个数独
	cout << "数独生成中..." << endl;
	int sum = 0;
	int sudoku2[10][10] = { 0 };
	int i = 100;
	/*if (argv[1] == "c") {
		i = *(argv[2]);
	}*/
	ofstream outfile;
	outfile.open("shudu.txt");
	while (i > 0) {
		
		for (int i = 1;i <= 9;i++) {
			for (int j = 1;j <= 9;j++) {
				problem[i][j] = sudoku2[i][j] = sudoku[i][j];
			}
		}
		cnt = 0;
		while (cnt != 1) {
			for (int i = 1;i <= 9;i++) {
				for (int j = 1;j <= 9;j++) {
					known[i][j] = 1;
				}
			}
			sum = 0;
			while (sum < 50) {
				int x = rand() % 9 + 1, y = rand() % 9 + 1;
				if (known[x][y] == 0) continue;
				else {
					sum++;
					known[x][y] = 0;
				}
			}
			for (int i = 1;i <= 9;i++) {
				for (int j = 1;j <= 9;j++) {
					if (known[i][j] == 0) sudoku[i][j] = problem[i][j] = 0;
				}
			}
			cnt = 0;
			f = 0;
			::memset(row, 0, sizeof(row));
			::memset(column, 0, sizeof(column));
			::memset(block, 0, sizeof(block));
			init();
			search2(1, 1);
			for (int i = 1;i <= 9;i++) {
				for (int j = 1;j <= 9;j++) {
					sudoku[i][j] = sudoku2[i][j];
					if (cnt != 1) problem[i][j] = sudoku2[i][j];
				}
			}
		}
		//print(problem);
		outfile << endl;
		for (int i = 1;i <= 9;i++) {
			for (int j = 1;j <= 9;j++) {
				if (j % 3 != 0) outfile << problem[i][j] << " ";//非九宫格边界
				else outfile << problem[i][j] << " ";//九宫格边界
			}
			outfile << endl;
		}
		i--;
	}
	outfile.close();
	cout << "数独生成完毕！";
	return 0;
}
