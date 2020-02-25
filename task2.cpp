#include <stdio.h>

#include <iostream>
#include <algorithm>

using namespace std;

const int ny = 5; // –азмер массива по первому измерению
const int nx = 4; // –азмер массива по второму измерению

// јнализ расположени€ в пам€ти одномерного массива arr из n элементов
template<class Type>
void analyze_arr(int n, Type arr[]) {
	// ќпредел€ем размер минимального чанка, отводимого под элемент массива
	int min_chunk = 1000;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (abs(&arr[i] - &arr[j]) < min_chunk)
				min_chunk = abs(&arr[i] - &arr[j]);
	cout << "Probable minimal memory block size for array item = " << min_chunk*sizeof(Type) << endl;

	// ѕровер€ем гипотезу о линейном пор€дке выделени€ пам€ти
	bool seq_line = true;
	bool seq_revline = false;
	for (int i = 0; seq_line && i < n - 1; i++)
		if (&arr[i] > &arr[i+1])
			seq_line = false;
	if (seq_line)
		cout << "linear scheme" << endl;
	else {
		// ѕровер€ем гипотезу об обратном линейном пор€дке выделени€ пам€ти
		seq_revline = true;
		for (int i = 0; seq_revline && i < n - 1; i++)
			if (&arr[i] < &arr[i+1])
				seq_revline = false;

		if (seq_revline)
			cout << "linear reverse scheme" << endl;
		else {
			// ѕровер€ем гипотезу о перемешивании в линейном блоке
			bool in_block = true;
			// »щем минимальную и максимальную границы
			int imin = 0;
			int imax = 0;
			for (int i = 1; i < n; i++) {
				if (&arr[i] < &arr[imin])
					imin = i;
				if (&arr[i] > &arr[imax])
					imax = i;
			}
			int vars_size = n;
			if (&arr[imax] - &arr[imin] == vars_size - 1)
				cout << "in one block, packed and permutated!" << endl;
			else
				cout << "some non-linear scheme, may be mysterious optimizations?" << endl;

			return;
		}
	}
	// ѕровер€ем гипотезу о последовательном расположении с упаковкой
	bool seq_pack = true;
	for (int i = 0; seq_pack && i < n - 1; i++)
		if (seq_line && (&arr[i+1] - &arr[i]) != 1 || seq_revline && (&arr[i] - &arr[i+1]) != 1)
			seq_pack = false;
	if (seq_pack)
		cout << "packed!" << endl;
	else {
		// ѕровер€ем гипотезу о последовательном расположении без упаковки
		bool seq_no_pack = true;
		for (int i = 0; seq_no_pack && i < n - 1; i++)
			if (abs(&arr[i+1] - &arr[i]) % 2 != 0)
				seq_no_pack = false;
		if (seq_no_pack)
			cout << "aligned probably by " << min_chunk << endl;
		else
			cout << "can't understand, packed or aligned!" << endl;
	}
}

// јнализ расположени€ в пам€ти двумерного массива, представленного указател€ми на строки rows и на столбцы cols
void analyze_arr_2d(int ny, int nx, char * rows[], char * cols[]) {
	// ќпредел€ем принцип расположени€ -- по строкам или по столбцам
	bool by_row = cols[1] - cols[0] < rows[1] - rows[0];
	if (by_row) {
		cout << "probably lies by row" << endl;
		cout << "by row:" << endl;
		analyze_arr(nx, rows[0]);
	} else {
		cout << "probably lies by column" << endl;
		cout << "by column:" << endl;
		analyze_arr(ny, cols[0]);
	}
}

int main() {
        char one_dim_static[nx] = { 0 };

	cout << "ONE DIMENSIONAL STATIC:" << endl;
	analyze_arr(nx, one_dim_static);

        char * one_dim_dynamic = new char[nx];

	cout << "ONE DIMENSIONAL DYNAMIC:" << endl;
	analyze_arr(nx, one_dim_dynamic);

	char two_dim_static[ny][nx] = { 0 };
	char * rows[ny] = { NULL };
	char * cols[nx] = { NULL };
	int i;

	for (i = 0; i < ny; i++)
		rows[i] = &two_dim_static[i][0];
	for (i = 0; i < nx; i++)
		cols[i] = &two_dim_static[0][i];

	cout << "TWO DIMENSIONAL STATIC:" << endl;
	analyze_arr_2d(ny, nx, rows, cols);

	// —оздаем двумерный динамический массив как массив указателей на строки
	char ** by_row_2d = new char * [ny];
	for (i = 0; i < ny; i++)
		by_row_2d[i] = new char[nx];

	for (i = 0; i < ny; i++)
		rows[i] = &(by_row_2d[i][0]);
	for (i = 0; i < nx; i++)
		cols[i] = &(by_row_2d[0][i]);

	cout << "TWO DIMENSIONAL DYNAMIC BY ROW:" << endl;
	cout << "  ROWS ARRAY:" << endl;
	analyze_arr(ny, by_row_2d);
	cout << "  ARRAY BODY:" << endl;
	analyze_arr_2d(ny, nx, rows, cols);

	getchar(); // ∆дем нажати€ Enter

	return 0;
}