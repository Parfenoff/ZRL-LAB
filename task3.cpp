#include <stdio.h>

#include <iostream>

using namespace std;

// јнализ стратегии выделени€ пам€ти под структуру из n полей с адресами ptrs, с размерами sizes, с общим размером total_size
void analyze_ptrs(int n, int total_size, char * ptrs[], int sizes[]) {
	// ќпредел€ем размер минимального чанка, отводимого под переменную
	int min_chunk = total_size - (ptrs[n-1] - ptrs[0]); // –азмер последнего чанка

	cout << "sizeof() fields sizes = [";
	for (int i = 0; i < n; i++)
		cout << sizes[i] << " ";
	cout << "]" << endl;

	cout << "Real fields sizes = [";
	for (int i = 0; i < n - 1; i++)
		cout << (ptrs[i+1] - ptrs[i]) << " ";
	cout << min_chunk << "]" << endl;

	for (int i = 0; i < n - 1; i++)
		if (ptrs[i+1] - ptrs[i] < min_chunk)
			min_chunk = ptrs[i+1] - ptrs[i];
	cout << "Probable minimal memory block size for field = " << min_chunk << endl;

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
			cout << "some non-linear scheme, may be mysterious optimizations?" << endl;

			return;
		}
	}
	// ѕровер€ем гипотезу о последовательном расположении с упаковкой
	int struct_size = 0;
	for (int i = 0; i < n; i++)
		struct_size += sizes[i];
	if (total_size == struct_size)
		cout << "packed!" << endl;
	else
		cout << "aligned probably by " << min_chunk << endl;

	cout << "Summar fields size = " << struct_size << endl;
	cout << "Real (sizeof) size = " << total_size << endl;
}

const int n_fields = 4;

struct {
	long long telephone;
	int birth_year;
	short birth_month;
	unsigned char birth_day;
} Human_unpack;

#pragma pack(push, 1)
struct {
	long long telephone;
	int birth_year;
	short birth_month;
	unsigned char birth_day;
} Human_pack;
#pragma pack(pop)

struct {
	unsigned char birth_day;
	long long telephone;
	short birth_month;
	int birth_year;
} Human_unpack_v2;

#pragma pack(push, 1)
struct {
	unsigned char birth_day;
	long long telephone;
	short birth_month;
	int birth_year;
} Human_pack_v2;
#pragma pack(pop)

int main() {
	// ћассив указателей на пол€ упакованной структуры
	char * pptrs[n_fields] = {
		reinterpret_cast<char *>(&Human_pack.telephone),
		reinterpret_cast<char *>(&Human_pack.birth_year),
		reinterpret_cast<char *>(&Human_pack.birth_month),
		reinterpret_cast<char *>(&Human_pack.birth_day)
	};
	// ћассив размеров полей упакованной структуры
	int psizes[n_fields] = {
		sizeof(Human_pack.telephone),  
		sizeof(Human_pack.birth_year), 
		sizeof(Human_pack.birth_month),
		sizeof(Human_pack.birth_day)   
	};

	cout << "PACKED:" << endl;
	analyze_ptrs(n_fields, sizeof(Human_pack), pptrs, psizes);

	// ћассив указателей на пол€ упакованной структуры
	char * uptrs[n_fields] = {
		reinterpret_cast<char *>(&Human_unpack.telephone),
		reinterpret_cast<char *>(&Human_unpack.birth_year),
		reinterpret_cast<char *>(&Human_unpack.birth_month),
		reinterpret_cast<char *>(&Human_unpack.birth_day)
	};
	// ћассив размеров полей упакованной структуры
	int usizes[n_fields] = {
		sizeof(Human_unpack.telephone),  
		sizeof(Human_unpack.birth_year), 
		sizeof(Human_unpack.birth_month),
		sizeof(Human_unpack.birth_day)   
	};

	cout << "NON-PACKED:" << endl;
	analyze_ptrs(n_fields, sizeof(Human_unpack), uptrs, usizes);

	// ћассив указателей на пол€ упакованной структуры
	char * mpptrs[n_fields] = {
		reinterpret_cast<char *>(&Human_pack_v2.birth_day),
		reinterpret_cast<char *>(&Human_pack_v2.telephone),
		reinterpret_cast<char *>(&Human_pack_v2.birth_month),
		reinterpret_cast<char *>(&Human_pack_v2.birth_year)
	};
	// ћассив размеров полей упакованной структуры
	int mpsizes[n_fields] = {
		sizeof(Human_pack_v2.birth_day),
		sizeof(Human_pack_v2.telephone),  
		sizeof(Human_pack_v2.birth_month),
		sizeof(Human_pack_v2.birth_year) 
	};

	cout << "PACKED (another fields order):" << endl;
	analyze_ptrs(n_fields, sizeof(Human_pack_v2), mpptrs, mpsizes);

	// ћассив указателей на пол€ упакованной структуры
	char * muptrs[n_fields] = {
		reinterpret_cast<char *>(&Human_unpack_v2.birth_day),
		reinterpret_cast<char *>(&Human_unpack_v2.telephone),
		reinterpret_cast<char *>(&Human_unpack_v2.birth_month),
		reinterpret_cast<char *>(&Human_unpack_v2.birth_year)
	};
	// ћассив размеров полей упакованной структуры
	int musizes[n_fields] = {
		sizeof(Human_unpack_v2.birth_day),
		sizeof(Human_unpack_v2.telephone),  
		sizeof(Human_unpack_v2.birth_month),
		sizeof(Human_unpack_v2.birth_year) 
	};

	cout << "NON-PACKED (another fields order):" << endl;
	analyze_ptrs(n_fields, sizeof(Human_unpack_v2), muptrs, musizes);

	getchar(); // ∆дем нажати€ Enter

	return 0;
}