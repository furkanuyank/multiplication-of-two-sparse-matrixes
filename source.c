#include <stdio.h>
#include <stdlib.h>
#define MAX_TERM 101
#define _K 4
#define _L 4
#define _M 4

// Struct definition of sparse matrix.
typedef struct
{
    int row;
    int column;
    int value;
} non_zero_values;

// Prototypes of functions that are used.
void multiply(non_zero_values compact_matrix_1[], non_zero_values compact_matrix_2[], non_zero_values result_matrix[]); // O(n^2)
void transpose(non_zero_values *compact_matrix_2, non_zero_values *transposed_matrix);                                  // O(n^2)
void sort(non_zero_values matrix[]);                                                                                    // O(n^2)
void merge(non_zero_values matrix[]);                                                                                   // O(n^2)
void swap(non_zero_values matrix[], int x, int y);                                                                      // O(1)
void print(non_zero_values compact_matrix[]);                                                                           // O(n)

// Main function
int main()
{
    // Matrices will be multiplied.
    int sparse_matrix_1[_K][_L] = {{0, 10, 0, 5}, {0, 2, 4, 0}, {0, 0, 0, 0}, {3, 0, 0, 7}};
    int sparse_matrix_2[_L][_M] = {{1, 0, 0, 2}, {0, 0, 3, 0}, {0, 8, 0, 0}, {5, 0, 10, 0}};

    // Compact Matrix 1
    non_zero_values compact_matrix_1[MAX_TERM];

    // Transform sparse_matrix_1 to 1D array.
    int value_count = 1;
    compact_matrix_1[0].row = sizeof(sparse_matrix_1) / sizeof(sparse_matrix_1[0]);
    compact_matrix_1[0].column = sizeof(sparse_matrix_1[0]) / sizeof(sparse_matrix_1[0][0]);
    for (int i = 0; i < compact_matrix_1[0].row; i++) // iterates 0 to row size of matrix.
    {
        for (int j = 0; j < compact_matrix_1[0].column; j++) // iterates 0 to column size of matric for each row.
        {
            if (sparse_matrix_1[i][j] != 0) // execute if [i][j] index of matrix's value is not 0.
            {
                compact_matrix_1[value_count].row = i;
                compact_matrix_1[value_count].column = j;
                compact_matrix_1[value_count].value = sparse_matrix_1[i][j];
                value_count++;
            }
        }
    }
    compact_matrix_1[0].value = value_count - 1;

    // Compact Matrix 2
    non_zero_values compact_matrix_2[MAX_TERM];

    //  Transform sparse_matrix_2 to 1D array.
    value_count = 1;
    compact_matrix_2[0].row = sizeof(sparse_matrix_2) / sizeof(sparse_matrix_2[0]);
    compact_matrix_2[0].column = sizeof(sparse_matrix_2[0]) / sizeof(sparse_matrix_2[0][0]);
    for (int i = 0; i < compact_matrix_2[0].row; i++) // iterates 0 to row size of matrix.
    {
        for (int j = 0; j < compact_matrix_2[0].column; j++) // iterates 0 to column size of matric for each row.
        {
            if (sparse_matrix_2[i][j] != 0) // execute if [i][j] index of matrix's value is not 0.
            {
                compact_matrix_2[value_count].row = i;
                compact_matrix_2[value_count].column = j;
                compact_matrix_2[value_count].value = sparse_matrix_2[i][j];
                value_count++;
            }
        }
    }
    compact_matrix_2[0].value = value_count - 1;

    // Result matrix declaration.
    non_zero_values result_matrix[MAX_TERM];

    // Calculate multiplication of two sparse matrices and assign to matrix which is given by third parameter.
    multiply(compact_matrix_1, compact_matrix_2, result_matrix);

    // Prints result matrix of multiplication of two sparse matrices.
    print(result_matrix);

    // It makes prevent the program from closing immediately.
    printf("\nPress any key to exit...");
    getchar();
}

// Multiplication function
// Takes two matrices as parameters to be multiplied each others. complexity -> O(n^2)
void multiply(non_zero_values compact_matrix_1[], non_zero_values compact_matrix_2[], non_zero_values result_matrix[])
{
    // Transposes compact_matrix_2 and assigns to the transposed_matrix varaible.
    non_zero_values transposed_matrix[MAX_TERM];
    transpose(compact_matrix_2, transposed_matrix);

    // Multiplies non_zero_value matrices without sorting and merging. complexity-> O(n^2)
    result_matrix[0].row = compact_matrix_1[0].row;
    result_matrix[0].column = transposed_matrix[0].row;
    int quantity_of_non_zero_values = 0;
    int current_index = 1;
    for (int i = 1; i <= compact_matrix_1[0].value; i++) // iterates all non-zero values in first matrix.
    {
        for (int j = 1; j <= transposed_matrix[0].value; j++) // iterates all non-zero values in second(transposed) matrix for each non-zero values in first matrix.
        {
            if (compact_matrix_1[i].column == transposed_matrix[j].column) // executes if the columns of the first and second matrix are equal.
            {
                result_matrix[current_index].row = compact_matrix_1[i].row + 1;
                result_matrix[current_index].column = transposed_matrix[j].row + 1;
                result_matrix[current_index].value = (compact_matrix_1[i].value) * (transposed_matrix[j].value);
                current_index++;
                quantity_of_non_zero_values++;
            }
        }
    }
    result_matrix[0].value = quantity_of_non_zero_values;

    // Need to be sum the values that in the same indicies.
    merge(result_matrix);
}

// Applies the transpose operation to the matrix given as the first parameter and assign it to matrix as the second parameter. complexity-> O(n^2)
void transpose(non_zero_values *compact_matrix, non_zero_values *transposed_matrix)
{
    int n, i, j, currenti;
    n = compact_matrix[0].value;
    transposed_matrix[0].row = compact_matrix[0].column;
    transposed_matrix[0].column = compact_matrix[0].row;
    transposed_matrix[0].value = n;
    if (n > 0)
    {
        currenti = 1;
        for (i = 0; i < compact_matrix[0].column; i++)
        {
            for (j = 0; j <= n; j++)
            {
                if (compact_matrix[j].column == i)
                {
                    transposed_matrix[currenti].row = compact_matrix[j].column;
                    transposed_matrix[currenti].column = compact_matrix[j].row;
                    transposed_matrix[currenti].value = compact_matrix[j].value;
                    currenti++;
                }
            }
        }
    }
}

// Merges values at the same indices. complexity-> O(n^2)
void merge(non_zero_values matrix[])
{
    // Need to be sorted before merge the duplicate indices.
    sort(matrix);

    int merged_count = 0;
    for (int i = matrix[0].value; i >= 1; i--) // it iterates the the non-zero-values matrix backward.
    {
        int k = i;
        while (matrix[k].row == matrix[k - 1].row && matrix[k].column == matrix[k - 1].column) // it iterates as long as the subsequent indices is the same.
        {
            matrix[k - 1].value += matrix[k].value;
            matrix[k].row = -1;
            k--;
            merged_count++;
        }
        if (matrix[i].value == 0) // it eliminate the indices if the values become 0 after sum the values of duplicate indices.
        {
            matrix[k].row = -1;
            merged_count++;
        }
    }
    // Slide one by one the duplicate indices to the out of the bounds of the matrix.
    for (int i = 1; i < matrix[0].value; i++)
    {
        for (int j = 1; j <= matrix[0].value - i; j++)
        {
            if (matrix[j].row == -1)
            {
                swap(matrix, j, j + 1);
            }
        }
    }
    matrix[0].value -= merged_count;
}

// Sorts the indices in the matrix by row major order. Complexity-> O(n^2)
void sort(non_zero_values matrix[])
{
    for (int i = 1; i < matrix[0].value; i++)
    {
        for (int j = 1; j <= matrix[0].value - i + 1; j++)
        {
            if (matrix[j].row == matrix[j + 1].row && matrix[j].column > matrix[j + 1].column) // swaps the values if current and next matrices's rows and columns are equal.
            {
                swap(matrix, j, j + 1);
            }
        }
    }
}

// Swaps x and y in given matrix as a parameter.
void swap(non_zero_values matrix[], int x, int y)
{
    int temp_row = matrix[x].row;
    int temp_col = matrix[x].column;
    int temp_value = matrix[x].value;

    matrix[x].row = matrix[y].row;
    matrix[x].column = matrix[y].column;
    matrix[x].value = matrix[y].value;
    matrix[y].row = temp_row;
    matrix[y].column = temp_col;
    matrix[y].value = temp_value;
}

// Prints matrixs in non-zero format.
void print(non_zero_values compact_matrix[])
{
    printf("Sparse Matrix Multiplication:\nRow\tColumn\tValue\n");
    for (int i = 0; i <= compact_matrix[0].value; i++)
    {
        printf("%d\t %d\t %d \n", compact_matrix[i].row, compact_matrix[i].column, compact_matrix[i].value);
    }
}