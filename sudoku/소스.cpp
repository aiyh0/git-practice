#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

#define SIZE 9 
#define MARGIN_X 2
#define MARGIN_Y 2
#define SUDOKU 5 // 0~5
#define MOD 1

void gotoxy(int x, int y);
void frame();
void printNum(int, int, int, int);
void printStatus(int x, int y, int num);
bool checkDuplicate(int x, int y, int digit, int(*A)[9]);
bool checkDigitClear(int(*A)[9]);
void autoCandidate(int(*A)[9], bool(*candidate)[9][10]);
void printSudoku(int(*A)[9], bool(*candidate)[9][10], int(*lock)[9]);
int scanAnswer(const char* input, int(*A)[9], bool(*candidate)[9][10]);
int scanSingleCandidate(int(*A)[9], bool(*candidate)[9][10]);

int main() {
	system(" mode  con lines=45   cols=86 ");
	frame();
	int A[SIZE][SIZE] = {0};
	int lock[SIZE][SIZE] = { 0 };
	bool candidate[9][9][10] = { false };

	FILE* fp = fopen("sudoku.txt", "r+");
	if (fp == NULL) fclose(fp);
	else {
		for (int i = 0; i < SUDOKU; i++) {
			fseek(fp, 168, SEEK_CUR);
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				A[i][j] = fgetc(fp) - '0';
				printNum(j, i, 4, A[i][j]);
				if (A[i][j] != 0) {
					printStatus(j, i , 1);
					lock[i][j] = A[i][j];
				}
				else
					printStatus(j, i , 0);
			}
		}
	}
	checkDigitClear(A);
	gotoxy(2*(MARGIN_X + 2), MARGIN_Y + 2);

	if (MOD == 1) {
		autoCandidate(A, candidate);
		printSudoku(A, candidate, lock);
		_getch();

		int found = 1;
		while (found > 0) {
			found = 0;
			found += scanSingleCandidate(A, candidate);
			autoCandidate(A, candidate);
			found += scanAnswer("row", A, candidate);
			autoCandidate(A, candidate);

			found += scanSingleCandidate(A, candidate);
			autoCandidate(A, candidate);
			found += scanAnswer("column", A, candidate);
			autoCandidate(A, candidate);

			found += scanSingleCandidate(A, candidate);
			autoCandidate(A, candidate);
			found += scanAnswer("cell", A, candidate);
			autoCandidate(A, candidate);
		}
		printSudoku(A, candidate, lock);
		_getch();
	}
						
	int x = 0, y = 0, temp;
	char input;
	while (1) {
		input = _getch();
		if (input >= 49 && input <= 57 && lock[y][x] == 0) {
			int digit = input - '0';
			if (candidate[y][x][0] == false) {
				if (checkDuplicate(x, y, digit, A) == false) {
					printNum(x, y, 4, digit);
					A[y][x] = digit;
				}
			}
			else {
				if (candidate[y][x][digit] == false) {
					candidate[y][x][digit] = true;
					printNum(x, y, digit - 1, digit);
				}
				else {
					candidate[y][x][digit] = false;
					printNum(x, y, digit - 1, 0);
				}
			}
		}
		else {
			switch (input) {
			case'w': if (y > 0) y--; break;
			case'a': if (x > 0) x--; break;
			case's': if (y < 8) y++; break;
			case'd': if (x < 8) x++; break;
			case'\b':
				if (lock[y][x] == 0 && candidate[y][x][0] == false) {
					A[y][x] = 0;
					printNum(x, y, 4, A[y][x]);
				
				}
				break;
			case'r': // modify later
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						A[i][j] = lock[i][j];
						printNum(j, i, 4, A[i][j]);
					}
				}
				break;
			case'c':
				if (lock[y][x] == 0) {
					if (candidate[y][x][0] == false) {
						candidate[y][x][0] = true;
						printStatus(x, y, 2);
						printNum(x, y, 4, 0);
						for (int i = 1; i <= 9; i++) {
							if (candidate[y][x][i] == true)
								printNum(x, y, i - 1, i);
						}
					}
					else {
						candidate[y][x][0] = false;
						for (int i = 0; i < 9; i++) {
							printNum(x, y, i, 0);
						}
						printNum(x, y, 4, A[y][x]);
						printStatus(x, y, 0);
					}
				}
				break;
			}
		}
		if (checkDigitClear(A)) break;
		gotoxy(2 * (MARGIN_X + 4 * x + x / 3 + 2), MARGIN_Y + 4 * y + y / 3 + 2);
	}
	Sleep(1000);
	fseek(fp, 2, SEEK_CUR);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fputc(A[i][j] + '0', fp);
		}
	}
	fclose(fp);
}


void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void frame() {
	for (int n = 0; n < 9; n++) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(2 * (MARGIN_X + 13 * (n % 3) + 4 * j), MARGIN_Y + 13 * (n / 3) + 1 + i);
				printf("早");
			}
		}
		gotoxy(2 * (MARGIN_X + 13 * (n % 3)), MARGIN_Y + 13 * (n / 3));
		printf("旨 收 收 收 有 收 收 收 有 收 收 收 旬");
		gotoxy(2 * (MARGIN_X + 13 * (n % 3)), MARGIN_Y + 13 * (n / 3) + 4);
		printf("曳 收 收 收 朱 收 收 收 朱 收 收 收 朽");
		gotoxy(2 * (MARGIN_X + 13 * (n % 3)), MARGIN_Y + 13 * (n / 3) + 8);
		printf("曳 收 收 收 朱 收 收 收 朱 收 收 收 朽");
		gotoxy(2 * (MARGIN_X + 13 * (n % 3)), MARGIN_Y + 13 * (n / 3) + 12);
		printf("曲 收 收 收 朴 收 收 收 朴 收 收 收 旭");
	}
	for (int i = 0; i < 39; i++) {
		gotoxy(2 * (MARGIN_X - 1), MARGIN_Y + i);
		printf("早");
		gotoxy(2 * (MARGIN_X + 39), MARGIN_Y + i);
		printf("早");
	}
	for (int i = 0; i < 39; i++) {
		gotoxy(2 * (MARGIN_X + i), MARGIN_Y - 1);
		printf("收");
		gotoxy(2 * (MARGIN_X + i), MARGIN_Y + 39);
		printf("收");
	}
	gotoxy(2 * (MARGIN_X - 1), MARGIN_Y - 1);
	printf("旨");
	gotoxy(2 * (MARGIN_X + 39), MARGIN_Y - 1);
	printf("旬");
	gotoxy(2 * (MARGIN_X - 1), MARGIN_Y + 39);
	printf("曲");
	gotoxy(2 * (MARGIN_X + 39), MARGIN_Y + 39);
	printf("旭");

}

void printNum(int x, int y, int blank, int num) {
	gotoxy(2 * (MARGIN_X + 4 * x + x / 3 + 1 + blank % 3), MARGIN_Y + 4 * y + y / 3 + 1 + blank / 3);
	switch (num) {
	case 0: printf("﹛"); break;
	case 1: printf("ㄠ"); break;
	case 2: printf("ㄡ"); break;
	case 3: printf("ㄢ"); break;
	case 4: printf("ㄣ"); break;
	case 5: printf("ㄤ"); break;
	case 6: printf("ㄥ"); break;
	case 7: printf("ㄦ"); break;
	case 8: printf("ㄧ"); break;
	case 9: printf("ㄨ"); break;
	}
}
void printStatus(int x, int y, int num) {
	gotoxy(2 * (MARGIN_X + 4 * x + x / 3 + 3), MARGIN_Y + 4 * y + y / 3 + 1);
	switch (num) {
	case 0: printf("∞"); break;
	case 1: printf("≒"); break;
	case 2: printf("﹛"); break;
	}
}

bool checkDuplicate(int x, int y, int digit, int(*A)[9]) {
	for (int i = 3 * (y / 3); i < 3 * (y / 3 + 1); i++) {
		for (int j = 3 * (x / 3); j < 3 * (x / 3 + 1); j++) {
			if (A[i][j] == digit && i != y && j != x) return true;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (A[i][x] == digit && i != y || A[y][i] == digit && i != x) return true;
	}
	return false;
}

bool checkDigitClear(int(*A)[9]) {
	for (int i = 0; i < 9; i++) {
		printNum(i, 9, 4, 0);
		printf("   ");
	}
	int checkDigit[9] = { 0 };
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) continue;
			checkDigit[A[i][j] - 1]++;
		}
	}
	int count = 0;
	for (int i = 0; i < 9; i++) {
		if (checkDigit[i] == 9)
			count++;
		else {
			printNum(i, 9, 4, i + 1);
			printf("(%d)", 9 - checkDigit[i]);
		}
	}
	if (count == 9) return true;
	else return false;
}
void autoCandidate(int(*A)[9], bool (*candidate)[9][10]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				for (int k = 1; k <= 9; k++) {
					if (checkDuplicate(j, i, k, A) == false)
						candidate[i][j][k] = true;
					else
						candidate[i][j][k] = false;
				}
			}
		}
	}
}
void printSudoku(int(*A)[9], bool(*candidate)[9][10], int(*lock)[9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (lock[i][j] == 0) {
				for (int k = 1; k <= 9; k++)
					printNum(j, i, k - 1, 0);
				if (A[i][j] == 0) {
					printStatus(j, i, 2);
					for (int k = 1; k <= 9; k++) {
						if (candidate[i][j][k] == true)
							printNum(j, i, k - 1, k);
					}
				}
				else {
					printNum(j, i, 4, A[i][j]);
					printStatus(j, i, 0);
				}
			}
		}
	}
}
int scanAnswer(const char* input, int(*A)[9], bool(*candidate)[9][10]) {
	int x, y;
	bool row = false, column = false, cell = false;
	int found = 0;

	if (input == "row") row = true;
	else if (input == "column") column = true;
	else if (input == "cell") cell = true;
	else return false;

	for (int i = 0; i < 9; i++) {
		int count[9] = { 0 };
		for (int j = 0; j < 9; j++) {
			x = j * row + i * column + (3 * (i % 3) + j % 3) * cell;
			y = i * row + j * column + (3 * (i / 3) + j / 3) * cell;
			if (A[y][x] == 0) {
				for (int k = 1; k <= 9; k++) {
					if (candidate[y][x][k] == true)
						count[k - 1]++;
				}
			}
		}
		for (int n = 1; n <= 9; n++) {
			if (count[n - 1] == 1) {
				int j = 0;
				x = j * row + i * column + (3 * (i % 3) + j % 3) * cell;
				y = i * row + j * column + (3 * (i / 3) + j / 3) * cell;
				while (candidate[y][x][n] != true) {
					j++;
					x = j * row + i * column + (3 * (i % 3) + j % 3) * cell;
					y = i * row + j * column + (3 * (i / 3) + j / 3) * cell;
				}
				A[y][x] = n;
				for (int m = 1; m <= 9; m++)
					candidate[y][x][m] = false;
				found++;
			}
		}
	}
	return found;
}
int scanSingleCandidate(int(*A)[9], bool(*candidate)[9][10]) {
	int found = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				int count = 0;
				for (int k = 1; k <= 9; k++) {
					count += candidate[i][j][k];
				}
				if (count == 1) {
					int n = 1;
					while (candidate[i][j][n] != 1) n++;
					A[i][j] = n;
					candidate[i][j][n] = false;
					found++;
				}
			}
		}
	}
	return found;
}