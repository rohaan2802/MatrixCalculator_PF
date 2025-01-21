#include <iostream>
#include <iomanip>
//#define n 10         macros  ( Both const and Define can be used to initialize the value of n but not at a same time )
using namespace std;
double deter;
char ch;
const int n = 10;
void Input_Matrix(int[n][n], int);
void Output_Matrix(int[n][n], int);
void Symetric_Matrix(int[n][n], int);
void Identity_Matrix(int[n][n], int);
void Determinant_Matrix(int[n][n], int);
void Adjoint_Matrix(int[n][n], int);
void Inverse_Matrix(int[n][n], int);
void Matrix_solution(int[n][n], int);

int main()
{
New:
	int Matrix[n][n];
	int matrix_size, option;
	cout << "ENTER THE SIZE OF THE SQUARE MATRIX:" << endl; // size of Matrix
	cin >> matrix_size;
A:
	Input_Matrix(Matrix, matrix_size); // Calling function to take input of the Matrix	
	cout << "\n ================== \n";
cont:
	cout << "Choose any one of the following option and press the Number from 1-7 according to the task you want:" << endl;
	cout << "1. To display a matrix" << endl;
	cout << "2. To check whether the entered matrix is symmetric matrix or not" << endl;
	cout << "3. To check whether the entered matrix is identity matrix or not" << endl;
	cout << "4. To determine the determinant of entered matrix" << endl;
	cout << "5. To determine the adjoint of entered matrix" << endl;
	cout << "6. To determine the inverse of entered matrix" << endl;
	cout << "7. To determine the solution of entered matrix (i.e. solving multiple linear equations simultaneously)" << endl;
	cout << "8. To terminate the Program" << endl;
	cout << "\n \n \t \t \t Press any one of the options Given Above: >>>>>>>  ";
	cin >> option;
	switch (option)
	{
	case 1:
		Output_Matrix(Matrix, matrix_size);
		break;
	case 2:

		Symetric_Matrix(Matrix, matrix_size);
		break;
	case 3:
		Identity_Matrix(Matrix, matrix_size);
		break;
	case 4:
		Determinant_Matrix(Matrix, matrix_size);
		break;
	case 5:
		Adjoint_Matrix(Matrix, matrix_size);
		break;
	case 6:
		Inverse_Matrix(Matrix, matrix_size);
		break;
	case 7:
		Matrix_solution(Matrix, matrix_size);
		break;
	case 8:
		goto terminate;
		break;
	terminate:
		cout << "The Program is terminated....\n \n \t \t Thank You" << endl;
	}
	cout << "Do You want to Find Anything More:" << endl;
	cout << " \n " << endl;
	cout << " \t \t \t Press Y to Continue From Existing Program ..... OR ..... Press N to Start From New Program OR ..... Press 0 To Terminate .....\t >>>>>>>  ";
	cin >> ch;
	if (ch == 'Y' || ch == 'y')
	{
		goto cont;
	}
	if (ch == 'N' || ch == 'n')
		goto New;

	if (ch == '0')
		goto terminate;
	system("pause");
	return 0;
}

void Input_Matrix(int Matrix[n][n], int mat_size)   // Input FUnction
{
	cout << "\n\n=============================\n";
	cout << "  INPUT " << mat_size * mat_size << " VALUES INTO MATRIX\n";
	cout << "=============================\n\n";
	for (int i = 0; i < mat_size; i++)
	{
		for (int j = 0; j < mat_size; j++)
		{
			cout << "Enter value for Matrix[" << i + 1 << ", " << j + 1 << "]  >> ";
			cin >> Matrix[i][j];
		}
	}
}

void Output_Matrix(int Matrix[n][n], int mat_size)    // Output function to display matrix
{
	cout << "\n\n=======================\n";
	cout << "  DISPLAYING MATRIX\n";
	cout << "=======================\n\n\n";
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

void Symetric_Matrix(int Matrix[n][n], int mat_size)        // Function of symmetric Matrix
{
	cout << "\n\n=======================\n";
	cout << "  DISPLAYING THE ORIGINAL INPUT MATRIX\n";
	cout << "=======================\n\n\n";
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	cout << "\n\n=======================\n";
	cout << "  CHECK WHETHER IT IS SYMMETRIC MATRIX OR NOT\n";
	cout << "=======================\n\n\n";
	cout << " Transpose of a Matrix is........ \n \n";
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[j][i] << "  ";
		}
		cout << endl;
	}
	if (mat_size == 2)
	{
		if (Matrix[0][1] == Matrix[1][0])
		{
			cout << "AS TRANSPOSE OF A MATRIX = ORIGINAL INPUT MATRIX SO," << endl;
			cout << "IT IS SYMMETRIC MATRIX " << endl;
			cout << " \t \t \t ========================" << endl;
		}
		else
		{
			cout << "AS TRANSPOSE OF A MATRIX IS NOT EQUAL TO ORIGINAL INPUT MATRIX SO, " << endl;
			cout << "IT IS NOT SYMMETRIC MATRIX" << endl;
			cout << " \t \t \t ========================" << endl;
		}
	}
	if (mat_size == 3)
	{
		if ((Matrix[0][1] == Matrix[1][0]) && (Matrix[0][2] == Matrix[2][0]) && (Matrix[1][2] == Matrix[2][1]))
		{
			cout << "AS TRANSPOSE OF A MATRIX = ORIGINAL INPUT MATRIX SO," << endl;
			cout << "IT IS SYMMETRIC MATRIX " << endl;
			cout << " \t \t \t ========================" << endl;
		}
		else
		{
			cout << "AS TRANSPOSE OF A MATRIX IS NOT EQUAL TO ORIGINAL INPUT MATRIX SO," << endl;
			cout << "IT IS NOT SYMMETRIC MATRIX \n \n" << endl;
			cout << " \t \t \t ========================" << endl;
		}
	}
}

void Identity_Matrix(int Matrix[n][n], int mat_size)   // Function of Identity Matrix
{
	bool a;
	for (int i = 0; i < mat_size; i++)
	{
		for (int j = 0; j < mat_size; j++)
		{
			if (i == j && Matrix[i][j] != 1)
			{
				a = 1;
			}
			else	if (i != j && Matrix[i][j] != 0)
			{
				a = 1;

			}
		}
	}
	cout << "\n\n=======================\n";
	cout << "  DISPLAYING ORIGINAL INPUT MATRIX\n";
	cout << "=======================\n\n\n";
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	if (a == 0)
	{
		cout << "This is an Identity Matrix As All Enteries Of Its Principal Diagnol  Contains Entity 1" << endl;
	}
	else
		cout << "This is not an Identity Matrix As All Enteries Of Its Principal Diagnol doesnot Contains Entity 1" << endl;
}

void 	Determinant_Matrix(int Matrix[n][n], int mat_size)    // Function of Determinant of Matrix
{
	double deter;
	cout << " \t \t \t DISPLAYING ORIGINAL INPUT MATRIX \n " << endl;
	cout << " \t \t \t =================== \n " << endl;
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	// Calculating Determinant of 2 by 2 Matrix 
	if (mat_size == 2)
	{
		deter = (Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0]);
	}
	// Calculating Determinant of 3 by 3 Matrix
	if (mat_size == 3)
	{
		int x, y, z;
		x = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		y = ((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		z = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		deter = ((Matrix[0][0] * x) - (Matrix[0][1] * y) + (Matrix[0][2] * z));
	}
	// Display the Determinant
	cout << "Determinant of Matrix = " << deter << endl;
}

void Adjoint_Matrix(int Matrix[n][n], int mat_size)       // Function of Adjoint of a Matrix
{
	// Display the Original Input Matrix
	cout << " \t \t \t DISPLAYING ORIGINAL INPUT MATRIX \n " << endl;
	cout << " \t \t \t =================== \n " << endl;
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	// Calculating Adjoint of 2 by 2 Matrix
	if (mat_size == 2)
	{
		int a[2][2];
		a[0][0] = Matrix[0][0];
		a[1][1] = Matrix[1][1];
		Matrix[0][0] = a[1][1];
		Matrix[1][1] = a[0][0];
		Matrix[0][1] = -Matrix[0][1];
		Matrix[1][0] = -Matrix[1][0];
		// Display The Adjoint
		cout << "Adjoint of a matrix is" << endl;
		cout << Matrix[0][0] << "   " << Matrix[0][1] << "\n" << Matrix[1][0] << "   " << Matrix[1][1] << endl;
	}
	// Calculating Adjoint of 3 by 3 Matrix
	if (mat_size == 3)
	{
		int a[3][3];
		a[0][0] = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		a[0][1] = -((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		a[0][2] = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		a[1][0] = -((Matrix[0][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[0][2]));
		a[1][1] = ((Matrix[0][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[0][2]));
		a[1][2] = -((Matrix[0][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[0][1]));
		a[2][0] = ((Matrix[0][1] * Matrix[1][2]) - (Matrix[1][1] * Matrix[0][2]));
		a[2][1] = -((Matrix[0][0] * Matrix[1][2]) - (Matrix[1][0] * Matrix[0][2]));
		a[2][2] = ((Matrix[0][0] * Matrix[1][1]) - (Matrix[1][0] * Matrix[0][1]));
		// Calculaing Matrix
		Matrix[0][0] = a[0][0];     	Matrix[0][1] = a[1][0];    	Matrix[0][2] = a[2][0];
		Matrix[1][0] = a[0][1];     	Matrix[1][1] = a[1][1];    	Matrix[1][2] = a[2][1];
		Matrix[2][0] = a[0][2];     	Matrix[2][1] = a[1][2];    	Matrix[2][2] = a[2][2];
	}
	// Display the Adjoint 
	cout << "Adjoint of a Matrix is" << endl;
	cout << " \t \t \t =================== \n " << endl;
	cout << " " << Matrix[0][0] << "  " << Matrix[0][1] << "  " << Matrix[0][2] << endl;
	cout << " " << Matrix[1][0] << "  " << Matrix[1][1] << "  " << Matrix[1][2] << endl;
	cout << " " << Matrix[2][0] << "  " << Matrix[2][1] << "  " << Matrix[1][2] << endl;
}

void Inverse_Matrix(int Matrix[n][n], int mat_size)       // Fuction of Inverse of Matrix
{
	// Display original input Matrix
	cout << " \t \t \t DISPLAYING ORIGINAL INPUT MATRIX \n " << endl;
	cout << " \t \t \t =================== \n " << endl;
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	// Calculaing Inverse of 2 by 2 Matrix
	if (mat_size == 2)
	{
		int deter;
		// Calculating Determinant for 2 by 2 Matrix
		deter = (Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0]);
		// Calculating Adjoint for 2 by 2 Matrix
		int a[2][2];
		a[0][0] = Matrix[0][0];
		a[1][1] = Matrix[1][1];
		Matrix[0][0] = a[1][1];
		Matrix[1][1] = a[0][0];
		Matrix[0][1] = -Matrix[0][1];
		Matrix[1][0] = -Matrix[1][0];
		// Display Inverse of 2 by 2 Matrix
		if (deter != 0)
		{
			cout << "Inverse of a matrix is" << endl;
			cout << Matrix[0][0] / deter << "   " << Matrix[0][1] / deter << "\n" << Matrix[1][0] / deter << "   " << Matrix[1][1] / deter << endl;
		}
		else

			cout << "Determinat is zero so inverse cannot be find" << endl;
	}
	// Calculating Inverse of 3 by 3 Matrix
	if (mat_size == 3)
	{
		// Calculating Determinant of 3 by 3 Matrix
		int deter;
		int x, y, z;
		x = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		y = ((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		z = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		deter = ((Matrix[0][0] * x) - (Matrix[0][1] * y) + (Matrix[0][2] * z));
		// Calculating Adjoint for 3 by 3 Matrix
		int a[3][3];
		a[0][0] = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		a[0][1] = -((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		a[0][2] = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		a[1][0] = -((Matrix[0][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[0][2]));
		a[1][1] = ((Matrix[0][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[0][2]));
		a[1][2] = -((Matrix[0][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[0][1]));
		a[2][0] = ((Matrix[0][1] * Matrix[1][2]) - (Matrix[1][1] * Matrix[0][2]));
		a[2][1] = -((Matrix[0][0] * Matrix[1][2]) - (Matrix[1][0] * Matrix[0][2]));
		a[2][2] = ((Matrix[0][0] * Matrix[1][1]) - (Matrix[1][0] * Matrix[0][1]));
		// Calculaing Matrix
		Matrix[0][0] = a[0][0];     	Matrix[0][1] = a[1][0];    	Matrix[0][2] = a[2][0];
		Matrix[1][0] = a[0][1];     	Matrix[1][1] = a[1][1];    	Matrix[1][2] = a[2][1];
		Matrix[2][0] = a[0][2];     	Matrix[2][1] = a[1][2];    	Matrix[2][2] = a[2][2];
		// Display inverse of 3 by 3 Matrix
		if (deter != 0)
		{
			cout << "Inverse of a Matrix is" << endl;
			cout << " " << Matrix[0][0] / deter << "  " << Matrix[0][1] / deter << "  " << Matrix[0][2] / deter << endl;
			cout << " " << Matrix[1][0] / deter << "  " << Matrix[1][1] / deter << "  " << Matrix[1][2] / deter << endl;
			cout << " " << Matrix[2][0] / deter << "  " << Matrix[2][1] / deter << "  " << Matrix[2][2] / deter << endl;

		}
		else
			cout << "Determinat is zero so inverse cannot be find" << endl;
	}
}

void Matrix_solution(int Matrix[n][n], int mat_size)        // Function of solution of a Matrix
{
	// Display original input Matrix
	cout << " \t \t \t DISPLAYING ORIGINAL INPUT MATRIX \n " << endl;
	cout << " \t \t \t =================== \n " << endl;
	for (int i = 0; i < mat_size; i++)
	{
		cout << "\t";
		for (int j = 0; j < mat_size; j++)
		{
			cout << left << setw(5) << Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	// To find solution of equations By Matrix f 2 by 2
	if (mat_size == 2)
	{
		// Taking Input of Constant Matrix
		int arr[2][1];
		cout << "\n\n=============================\n";
		cout << "  INPUT " << mat_size * 1 << " VALUES INTO MATRIX\n";
		cout << "=============================\n\n";
		for (int i = 0; i < mat_size - 1; i++)
		{
			for (int j = 0; j < mat_size; j++)
			{
				cout << "Enter value for New Constant Matrix[" << i + 1 << ", " << j + 1 << "]  >> ";
				cin >> arr[i][j];
			}
		}
		cout << "=======================\n\n\n";
		for (int i = 0; i < mat_size; i++)
		{
			cout << "\t";
			for (int j = 0; j < mat_size - 1; j++)
			{
				cout << left << setw(5) << arr[i][j] << "  ";

			}
			cout << endl;
		}
		double deter;
		// Calculating Determinant for 2 by 2 Matrix
		deter = (Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0]);
		// Calculating Adjoint for 2 by 2 Matrix
		int a[2][2];
		a[0][0] = Matrix[0][0];
		a[1][1] = Matrix[1][1];
		Matrix[0][0] = a[1][1];
		Matrix[1][1] = a[0][0];
		Matrix[0][1] = -Matrix[0][1];
		Matrix[1][0] = -Matrix[1][0];
		// Display Solution of Matrix  of 2 by 2 Matrix
		if (deter != 0)
		{
			cout << "Solution of Equations of a matrix is" << endl;
			cout << ((Matrix[0][0] / deter) * arr[0][0]) + ((Matrix[0][1] / deter) * arr[0][1]) << "\n" << ((Matrix[1][0] / deter) * arr[0][0]) + ((Matrix[1][1] / deter) * arr[1][0]) << endl;
		}
		else
			cout << "Determinat is zero so Solution cannot be find" << endl;
	}
	// To find solution Matrix of 3 by 3 Matrix
	if (mat_size == 3)
	{
		// Taking Input of Constant Matrix
		int arr[3][1];
		cout << "\n\n=============================\n";
		cout << "  INPUT " << mat_size * 1 << " VALUES INTO MATRIX\n";
		cout << "=============================\n\n";
		for (int i = 0; i < mat_size - 2; i++)
		{
			for (int j = 0; j < mat_size; j++)
			{
				cout << "Enter value for New Constant Matrix[" << i + 1 << ", " << j + 1 << "]  >> ";
				cin >> arr[i][j];
			}
		}
		cout << "=======================\n\n\n";
		for (int i = 0; i < mat_size; i++)
		{
			cout << "\t";
			for (int j = 0; j < mat_size - 2; j++)
			{
				cout << left << setw(5) << arr[i][j] << "  ";

			}
			cout << endl;
		}
		// Determinant of 3 by 3 Matrix 
		int x, y, z;
		x = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		y = ((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		z = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		deter = ((Matrix[0][0] * x) - (Matrix[0][1] * y) + (Matrix[0][2] * z));
		// Adjoint of 3 by 3 Matrix
		int a[3][3];
		a[0][0] = ((Matrix[1][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[1][2]));
		a[0][1] = -((Matrix[1][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[1][2]));
		a[0][2] = ((Matrix[1][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[1][1]));
		a[1][0] = -((Matrix[0][1] * Matrix[2][2]) - (Matrix[2][1] * Matrix[0][2]));
		a[1][1] = ((Matrix[0][0] * Matrix[2][2]) - (Matrix[2][0] * Matrix[0][2]));
		a[1][2] = -((Matrix[0][0] * Matrix[2][1]) - (Matrix[2][0] * Matrix[0][1]));
		a[2][0] = ((Matrix[0][1] * Matrix[1][2]) - (Matrix[1][1] * Matrix[0][2]));
		a[2][1] = -((Matrix[0][0] * Matrix[1][2]) - (Matrix[1][0] * Matrix[0][2]));
		a[2][2] = ((Matrix[0][0] * Matrix[1][1]) - (Matrix[1][0] * Matrix[0][1]));
		// Calculaing Matrix
		Matrix[0][0] = a[0][0];     	Matrix[0][1] = a[1][0];    	Matrix[0][2] = a[2][0];
		Matrix[1][0] = a[0][1];     	Matrix[1][1] = a[1][1];    	Matrix[1][2] = a[2][1];
		Matrix[2][0] = a[0][2];     	Matrix[2][1] = a[1][2];    	Matrix[2][2] = a[2][2];
		// Display Solution of Matrix of 3 by 3 Matrix
		if (deter != 0)
		{
			cout << "Solution of Equations of a Matrix is" << endl;
			cout << " " << ((Matrix[0][0] / deter) * arr[0][0]) + ((Matrix[0][1] / deter) * arr[0][1]) + ((Matrix[0][2] / deter) * arr[0][2]) << endl;
			cout << " " << ((Matrix[1][0] / deter) * arr[0][0]) + ((Matrix[1][1] / deter) * arr[0][1]) + ((Matrix[1][2] / deter) * arr[0][2]) << endl;
			cout << " " << ((Matrix[2][0] / deter) * arr[0][0]) + ((Matrix[2][1] / deter) * arr[0][1]) + ((Matrix[2][2] / deter) * arr[0][2]) << endl;
		}
		else
			cout << "Determinat is zero so Solution cannot be find" << endl;
	}
}