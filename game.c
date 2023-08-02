#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void printGrid(int** grid, int rows, int cols) {
    system("cls"); 
    for (int i = 0; i < rows; i++) { // itera pela grid
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 0) {
                printf("~"); // célula morta
            } else {
                // código de escape ANSI para definir # para verde
                printf("\x1B[32m#"); 
                // redefinindo de volta para o padrão  (aparatemente isso é um bom costume)
                printf("\x1B[0m");
            }
        }
        printf("\n");
    }
}

void calculateNextGeneration(int** grid, int rows, int cols) {
    // alocando memória para nova matriz 
    int** nextGrid = (int**)malloc(rows * sizeof(int*)); // alocando um array de ponteiros para representar linhas da matriz
    for (int i = 0; i < rows; i++) {
        nextGrid[i] = (int*)malloc(cols * sizeof(int)); // alocando um array de ponteiros para cada linha da matriz
    }

    int liveCells = 0;

    for (int i_row = 0; i_row < rows; i_row++) {
        for (int j_col = 0; j_col < cols; j_col++) {
            int neighbors = 0;

            // loop para verificar cada célula vizinha
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i_row + i >= 0 && i_row + i < rows && j_col + j >=0 && j_col + j < cols) { // verifica se está dentro da grade
                        if (grid[i_row + i][j_col + j] == 1) { // célula viva
                            neighbors++;
                        }
                    }
                }
            }

            // subtrai 1 do contador de vizinhos se a célula atual estiver viva (já que ela foi contada no loopa acima)
            if (grid[i_row][j_col] == 1) {
                neighbors--;
            }

            // aplicando as regras do jogo da vida
            if (grid[i_row][j_col] == 1 && (neighbors < 2 || neighbors > 3)) {
                nextGrid[i_row][j_col] = 0; // célula morre por solidão ou superpopulação
            } else if (grid[i_row][j_col] == 0 && neighbors == 3) {
                nextGrid[i_row][j_col] = 1; // Uma célula morta com exatamente 3 vizinhos vivos
            } else {
                nextGrid[i_row][j_col] = grid[i_row][j_col]; // o estado da célula permanece o mesmo
            }

            if (nextGrid[i_row][j_col] == 1) {
                liveCells++; // conta as células vivas
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        memcpy(grid[i], nextGrid[i], cols * sizeof(int)); // copia os valores de nextGrid para matriz grid
    }

    for (int i = 0; i < rows; i++) { // liberando memória alocada para cada linha
        free(nextGrid[i]);
    }
    free(nextGrid); // libera a memória do array de ponteiros
}

int main() {
    FILE *pFile; // ponteiro do arquivo
    char fileRow[100];
    char fileName[100];
    int rows, cols;
    int **grid; 
    int generations;

    printf("Type the name of the file: ");
    scanf("%s", &fileName);

    printf("How many generations? ");
    scanf("%d", &generations);
    
    pFile = fopen(fileName, "r"); // abre o arquivo no modo leitura

    // verifica arquivo
    if (pFile == NULL) {
        printf("File dooesn't exist or is corrupted");
        exit(1);
    } else {
        fgets(fileRow, sizeof(fileRow), pFile); // lê a primeira linha
        rows = atoi(fileRow); // converte ascii para int

        fgets(fileRow, sizeof(fileRow), pFile); //lê a segunda linha
        cols = atoi(fileRow); // converte ascii para int

        // alocação de memória para a grade
        grid = (int **)malloc(rows * sizeof(int *)); // alocado ponteiro de linhas
        for (int i = 0; i < rows; i++) {
            grid[i] = (int *)malloc(cols * sizeof(int)); 
        }

        // preenchimento da grade a partir do arquivo
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int value;
                fscanf(pFile, "%1d", &value); // lê 1 digito de cada vez
                grid[i][j] = value;
            }
            fscanf(pFile, "\n"); // avança para a próxima linha
        }
        fclose(pFile);
    }

    for (int gen = 0; gen < generations; gen++) {
        printf("\n");
        printf("Geracao %d: \n", gen);
        printGrid(grid, rows, cols);
        calculateNextGeneration(grid, rows, cols);

        // contagem de células vivas
        int liveCells = 0; // Reset do contador de células vivas vira uma célula viva
        for (int i_row = 0; i_row < rows; i_row++) {
            for (int j_col = 0; j_col < cols; j_col++) {
                if (grid[i_row][j_col] == 1) {
                    liveCells++;
                }
            }
        }
        printf("Celulas vivas: %d\n", liveCells);
        Sleep(300);
    }

    // Libera a memória alocada para a matriz grid
    for (int i = 0; i < rows; i++) { //libera cada linha individualmente
        free(grid[i]);
    }
    free(grid); // ponteiro principal é liberado

    return 0;
}
