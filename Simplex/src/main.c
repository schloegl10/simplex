#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

float aPadrao[2][5] = {
{8.0,	6.0,	1.0,	1.0, 0.0},
{2.0,   1.5,	0.5,	0.0, 1.0}
};

float bPadrao[2][1] = {{50}, {15}};

float cPadrao[5][1] = {{47}, {32}, {20}, {0}, {0}};

bool minimizar = false;

void printMatrix(int linhas, int colunas, float** matriz) {
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			printf("%f ", matriz[linha][coluna]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void printMatrixInt(int linhas, int colunas, int** matriz) {
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			printf("%d ", matriz[linha][coluna]);
		}
		printf("\n");
	}
}

float** criaMatriz(int linha, int coluna)
{
    float* values = calloc(linha*coluna, sizeof(float));
    float** linhas = malloc(linha*sizeof(float*));
    for (int i=0; i<linha; ++i)
    {
        linhas[i] = values + i*coluna;
    }
    return linhas;
}

int** criaMatrizInt(int linha, int coluna)
{
    int* values = calloc(linha*coluna, sizeof(int));
    int** linhas = malloc(linha*sizeof(int*));
    for (int i=0; i<linha; ++i)
    {
        linhas[i] = values + i*coluna;
    }
    return linhas;
}

float** criaMatrizSomaIdentidade(int linha, int coluna, float** matriz)
{
    float* values = calloc(linha*(coluna+linha), sizeof(float));
    float** linhas = malloc(linha*sizeof(float*));
    for (int i=0; i<linha; ++i) {
        linhas[i] = values + i*(coluna+linha);
		for(int c = 0; c < coluna+linha; c++) {
			if(c < coluna) {
				linhas[i][c] = matriz[i][c];
			} else if (c == coluna + i) {
				linhas[i][c] = 1.0;
			} else {
				linhas[i][c] = 0.0;
			}
		}
    }
    return linhas;
}

float** multiplicaMatriz(int linhasA, int colunasA, int linhasB, int colunasB, float** matrizA, float** matrizB) {
	float** matrizResultado = criaMatriz(linhasA, colunasB);
	float somaDosProdutos;
	int linha, coluna, indiceMult;
	if(colunasA == linhasB) {
		for(linha = 0; linha < linhasA; linha++) {
			for(coluna = 0; coluna < colunasB; coluna++) {
				somaDosProdutos = 0.0;
				for(indiceMult = 0; indiceMult < colunasA; indiceMult++) {
					somaDosProdutos += matrizA[linha][indiceMult] * matrizB[indiceMult][coluna];
				}
				
				matrizResultado[linha][coluna] = somaDosProdutos;
			}
		}
	} else {
		printf("Matrizez n??o podem ser multiplicadas");
	}
	return matrizResultado;
}

float** obtemTransposta(int linhas, int colunas, float** matriz) {
	float** matrizTransposta = criaMatriz(colunas, linhas);
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			matrizTransposta[coluna][linha] = matriz[linha][coluna];
		}
	}
	return matrizTransposta;
}

float** converteMatrizParaPointerMatriz(int linhas, int colunas, float matriz[linhas][colunas]) {
	float** matrizPointer = criaMatriz(linhas, colunas);
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			matrizPointer[linha][coluna] = matriz[linha][coluna];
		}
	}
	return matrizPointer;
}

float** duplicarMatriz(int linhas, int colunas, float** matriz) {
	float** matrizPointer = criaMatriz(linhas, colunas);
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			matrizPointer[linha][coluna] = matriz[linha][coluna];
		}
	}
	return matrizPointer;
}

int** duplicarMatrizInt(int linhas, int colunas, int** matriz) {
	int** matrizPointer = criaMatrizInt(linhas, colunas);
	int linha, coluna;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			matrizPointer[linha][coluna] = matriz[linha][coluna];
		}
	}
	return matrizPointer;
}

float** criaMatrizIdentidade(int dimensao) {
	float** matrizPointer = criaMatriz(dimensao, dimensao);
	int linha, coluna;
	for(linha = 0; linha < dimensao; linha++) {
		for(coluna = 0; coluna < dimensao; coluna++) {
			if(linha == coluna) {
				matrizPointer[linha][coluna] = 1;
			} else {
				matrizPointer[linha][coluna] = 0;
			}
		}
	}
	return matrizPointer;
}

int** criaIBFase1(int numeroRestricoes, int numeroVariaveis) {
	int** iB = criaMatrizInt(1, numeroRestricoes);
	for(int i = 0; i < numeroRestricoes; i++) {
		iB[0][i] = numeroVariaveis + i;
	}
	return iB;
}

float** obtemB(int dimensao, float** a, int** iB) {
	float** B = criaMatriz(dimensao, dimensao);
	for(int linha = 0; linha < dimensao; linha++) {
		for(int coluna = 0; coluna < dimensao; coluna++) {
			int indiceIB = iB[0][coluna];
			B[linha][coluna] = a[linha][indiceIB];
		}
	}
	return B;
}

void moveLinha(float** matriz, int linhaFinal, int linhaASerMovida, int dimensao) {
	float** tempColunaASerMovida = criaMatriz(1, dimensao);
	for(int i = 0; i < dimensao; i++) {
		tempColunaASerMovida[0][i] = matriz[linhaFinal][i];
	}
	for(int i = 0; i < dimensao; i++) {
		matriz[linhaFinal][i] = matriz[linhaASerMovida][i];
	}
	for(int i = 0; i < dimensao; i++) {
		matriz[linhaASerMovida][i] = tempColunaASerMovida[0][i];
	}
}

void dividePivoPorMult(float** matriz, int linha, int dimensao, float mult) {
	for(int i = 0; i < dimensao; i++) {
		matriz[linha][i] = matriz[linha][i] * mult;
	}
}

void subtraiLinhaPivoLinhasDiferenteZero(float** matriz, float** inversa, int pivo, int dimensao) {
	float mult;
	for(int l = 0; l < dimensao; l++) {
		if(l != pivo && matriz[l][pivo] != 0) {
			mult = matriz[l][pivo];
			for(int c = 0; c < dimensao; c++) {
				matriz[l][c] = matriz[l][c] - matriz[pivo][c] * mult;
				inversa[l][c] = inversa[l][c] - inversa[pivo][c] * mult;
			}
		}
	}
}

float** obtemInversa(int dimensao, float** matriz) {
	float** matriz2 = duplicarMatriz(dimensao,dimensao, matriz);
	float** matrizInversa = criaMatrizIdentidade(dimensao);
	for(int coluna = 0; coluna < dimensao; coluna++) {
		int linhaPivo = -1;
		for(int linha = coluna; linha < dimensao; linha++) {
			if(linhaPivo == -1 && matriz2[linha][coluna] != 0) {
				linhaPivo = coluna;
			}
		}
		if(linhaPivo != coluna &&linhaPivo != -1) {
			moveLinha(matriz2, coluna, linhaPivo, dimensao);
			moveLinha(matrizInversa, coluna, linhaPivo, dimensao);
		}
		float mult = 1/matriz2[coluna][coluna];
		dividePivoPorMult(matriz2, coluna, dimensao, mult);
		dividePivoPorMult(matrizInversa, coluna, dimensao, mult);
		subtraiLinhaPivoLinhasDiferenteZero(matriz2, matrizInversa, coluna, dimensao);
	}
	return matrizInversa;
}

bool validaXb(int dimensao, float** matriz) {
	bool valida = true;
	for(int i = 0; i < dimensao; i++) {
		if(matriz[i][0] < 0) {
			valida = false;
		}
	}
	return valida;
}

float** completaXb(int numeroRestricoes, int numeroVariaveis, float** Xb, int** iB) {
	float** XbCompleto = criaMatriz(numeroVariaveis, 1);
	int contadorIb = 0;
	while(contadorIb < numeroRestricoes) {
		for(int i = 0; i < numeroVariaveis; i++) {
			if(i == iB[0][contadorIb]) {
				XbCompleto[i][0] = Xb[contadorIb][0];
				contadorIb++;	
			} else {
				XbCompleto[i][0] = 0;
			}
		}
	}
	return XbCompleto;
}

float** criaCFase1(int numeroVariaveis, int numeroRestricoes) {
	float** c = criaMatriz(numeroVariaveis + numeroRestricoes, 1);
	for(int i = 0; i < numeroVariaveis + numeroRestricoes; i++) {
		if(i < numeroVariaveis) {
			c[i][0] = 0;
		} else {
			c[i][0] = 1;
		}
	}
	return c;
}

void ordenaLista(int dimensao, int** lista) {
	int a;
	for (int i = 0; i < dimensao; ++i) {
    	for (int j = i + 1; j < dimensao; ++j) {
            if (lista[0][i] > lista[0][j]) {
                a =  lista[0][i];
                lista[0][i] = lista[0][j];
                lista[0][j] = a;
            }
        }
    }
}

float** obtemN(int numeroVariaveis, int numeroRestricoes, float** a, int** iB) {
	float** N = criaMatriz(numeroRestricoes, numeroVariaveis - numeroRestricoes);
	int contadorIb = 0;
	int contadorN = 0;
	for(int i = 0; i < numeroVariaveis; i++) {
		if(i != iB[0][contadorIb]) {
			for(int c = 0; c < numeroRestricoes; c++) {
				N[c][contadorN] = a[c][i];
			}
			contadorN++;
		} else {
			contadorIb++;
		}
	}
	return N;
}

float** obtemCTB(int numeroRestricoes, int numeroVariaveis, float** cT, int** iB) {
	float** cTB = criaMatriz(1, numeroRestricoes);
	int contadorIb = 0;
	int contadorB = 0;
	for(int i = 0; i < numeroVariaveis; i++) {
		if(i == iB[0][contadorIb]) {
			cTB[0][contadorB] = cT[0][i];
			contadorB++;
			contadorIb++;
		}
	}
	return cTB;
}

float** obtemCTN(int numeroRestricoes, int numeroVariaveis, float** cT, int** iB) {
	float** cTN = criaMatriz(1, numeroVariaveis - numeroRestricoes);
	int contadorIb = 0;
	int contadorN = 0;
	for(int i = 0; i < numeroVariaveis; i++) {
		if(i != iB[0][contadorIb]) {
			cTN[0][contadorN] = cT[0][i];
			contadorN++;
		} else {
			contadorIb++;
		}
	}
	return cTN;
}

float** subTraiMatriz(int linhas, int colunas, float** matrizA, float** matrizB) {
	float** matrizResultado = criaMatriz(linhas,colunas);
	for(int l = 0; l < linhas; l++) {
		for(int c = 0; c < colunas; c++) {
			matrizResultado[l][c] = matrizA[l][c] - matrizB[l][c];
		}
	}
	return matrizResultado;
}

int obtemCustoReduzidoMelhor(int dimensao, float** custoReduzido, bool min) {
	int index = -1;
	for(int i = 0; i < dimensao; i++) {
		if((min == true && custoReduzido[0][i] < 0) || (min == false && custoReduzido[0][i] > 0)) {
			index = i;
			break;
		}
	}
	return index;
}

int** obtemIN(int numeroRestricoes, int numeroVariaveis, int** iB) {
	int** iN = criaMatrizInt(1, numeroVariaveis - numeroRestricoes);
	int contadorIb = 0;
	int contadorIN = 0;
	for(int i = 0; i < numeroVariaveis; i++) {
		if(i != iB[0][contadorIb]) {
			iN[0][contadorIN] = i;
			contadorIN++;
		} else {
			contadorIb++;
		}
	}
	return iN;
}

int obtemMenorRestricao(int numeroRestricoes, int index, float** BinvN, float** XbIncompleto) {
	float menorRestricao = -1;
	int indexMenorRestricao = -1;
	for(int i = 0; i < numeroRestricoes; i++) {
		if(indexMenorRestricao == -1 && BinvN[i][index] > 0) {
			menorRestricao = XbIncompleto[i][0]/BinvN[i][index];
			indexMenorRestricao = i;
		} else if(indexMenorRestricao != -1 && (menorRestricao > XbIncompleto[i][0]/BinvN[i][index]) && BinvN[i][index] > 0) {
			menorRestricao = XbIncompleto[i][0]/BinvN[i][index];
			indexMenorRestricao = i;
		}
	}
	return indexMenorRestricao;
}

bool validaInfinita(int numeroRestricoes, int index, float** BinvN) {
	bool algumValorPositivo = false;
	for(int i = 0; i < numeroRestricoes; i++) {
		if(0 <= BinvN[i][index]) {
			algumValorPositivo = true;
		}
	}
	return (algumValorPositivo == false);
}

int** fase1(int numeroVariaveis, int numeroRestricoes, float** a, float** b) {
	bool sucesso = true;
	bool infinita = false;
	float** c = criaCFase1(numeroVariaveis, numeroRestricoes);
	float** aMaisI = criaMatrizSomaIdentidade(numeroRestricoes, numeroVariaveis, a);
	int colunasAMaisI = numeroVariaveis + numeroRestricoes;
	int** iB = criaIBFase1(numeroRestricoes, numeroVariaveis);
	int** iN = obtemIN(numeroRestricoes, colunasAMaisI, iB);
	int iteracao = 0;
	while(true) {
		iteracao++;
		printf("Iniciando a itera????o %d com Ib igual a:\n", iteracao);
		printMatrixInt(1, numeroRestricoes, iB);
		ordenaLista(numeroRestricoes, iB);
		ordenaLista(colunasAMaisI - numeroRestricoes, iN);
		float** B = obtemB(numeroRestricoes, aMaisI, iB);
		printf("B da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, numeroRestricoes, B);
		float** N = obtemN(colunasAMaisI, numeroRestricoes, aMaisI, iB);
		printf("N da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, colunasAMaisI - numeroRestricoes, N);
		float** Binversa = obtemInversa(numeroRestricoes, B);
		printf("B inversa da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, numeroRestricoes, Binversa);
		float** XbIncompleto = multiplicaMatriz(numeroRestricoes, numeroRestricoes, numeroRestricoes, 1, Binversa, b);
		bool XbValido = validaXb(numeroRestricoes, XbIncompleto);
		printf("Xb incompleto da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, 1, XbIncompleto);
		if(XbValido == true) {
			float** Xb = completaXb(numeroRestricoes, colunasAMaisI,  XbIncompleto, iB);
			float** cT = obtemTransposta(1, colunasAMaisI, c);
			float** cTx = multiplicaMatriz(1, colunasAMaisI, colunasAMaisI, 1, cT, Xb);
			if(cTx[0][0] == 0) {
				sucesso = true;
				break;
			}
			float** BinvN = multiplicaMatriz(numeroRestricoes, numeroRestricoes, numeroRestricoes, numeroVariaveis, Binversa, N);
			float** cTb = obtemCTB(numeroRestricoes, colunasAMaisI, cT, iB);
			float** cTN = obtemCTN(numeroRestricoes, colunasAMaisI, cT, iB);
			float** CTbBinvN = multiplicaMatriz(1, numeroRestricoes, numeroRestricoes, numeroVariaveis, cTb, BinvN);
			float** custoReduzido = subTraiMatriz(1, numeroVariaveis, cTN, CTbBinvN);
			printf("Custo reduzido da itera????o %d:\n", iteracao);
			printMatrix(1, numeroVariaveis, custoReduzido);
			int indexMaiorZero = obtemCustoReduzidoMelhor(numeroVariaveis, custoReduzido, true);
			if(indexMaiorZero != -1) {
				int indexEntrarNoIb = iN[0][indexMaiorZero];
				infinita = validaInfinita(numeroRestricoes, indexMaiorZero, BinvN);
				if(infinita == true) {
					break;
				}
				int indexSubstituir = obtemMenorRestricao(numeroRestricoes, indexMaiorZero, BinvN, XbIncompleto);
				int indexIB = iB[0][indexSubstituir];
				printf("Ser?? substituido o %d pelo %d na itera????o %d:\n", indexIB, iN[0][indexMaiorZero], iteracao);
				printf("IN da iteracao %d:\n", iteracao);
				printMatrixInt(1, numeroVariaveis, iN);
				iB[0][indexSubstituir] = indexEntrarNoIb;
				iN[0][indexMaiorZero] = indexIB;
			} else {
				printf("Custo reduzido n??o foi encontrado");
				sucesso = false;
				break;
			}
		} else {
			printf("Xb n??o ?? valido\n\n");
			sucesso = false;
			break;
		}
	}
	if(sucesso == false) {
		printf("A matriz n??o tem solu????o");
		iB[0][0] = -1;
	} else if (infinita == true) {
		printf("A matriz ?? infinita");
		iB[0][0] = -1;
	}
	return iB;
}

void fase2(int numeroVariaveis, int numeroRestricoes, float** a, float** b, float** c, int** iB) {
	bool sucesso = true;
	bool infinita = false;
	int** iN = obtemIN(numeroRestricoes, numeroVariaveis, iB);
	int iteracao = 0;
	while(true) {
		iteracao++;
		printf("Iniciando a itera????o %d com Ib igual a:\n", iteracao);
		printMatrixInt(1, numeroRestricoes, iB);
		ordenaLista(numeroRestricoes, iB);
		ordenaLista(numeroVariaveis - numeroRestricoes, iN);
		float** B = obtemB(numeroRestricoes, a, iB);
		printf("B da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, numeroRestricoes, B);
		float** N = obtemN(numeroVariaveis, numeroRestricoes, a, iB);
		printf("N da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, numeroVariaveis - numeroRestricoes, N);
		float** Binversa = obtemInversa(numeroRestricoes, B);
		printf("B inversa da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, numeroRestricoes, Binversa);
		float** XbIncompleto = multiplicaMatriz(numeroRestricoes, numeroRestricoes, numeroRestricoes, 1, Binversa, b);
		bool XbValido = validaXb(numeroRestricoes, XbIncompleto);
		printf("Xb incompleto da itera????o %d:\n", iteracao);
		printMatrix(numeroRestricoes, 1, XbIncompleto);
		if(XbValido == true) {
			float** Xb = completaXb(numeroRestricoes, numeroVariaveis,  XbIncompleto, iB);
			float** cT = obtemTransposta(1, numeroVariaveis, c);
			float** cTx = multiplicaMatriz(1, numeroVariaveis, numeroVariaveis, 1, cT, Xb);
			float** BinvN = multiplicaMatriz(numeroRestricoes, numeroRestricoes, numeroRestricoes, numeroVariaveis, Binversa, N);
			float** cTb = obtemCTB(numeroRestricoes, numeroVariaveis, cT, iB);
			float** cTN = obtemCTN(numeroRestricoes, numeroVariaveis, cT, iB);
			float** CTbBinvN = multiplicaMatriz(1, numeroRestricoes, numeroRestricoes, numeroVariaveis - numeroRestricoes, cTb, BinvN);
			float** custoReduzido = subTraiMatriz(1, numeroVariaveis - numeroRestricoes, cTN, CTbBinvN);
			printf("Custo reduzido da itera????o %d:\n", iteracao);
			printMatrix(1, numeroVariaveis - numeroRestricoes, custoReduzido);
			int indexMaiorZero = obtemCustoReduzidoMelhor(numeroVariaveis - numeroRestricoes, custoReduzido, minimizar);
			if(indexMaiorZero != -1) {
				int indexEntrarNoIb = iN[0][indexMaiorZero];
				infinita = validaInfinita(numeroRestricoes, indexMaiorZero, BinvN);
				if(infinita == true) {
					break;
				}
				int indexSubstituir = obtemMenorRestricao(numeroRestricoes, indexMaiorZero, BinvN, XbIncompleto);
				int indexIB = iB[0][indexSubstituir];
				printf("Ser?? substituido o %d pelo %d na itera????o %d:\n", indexIB, iN[0][indexMaiorZero], iteracao);
				printf("IN da iteracao %d:\n", iteracao);
				printMatrixInt(1, numeroVariaveis, iN);
				iB[0][indexSubstituir] = indexEntrarNoIb;
				iN[0][indexMaiorZero] = indexIB;
			} else {
				printf("Solu????o otima encontrada valor de: %f\n\n", cTx[0][0]);
				sucesso = true;
				break;
			}
		} else {
			printf("Xb n??o ?? valido\n\n");
			sucesso = false;
			break;
		}
	}
	if(sucesso == false) {
		printf("A matriz n??o tem solu????o");
	} else if (infinita == true) {
		printf("A matriz ?? infinita");
	}
	printf("Sucesso fase 2 Ib igual a :\n");
	printMatrixInt(1, numeroRestricoes, iB);
}

float main()
{
	//le arquivo
    FILE *fp = fopen("problema.txt", "r");
    char chunk[128];
    size_t len = sizeof(chunk);
    char *line = malloc(len);
    if(line == NULL) {
        exit(1);
    }
    line[0] = '\0';
    int index = 0;
    int altura = 0;
    int largura = 0;
	float** A;
	float** b;
	float** c;
    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
        // Resize the line buffer if necessary
        size_t len_used = strlen(line);
        size_t chunk_used = strlen(chunk);
        if(len - len_used < chunk_used) {
            len *= 2;
            if((line = realloc(line, len)) == NULL) {
                perror("Unable to reallocate memory for the line buffer.");
                free(line);
                exit(1);
            }
        }
        strncpy(line + len_used, chunk, len - len_used);
        len_used += chunk_used;
        if(line[len_used - 1] == '\n') {
			printf(line);
            char *ptr;
            float d = (float) strtod(line, &ptr);
			if(index == 0) {
				if(line[0] == 't') {
					minimizar = true;
				} else {
					minimizar = false;
				}
			} else if(index == 1) {
                altura = (int)d;
            } else if (index == 2) {
                largura = (int)d;
				A = criaMatriz(altura, largura);
				b = criaMatriz(altura, 1);
				c = criaMatriz(largura, 1);
            } else if (index > 4 && index <= altura + 4) {
				A[index-5][0] = (float) d;
				for(int i = 1; i < largura; i++) {
					A[index-5][i] = (float)strtod(ptr, &ptr);
				}
			} else if (index > altura + 6 && index <= (2 *altura) + 6) {
				b[index - altura - 7][0] = (float)d;
			} else if (index > (2 *altura) + 8 && index <= (2*altura) + largura + 8) {
				c[index - ((2*altura) + 9)][0] = (float)d;
			}
			line[0] = '\0';
		}
        index++;
    }
    fclose(fp);
    free(line);
	printf("A matrix ter?? %d restricoes e %d variaveis\n", altura, largura);
	printf("A:\n");
	printMatrix(altura, largura, A);
	printf("b:\n");
	printMatrix(altura, 1, b);
	printf("c:\n");
	printMatrix(largura, 1, c);
	printf("Iniciando a fase 1");
	int** iB = fase1(largura, altura, A, b);
	if(iB[0][0] != -1) {
		printf("Sucesso fase 1 Ib igual a :\n");
		printMatrixInt(1, altura, iB);
		fase2(largura, altura, A, b, c, iB);
	}
	return (0);
}


