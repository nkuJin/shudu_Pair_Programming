#include <iostream>
#include <stdlib.h>
#include <windows.h>
using namespace std;

int sudoku[10][10];
int row[10][10], column[10][10], block[4][4][10];
int known[10][10];
const int colour[10] = { 0,3,4,13,5,11,6,14,2,9 };
void print() {
	cout << "    1   2   3    4   5   6    7   8   9   " << endl;
	cout << "   =====================================  " << endl;
	for (int i = 1;i <= 9;i++) {
		cout << i << "|| ";
		for (int j = 1;j <= 9;j++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour[sudoku[i][j]]);
			cout << sudoku[i][j];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			if (j % 3 != 0) cout << " | ";
			else cout << " || ";
		}
		cout << endl;
		if (i % 3 != 0)cout << "   -------------------------------------  " << endl;
		else       cout << "   =====================================  " << endl;
	}

}
void search(int x, int y) {
	if (known[x][y]) {
		search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
	}
	else {
		if (x == 10 && y == 1) {
			print();
			exit(0);
		}
		else
			for (int i = 1;i <= 9;i++) {
				if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0) {
					sudoku[x][y] = i;
					row[x][i] = 1;
					column[y][i] = 1;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
					search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
					sudoku[x][y] = 0;
					row[x][i] = 0;
					column[y][i] = 0;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
				}
			}
	}
}


int main() {
	for (int i = 1;i <= 9;i++) {
		for (int j = 1;j <= 9;j++) {
			cin >> sudoku[i][j];
			if (sudoku[i][j]) known[i][j] = 1;
		}
	}
	for (int i = 1;i <= 9;i++) {
		for (int j = 1;j <= 9;j++) {
			if (sudoku[i][j]) {
				row[i][sudoku[i][j]] = 1;
				column[j][sudoku[i][j]] = 1;
				block[(i - 1) / 3 + 1][(j - 1) / 3 + 1][sudoku[i][j]] = 1;
			}
		}
	}
	search(1, 1);
	return 0;
}
