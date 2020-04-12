#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

typedef struct QueenPosition {
    int row;
    int column;
};

/* Display the matrix. */
void printMatrix(int** S, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << S[i][j] << " ";
		}
		cout << endl;
	}
}

/* Display an array */
void printArray(int* S, int n) {
    for (int i = 0; i < n; ++i)
	{
        cout << S[i] << " ";
	}
	cout << endl;
}

/* Initialize the items with 0. */
void initWithZeros(int ** matrix, int n) {
    for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = 0;
		}
	}
}

bool isAttacked(int** S, QueenPosition Q, int n) {
    int size = 2 * n - 1;
	int* sdiag = new int[size];
    int* mdiag = new int[size];

    // initialize with zeros
	for (int i = 0; i < size; i++) {
		sdiag[i] = 0;
		mdiag[i] = 0;
	}

    int g = 0, k = 0, i = 0, j = 0;
    for (k = 1 - n; k < n; k++) ///mdiag tömb meghatározása
	{
		int sum = 0;
		if (k < 0)
			for (j = 0; j < n + k; j++)
			{
				i = j - k;
				sum += S[i][j];
			}
		else
			for (i = 0; i < n - k; i++)
			{
				j = i + k;
				sum += S[i][j];
			}
		mdiag[g] = sum;
		g++;
	}

	cout << "Mdiag: " << endl;
    printArray(mdiag, size);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			k = i + j;
			sdiag[k] = sdiag[k] + S[i][j];
		}
	}

	cout << "Sdiag: " << endl;
    printArray(sdiag, size);

    int pos = Q.row + Q.column;
    return mdiag[pos] > 1 || sdiag[pos] > 1;
}

int getCountOfConflicts(int** S, int n) {
        int size = 2 * n - 1;
	int* sdiag = new int[size];
    int* mdiag = new int[size];

    // initialize with zeros
	for (int i = 0; i < size; i++) {
		sdiag[i] = 0;
		mdiag[i] = 0;
	}

    int g = 0, k = 0, i = 0, j = 0;
    for (k = 1 - n; k < n; k++) ///mdiag tömb meghatározása
	{
		int sum = 0;
		if (k < 0)
			for (j = 0; j < n + k; j++)
			{
				i = j - k;
				sum += S[i][j];
			}
		else
			for (i = 0; i < n - k; i++)
			{
				j = i + k;
				sum += S[i][j];
			}
		mdiag[g] = sum;
		g++;
	}

	int count2 = 0;
    for (i = 0; i < size; i++) {
        if (mdiag[i] > 1) {
            count2 += mdiag[i];
        }
    }

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			k = i + j;
			sdiag[k] = sdiag[k] + S[i][j];
		}
	}

	 for (i = 0; i < size; i++) {
        if (sdiag[i] > 1) {
            count2 += sdiag[i];
        }
    }

    return count2;
}

bool notIn(QueenPosition* Q, int index, int n, bool isOkZero) {
    if (index == 0 && isOkZero) {
        return true;
    }

    for (int i = 0; i < n; ++i) {
        if (Q[i].column == index || (index == 0 && !isOkZero)) {
            return false;
        }
    }

    return true;
}

/* Generate randomly the positions of the queens.
Put one queen in each row. */
void addQueens(int** S, int n, QueenPosition* Q)
{
	srand(time(0));
	bool isOkZero = true;
	for (int i = 0; i < n; ++i)
	{
        int column = rand() % n;
        while (!notIn(Q, column, n, isOkZero)) {
            column = rand() % n;
        }

        if (column == 0) {
            isOkZero = false;
        }

		S[i][column] = 1;
		QueenPosition q = {i, column};
		Q[i] = q;
	}
}

bool isOk(int** S, QueenPosition q1, QueenPosition q2, int conflictCount, int n) {
    int** S2 = new int*[n];
    for (int i = 0; i < n; i++) {
		S2[i] = new int[n];
	}

    for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			S2[i][j] = S[i][j];
		}
	}

	S2[q1.row][q1.column] = 0;
	S2[q2.row][q2.column] = 0;
	S2[q2.row][q1.column] = 1;
	S2[q1.row][q2.column] = 1;
	int conflicts = getCountOfConflicts(S2, n);

	return conflicts <= conflictCount;
}

void solveNQueenProblem(int** S, int n, QueenPosition* Q)
{
    int nswaps;
    do {
        nswaps = 0;
        int conflictCount = getCountOfConflicts(S, n);
        if (conflictCount == 0) {
            break;
        }
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if (isAttacked(S, Q[i], n) || isAttacked(S, Q[j], n)) {
                    if (isOk(S, Q[i], Q[j], conflictCount, n)) {
                        S[Q[i].row][Q[i].column] = 0;
                        S[Q[j].row][Q[i].column] = 1;
                        S[Q[j].row][Q[j].column] = 0;
                        S[Q[i].row][Q[j].column] = 1;
                        nswaps++;
                    }
                }
            }
        }
    } while (nswaps == 0);
}

int main()
{
	int n;
	cout << "Enter the size of the matrix:  ";
	cin >> n;
	int** S = new int*[n];
	for (int i = 0; i < n; i++) {
		S[i] = new int[n];
	}
    initWithZeros(S, n);

    QueenPosition* Q = new QueenPosition[n];
    addQueens(S, n, Q);
	printMatrix(S, n);

	solveNQueenProblem(S, n, Q);

	cout << "Solution: " << endl;
	printMatrix(S, n);
	return 0;
}
