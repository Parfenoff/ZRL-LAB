#include <stdio.h>

#include <iostream>
#include <algorithm>

using namespace std;

const int n_globals = 5; // „исло глобальных переменных

// √лобальные переменные
int global_1;
short global_2;
char global_3;
char global_4;
char global_5;

// јнализ стратегии выделени€ пам€ти под n переменных с адресами ptrs, с размерами sizes
void analyze_ptrs(int n, char * ptrs[], int sizes[]) {
	// ќпредел€ем размер минимального чанка, отводимого под переменную
	int min_chunk = 1000;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (abs(ptrs[i] - ptrs[j]) < min_chunk)
				min_chunk = abs(ptrs[i] - ptrs[j]);
	cout << "Probable minimal memory block size for variable = " << min_chunk << endl;

	// ѕровер€ем гипотезу о линейном пор€дке выделени€ пам€ти
	bool seq_line = true;
	bool seq_revline = false;
	for (int i = 0; seq_line && i < n - 1; i++)
		if (ptrs[i] > ptrs[i+1])
			seq_line = false;
	if (seq_line)
		cout << "linear scheme" << endl;
	else {
		// ѕровер€ем гипотезу об обратном линейном пор€дке выделени€ пам€ти
		seq_revline = true;
		for (int i = 0; seq_revline && i < n - 1; i++)
			if (ptrs[i] < ptrs[i+1])
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
				if (ptrs[i] < ptrs[imin])
					imin = i;
				if (ptrs[i] > ptrs[imax])
					imax = i;
			}
			int vars_size = 0;
			for (int i = 0; i < n; i++)
				vars_size += sizes[i];
			if (ptrs[imax] - ptrs[imin] == vars_size - sizes[imax])
				cout << "in one block, packed and permutated!" << endl;
			else
				cout << "some non-linear scheme, may be mysterious optimizations?" << endl;

			return;
		}
	}
	// ѕровер€ем гипотезу о последовательном расположении с упаковкой
	bool seq_pack = true;
	for (int i = 0; seq_pack && i < n - 1; i++)
		if (seq_line && (ptrs[i+1] - ptrs[i]) != sizes[i] || seq_revline && (ptrs[i] - ptrs[i+1]) != sizes[i+1])
			seq_pack = false;
	if (seq_pack)
		cout << "packed!" << endl;
	else {
		// ѕровер€ем гипотезу о последовательном расположении без упаковки
		bool seq_no_pack = true;
		for (int i = 0; seq_no_pack && i < n - 1; i++)
			if (abs(ptrs[i+1] - ptrs[i]) % 2 != 0)
				seq_no_pack = false;
		if (seq_no_pack)
			cout << "aligned probably by " << min_chunk << endl;
		else
			cout << "can't understand, packed or aligned!" << endl;
	}
}

int main() {
	// ћассив указателей на глобальные переменные
	char * gptrs[n_globals] = {
		reinterpret_cast<char *>(&global_1),
		reinterpret_cast<char *>(&global_2),
		reinterpret_cast<char *>(&global_3),
		reinterpret_cast<char *>(&global_4),
		reinterpret_cast<char *>(&global_5)
	};
	// ћассив размеров глобальных переменных
	int gsizes[n_globals] = {
		sizeof(global_1),
		sizeof(global_2),
		sizeof(global_3),
		sizeof(global_4),
		sizeof(global_5)
	};

	cout << "GLOBALS:" << endl;
	analyze_ptrs(n_globals, gptrs, gsizes);

	int i;

	const int n_locals = 5; // „исло локальных переменных

	// .Ћокальные переменные
	int local_1 = 1;
	short local_2 = 2;
	char local_3 = 3;
	char local_4 = 4;
	char local_5 = 5;

	cout << "To prevent some optimizations we print this: ";
	cout << reinterpret_cast<void *>(&local_1) <<
		reinterpret_cast<void *>(&local_2) <<
		reinterpret_cast<void *>(&local_3) <<
		reinterpret_cast<void *>(&local_4) <<
		reinterpret_cast<void *>(&local_5) << endl;

	// ћассив указателей на глобальные переменные
	char * lptrs[n_locals] = {
		reinterpret_cast<char *>(&local_1),
		reinterpret_cast<char *>(&local_2),
		reinterpret_cast<char *>(&local_3),
		reinterpret_cast<char *>(&local_4),
		reinterpret_cast<char *>(&local_5)
	};
	// ћассив размеров локальных переменных
	int lsizes[n_locals] = {
		sizeof(local_1),
		sizeof(local_2),
		sizeof(local_3),
		sizeof(local_4),
		sizeof(local_5)
	};

	cout << "AUTOS:" << endl;
	analyze_ptrs(n_locals, lptrs, lsizes);

	const int n_dyns = 5; // „исло локальных переменных

	// .Ћокальные переменные
	int * dyn_1 = new int;
	short * dyn_2 = new short;
	char * dyn_3 = new char;
	char * dyn_4 = new char;
	char * dyn_5 = new char;

	// ћассив указателей на глобальные переменные
	char * dptrs[n_dyns] = {
		reinterpret_cast<char *>(dyn_1),
		reinterpret_cast<char *>(dyn_2),
		reinterpret_cast<char *>(dyn_3),
		reinterpret_cast<char *>(dyn_4),
		reinterpret_cast<char *>(dyn_5)
	};
	// ћассив размеров локальных переменных
	int dsizes[n_dyns] = {
		sizeof(*dyn_1),
		sizeof(*dyn_2),
		sizeof(*dyn_3),
		sizeof(*dyn_4),
		sizeof(*dyn_5)
	};

	cout << "DYNAMICS:" << endl;
	analyze_ptrs(n_dyns, dptrs, dsizes);

	// ќпредел€ем последовательность областей переменных в пам€ти
	char * mems[3] = { "GLOBALS", "AUTOS", "DYNAMICS" };
	char * mptrs[3] = { gptrs[0], lptrs[0], dptrs[0] };
	if (mptrs[0] > mptrs[1]) {
		swap(mems[0], mems[1]);
		swap(mptrs[0], mptrs[1]);
	}
	if (mptrs[1] > mptrs[2]) {
		swap(mems[1], mems[2]);
		swap(mptrs[1], mptrs[2]);
	}
	if (mptrs[0] > mptrs[1]) {
		swap(mems[0], mems[1]);
		swap(mptrs[0], mptrs[1]);
	}
	cout << "Sequence in memory: ";
	for (i = 0; i < 3; i++)
		cout << mems[i] << " ";
	cout << endl;

	getchar(); // ∆дем нажати€ Enter

	return 0;
}