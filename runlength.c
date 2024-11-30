#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compactar(const char *entrada, const char *saida) {
    FILE *arquivoEntrada = fopen(entrada, "r");
    FILE *arquivoSaida = fopen(saida, "w");

    if (arquivoEntrada == NULL || arquivoSaida == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    char linha[256];
    int largura, altura, maxValor;

    fgets(linha, sizeof(linha), arquivoEntrada);
    fprintf(arquivoSaida, "P8\n");

    fgets(linha, sizeof(linha), arquivoEntrada);
    while (linha[0] == '#') { 
        fgets(linha, sizeof(linha), arquivoEntrada);
    }
    sscanf(linha, "%d %d", &largura, &altura);
    fprintf(arquivoSaida, "%d %d\n", largura, altura);

    fscanf(arquivoEntrada, "%d", &maxValor);
    fprintf(arquivoSaida, "%d\n", maxValor);

    int valorAtual, valorAnterior = -1, contador = 0;
    while (fscanf(arquivoEntrada, "%d", &valorAtual) == 1) {
        if (valorAtual == valorAnterior) {
            contador++;
        } else {
            if (contador > 0) {
                fprintf(arquivoSaida, "@%d %d ", contador, valorAnterior);
            }
            valorAnterior = valorAtual;
            contador = 1;
        }
    }

    if (contador > 0) { 
        fprintf(arquivoSaida, "@%d %d ", contador, valorAnterior);
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    printf("Arquivo compactado com sucesso para '%s'.\n", saida);
}

void descompactar(const char *entrada, const char *saida) {
    FILE *arquivoEntrada = fopen(entrada, "r");
    FILE *arquivoSaida = fopen(saida, "w");

    if (arquivoEntrada == NULL || arquivoSaida == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    char linha[256];
    int largura, altura, maxValor;

    fgets(linha, sizeof(linha), arquivoEntrada);
    fprintf(arquivoSaida, "P2\n");

    fgets(linha, sizeof(linha), arquivoEntrada);
    while (linha[0] == '#') { 
        fgets(linha, sizeof(linha), arquivoEntrada);
    }
    sscanf(linha, "%d %d", &largura, &altura);
    fprintf(arquivoSaida, "%d %d\n", largura, altura);

    fscanf(arquivoEntrada, "%d", &maxValor);
    fprintf(arquivoSaida, "%d\n", maxValor);

    char identificador;
    int contador, valor;
    while (fscanf(arquivoEntrada, " %c", &identificador) == 1) { 
        if (identificador == '@') { 
            fscanf(arquivoEntrada, "%d %d", &contador, &valor);
            for (int i = 0; i < contador; i++) {
                fprintf(arquivoSaida, "%d ", valor);
            }
        } else { 
            ungetc(identificador, arquivoEntrada);
            fscanf(arquivoEntrada, "%d", &valor);
            fprintf(arquivoSaida, "%d ", valor);
        }
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    printf("Arquivo descompactado com sucesso para '%s'.\n", saida);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <entrada> <saida>\n", argv[0]);
        printf("Arquivos de entrada devem estar nos formatos '.pgm' ou '.pgmc'.\n");
        return 1;
    }

    if (strstr(argv[1], ".pgm") != NULL) {
        compactar(argv[1], argv[2]);
    } else if (strstr(argv[1], ".pgmc") != NULL) {
        descompactar(argv[1], argv[2]);
    } else {
        printf("Formato de arquivo desconhecido. Use '.pgm' para compactar ou '.pgmc' para descompactar.\n");
        return 1;
    }

    return 0;
}
