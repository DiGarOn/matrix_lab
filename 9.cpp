//вар 3
/*
Корректность введенных данных в №1, пункт 3.
Проверка на одинаковую размерность для перемножения матриц
*/

#include <iostream>
#define ic_ std::cout<<"\x1b[92mline \x1b[94m"<<__LINE__<<"\x1b[92m in function \x1b[94m"<<__PRETTY_FUNCTION__<<"\n";
//для работы с файлами
#include <fstream>

using namespace std;

//структура matrix. ей хватает всего 2 параметра 
// side - сторона квадратной матрицы. грубо говоря - размерность 
// data - массив данных. задан **, тк используется двумерный массив
struct matrix {
	int side;
	double ** data;
};

//создание пустой матрицы для промежуточных действий 
matrix * init_empty_matrix(int); 
// функция задания матрицы сразу в нужном нам виде
matrix * init_matrix(int);
//функуия для зполнения матрицы из файла
matrix * init_matrix_file(ifstream *);
// функция вывода матрицы в файл
void print_matrix(matrix *, ofstream *);
//функция перемножения матриц
matrix * increase_matrix(matrix *, matrix *);

//функция нахождения обратной матрицы
matrix * reverse_matrix(matrix *);
//функция нахождения транспонированной матрицы
matrix * transpose_matrix(matrix * );
//функция нахождения определителя матрицы
int det_matrix(matrix *);
//функция для очистки памяти после использования вспомогательной переменной
void ClearMemory(matrix *);
//функция назоождения алгебраической подстановки 
matrix * AlgDop(matrix *);  

int main() {
	//открываем файл на чтений и читаем оттуда данные
	ifstream in;
	in.open("/Users/dmitriygarkin/Desktop/яп_лаб_2/задача 1/finput.txt");
	if(!(in.is_open())){
		cout << "file not find\n";
		exit(0);
	}
	int n;
	in >> n;
	//открываем файл на запись 
	ofstream file;
	file.open("/Users/dmitriygarkin/Desktop/яп_лаб_2/задача 1/fout.txt");
	if(!(file.is_open())){
		cout << "file not find\n";
		exit(0);
	}

	//создаем переменную типа matrix
	matrix* mat;
	mat = init_matrix(n);
	//выводится матрица в том виде, в котором она требуется условие 3го варианта
	file << "матрица в том виде, в котором она требуется условие 3го варианта"<<endl;
	print_matrix(mat, &file);
	//теперь запишем в нашу матрицу ее, умноженную на саму себя
	mat = increase_matrix(mat, mat);
	if(mat) {
		file << "теперь запишем в нашу матрицу ее, умноженную на саму себя"<<endl;
		print_matrix(mat, &file);
	} else {
		file << "эти матрицы нельзя перемножать";
	}
	//транспонируем ее
	mat = transpose_matrix(mat);
	file << "транспонируем ее"<<endl;
	print_matrix(mat, &file);
	//обратная матрица существует только тогда, когда определитель изначальной !=0
	mat = reverse_matrix(mat);
	if(mat) {
		print_matrix(mat, &file);
	} else {
		file << "не существует обратная\n";
	}
	//при пробных запусках было обнаружено, что определители матриц, 
	//которые задаются условием равны 0, потому я решил продемонстрировать
	// нахождение обратной матрицы на своем примере 
	matrix* mat_1;
	mat_1 = init_matrix_file(&in);
	file << "произвольная матрицы"<<endl;
	print_matrix(mat_1, &file);
	file << "определитель произвольной матрицы " << det_matrix(mat_1) << endl;
	mat_1 = reverse_matrix(mat_1);
	if(mat_1){
		file << "матрица, обратная произвольной"<<endl;
		print_matrix(mat_1, &file);
	} else{
		file << "матрица, обратная данной не существует\n";
	} 
	if(!mat){
		mat = init_matrix(n);
	}

	mat = increase_matrix(mat, mat_1);
	if(mat) {
		print_matrix(mat, &file);
	} else {
		file << "эти матрицы нельзя перемножать\n";
	}

	return 0;
}

matrix * init_empty_matrix(int n) {
	matrix * a;
	a = new matrix[1]; 
	a -> side = n;
	//создаем двументрый массив
	a -> data = new double*[n];
	for(int i = 0; i < n; i++) {
		a->data[i] = new double[n];
	} 
	return a;
}

matrix * init_matrix(int n) {
	if(n < 4 || n > 16) {
		cout << "incorrect data";
		exit(0);
	}
	//создаем переменную нужного нам типа
	matrix * a;
	a = new matrix[1]; 
	a -> side = n;
	//создаем двументрый массив
	a -> data = new double*[n];
	for(int i = 0; i < n; i++) {
		a->data[i] = new double[n];
	} 
	//заполняем его так, как сказано в условии 
	int s = 1;
	for(int i = 0; i < n; i++) {
		if(i % 2 == 0) {
			for(int j = 0; j < n; j++) {
				a->data[j][i] = s;
				s++;
			}
		} else {
			for(int j = n-1; j>=0; j--) {
				a->data[j][i] = s;
				s++;
			}
		}
	}
	return a;
}

matrix * init_matrix_file(ifstream * in) {
	int n;
	*in >> n;
	if(n < 4 || n > 16) {
		cout << "incorrect data file";
		exit(0);
	}
	matrix * a;
	a = new matrix[2];
	a->side = n;
	a->data = new double * [a->side];
	for(int i = 0; i < a->side; i++) {
		a->data[i] = new double[a->side];
		for(int j = 0; j < a->side; j++) {
			if(!(*in >> a->data[i][j])) {
				cout << "с матрицей что-то не так\n";
				exit(0);
			}
		}
	}
	return a;
}

//просто вывод матрицы в файл, ничего особенного 
void print_matrix(matrix * a, ofstream * file) {
	for(int i = 0; i < a->side; i++) {
		for(int j = 0; j < a->side; j++) {
			*file << a->data[i][j] << " ";
		}
		*file << endl;
	}
	*file << "_" << endl;
}

//перемножение матриц
matrix * increase_matrix(matrix *a, matrix *b) {
	if(a->side != b->side) {
		return 0;
	}
	//создаем новую переменную и записываем в нее результат 
	matrix * c = init_empty_matrix(a->side);
	//перемножение делается следующим образом - значение ячейки равно сумме
	//произведений элементов строки и столбца, пересечением которых является
	// эта ячейка
	for(int i = 0; i < b->side; i++) {
		for(int j = 0; j < b->side; j++) {
			int s = 0;
			for(int k = 0; k < b->side; k++) {
				s += a->data[i][k]*b->data[k][j];
			}
			c->data[i][j] = s;
		}
	}
	return c;
}

//транспонирование матрицы очень лего выполняется в представлении двумерного массива
matrix* transpose_matrix(matrix * b) {
	matrix * c = init_empty_matrix(b->side);
	for(int i = 0; i < b->side; i++) {
		for(int j = 0; j < b->side; j++) {
			c->data[i][j] = b->data[j][i]; 
		}
	}
	return c;	
}

//функция для очистки памяти(стоит щадить свои компьютеры даже при выполнении лаб)
void ClearMemory(matrix * a) {
	for(int i = 0; i < a->side; i++) {
		delete[] a->data[i];
	}
	delete[] a->data;
	delete[] a;
}

//нахождение определитея матрицы рекурсивны методом 
//говорим, что определитель матрицы - алгебраисческая сумма попеременных знаков 
//значений первой строки, умноженных на отпределители матриц, меньшего размера
//с помощью рекурсии смодим до очевидных вещей (типа размерности 2 и 1)
//размерость 1 по факту никогда не ипользуется, но написано, как для общего вида
int det_matrix(matrix * a) {
	if(a->side == 1){
		return a->data[0][0];
	} else if(a->side == 2){
		return a->data[0][0]*a->data[1][1]-a->data[0][1]*a->data[1][0];
	} else { 
		int res = 0;
		int one = 1;
		for (int k = 0; k < a->side; k++) {
            matrix* m = init_empty_matrix(a->side-1);
            //не рассматривается первая строка матрицы, тк оттуда всегда 
            //берутся элементы
            for (int i = 1; i < a->side; i++) {
                int t = 0;
                for (int j = 0; j < a->side; j++) {
                    if (j == k)
                        continue;
                    m->data[i-1][t] = a->data[i][j];
                    t++;
                }
            }
            res += one * a->data[0][k] * det_matrix(m);
            ClearMemory(m);
            one *= -1; 
        }
        return res;
	} 
}

//поиск алгебраического дополнения
//оно редставляет из себя матрицу, каждый элемент которой равен
//определителю транспонированной матрицы без соотв. столбца и строки
matrix * AlgDop(matrix * a) {
	//создаю переменную, которая будет результатом работы функции
	matrix * res = init_empty_matrix(a->side);
	//переменная для определения знака очередного слогаемого 
	int one = 1;
	for(int i = 0; i < a->side; i++) {
		for(int j = 0; j < a->side; j++) {
			//создаем промежуточную укороченную матрицу
			matrix * c = init_empty_matrix(a->side-1);
			//флаг для номера строк
			//создан по принципу: после того, как k и i сопадут нужно будет брать 
			//значение на 1 меньше, тк эти элементы не будут попадать в новую матрицу
			bool flag_k = false;
			for(int k = 0; k < a->side; k++) {
				bool flag_l = false;
				for(int l = 0; l < a->side; l++) {
					if(i == k) {
						flag_k = true;
						continue;
					}
					if(j == l) {
						flag_l = true;
						continue;
					}
					int new_k, new_l;
					new_k = flag_k? k-1: k;
					new_l = flag_l? l-1: l;
					c->data[new_k][new_l] = a->data[k][l];
				}
			}
			res->data[i][j] = one * det_matrix(c);
			one *= -1;
			ClearMemory(c);
		}
	}
	return res;
}

//нахождение обратной матрицы следующи способом:
//1) считается определитель начальной матрицы
//2) считаем, что он не равен 0 (это было проверено до вызова функции)
//3) транспонируем матрицу
//4) на основе транспонированной матрицы находим алгеброические дополнения
//5) каждый элемент новой матрицы делим на определитель 
matrix * reverse_matrix(matrix * a) {
	int det = det_matrix(a);
	if(det == 0) {
		return 0;
	}
	matrix * res = init_empty_matrix(a->side);
	res = transpose_matrix(a);
	res = AlgDop(res);
	for(int i = 0; i < a->side; i++) {
		for(int j = 0; j < a->side; j++) {
			res->data[i][j] /= det*1.0;
		}
	}
	return res;  	
}
