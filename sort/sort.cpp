#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#define MSEC 500
#define SIZE 28

void gotoxy(int x, int y);
void printArray(int* A, int n);
void printGraph(int* A, int start, int end);
void swap(int* a, int* b);
void randomArray(int* A, int n);

void bubbleSort(int* A, int n);
void selectionSort(int* A, int n);
void insertionSort(int* A, int n);
void quickSort(int* A, int start, int end);
int* mergeSort(int* A, int start, int end);
int mergeSort2(int* A, int* temp, int start, int end);


int main() {
	int A[SIZE], temp[SIZE], temp2[SIZE];
	randomArray(A, SIZE);
	printGraph(A, 0, SIZE - 1);
	memcpy(temp, A, sizeof(A));

	char input;
	while ((input = _getch()) != 'x') {
		switch (input) {
		case'1':bubbleSort(A, SIZE); break;
		case'2':selectionSort(A, SIZE); break;
		case'3':insertionSort(A, SIZE); break;
		case'4':quickSort(A, 0, SIZE - 1); break;
		case'5':mergeSort(A, 0, SIZE - 1); break;
		case'6':mergeSort2(A, temp2, 0, SIZE - 1); break;
		case'r':
			memcpy(A, temp, sizeof(A));
			printGraph(A, 0, SIZE - 1);
			break;
		case' ':
			randomArray(A, SIZE);
			printGraph(A, 0, SIZE - 1);
			memcpy(temp, A, sizeof(A));
			break;
		}
	}
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void printArray(int* A, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d	", A[i]);
	}
	printf("\n");
}
void printGraph(int* A, int start, int end) {
	for (int j = start; j <= end; j++) {
		for (int i = 0; i < SIZE; i++) {
			gotoxy(6 + 4 * j, 28 - i);
			printf("¡¡");
		}
	}
	for (int j = start; j <= end; j++) {
		gotoxy(6 + 4 * j, 29);
		printf("%2d",A[j]);
		for (int i = 0; i < A[j]; i++) {
			gotoxy(6 + 4 * j, 28 - i);
			printf("¢Æ");
		}
	}
	gotoxy(0, 0);
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
void randomArray(int* A, int n) {
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		A[i] = i + 1;
	for (int i = 0; i < n - 1; i++) {
		int j = rand() % (n - i) + i;
		swap(&A[i], &A[j]);
	}
}

void bubbleSort(int* A, int n) {
	if (n == 1) return;
	bool flag = false;
	for (int i = 0; i < n - 1; i++)
		if (A[i] > A[i + 1]) {
			swap(&A[i], &A[i + 1]); 
			flag = true;
		}
	if (flag == false) return;
	printGraph(A, 0, SIZE - 1);
	Sleep(MSEC);
	bubbleSort(A, n - 1);
}

void selectionSort(int* A, int n) {
	if (n == 1) return;
	for (int i = 0; i < n - 1; i++)
		if (A[n - 1] < A[i]) swap(&A[n - 1], &A[i]);
	printGraph(A, 0, SIZE - 1);
	Sleep(MSEC);
	selectionSort(A, n - 1);
}

void insertionSort(int* A, int n) {
	if (n == 0) return;
	if (n == SIZE) return insertionSort(A, n - 1);;
	int k = n, temp = A[n - 1];
	while (A[n - 1] > A[k] && k < SIZE) k++;
	for (int i = n - 1; i < k - 1; i++)
		A[i] = A[i + 1];
	A[k - 1] = temp;
	printGraph(A, 0, SIZE - 1);
	Sleep(MSEC);
	insertionSort(A, n - 1);
}

void quickSort(int* A,int start,int end) {
	if (end - start <= 0) return;
	int i;
	for (i = start; i < end; i++) {
		if (A[i] > A[end]) {
			int j = i + 1;
			while (A[j] > A[end] && j < end) j++;
			if (j == end) break;
			swap(&A[i], &A[j]);
		}
	}
	swap(&A[i], &A[end]);
	printGraph(A, 0, SIZE - 1);
	Sleep(MSEC);
	quickSort(A, start, i - 1);
	quickSort(A, i + 1, end);
}

int* mergeSort(int* A, int start, int end) {
	int n = end - start + 1;
	int* subA = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		subA[i] = A[start + i];

	if (n == 1) return subA;

	int m = (start + end) / 2;
	int* L = mergeSort(A, start, m);
	int* R = mergeSort(A, m + 1, end);
	int nL = m - start + 1;
	int nR = end - m;

	int i = 0, j = 0, k = 0;
	while (j < nL && k < nR) {
		if (L[j] <= R[k]) {
			subA[i] = L[j];
			j++;
		}
		else {
			subA[i] = R[k];
			k++;
		}
		i++;
	}
	while (j < nL) {
		subA[i] = L[j];
		j++;
		i++;
	}
	while (k < nR) {
		subA[i] = R[k];
		k++;
		i++;
	}
	free(L);
	free(R);
	for (int i = 0; i < n; i++)
		A[start + i] = subA[i];
	printGraph(A, start, end);
	Sleep(MSEC);
	return subA;
}
int mergeSort2(int* A, int* temp, int start, int end) {
	int n = end - start + 1;
	if (n == 1) return start;

	int m = (start + end) / 2;
	int startL = mergeSort2(A, temp, start, m);
	int startR = mergeSort2(A, temp, m + 1, end);
	int nL = m - start + 1;
	int nR = end - m;

	int i = start, j = startL, k = startR;
	while (j < startL + nL && k < startR + nR) {
		if (A[j] <= A[k]) {
			temp[i] = A[j];
			j++;
		}
		else {
			temp[i] = A[k];
			k++;
		}
		i++;
	}
	while (j < startL + nL) {
		temp[i] = A[j];
		j++;
		i++;
	}
	while (k < startR + nR) {
		temp[i] = A[k];
		k++;
		i++;
	}
	for (int i = start; i <= end; i++)
		A[i] = temp[i];
	printGraph(A, start, end);
	Sleep(MSEC);
	return start;
}