/*
 * Trabalho de listas: lista de impressoras
 * Algoritmos e Estrutura de Dados I - UCS
 * Pedro Henrique França
 *
 * As impressoras ficam em uma lista encadeada e cada impressora tem a sua
 * fila de impressão, que é outra lista encadeada (de documentos).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define IMPRESSORAS_INICIAIS 3

typedef struct Documento {
    int codigo;
    char nome[MAX_NOME];
    struct Documento *prox;
} Documento;

typedef struct {
    Documento *inicio;
    Documento *fim;
    int qtd;
} Fila;

typedef struct Impressora {
    int id;
    int ligada;
    int impressas;
    Fila fila;
    struct Impressora *prox;
} Impressora;

/* ---------- lista de impressoras ---------- */

Impressora *insereImpressora(Impressora *lista) {
    Impressora *nova = malloc(sizeof(Impressora));
    if (nova == NULL) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    nova->id = 1;
    nova->ligada = 1;
    nova->impressas = 0;
    nova->fila.inicio = NULL;
    nova->fila.fim = NULL;
    nova->fila.qtd = 0;
    nova->prox = NULL;

    if (lista == NULL)
        return nova;
    Impressora *ultima = lista;
    while (ultima->prox != NULL)
        ultima = ultima->prox;
    nova->id = ultima->id + 1;
    ultima->prox = nova;
    return lista;
}

Impressora *buscaImpressora(Impressora *lista, int id) {
    while (lista != NULL && lista->id != id)
        lista = lista->prox;
    return lista;
}

void liberaTudo(Impressora *lista) {
    while (lista != NULL) {
        Impressora *proxima = lista->prox;
        Documento *d = lista->fila.inicio;
        while (d != NULL) {
            Documento *prox = d->prox;
            free(d);
            d = prox;
        }
        free(lista);
        lista = proxima;
    }
}

/* ---------- fila de impressão ---------- */

void exibeFila(Impressora *imp) {
    printf("Impressora %d (%s) - %d documento(s) na fila\n", imp->id,
           imp->ligada ? "ligada" : "desligada", imp->fila.qtd);
    int posicao = 1;
    for (Documento *d = imp->fila.inicio; d != NULL; d = d->prox)
        printf("  %d) codigo %d - %s\n", posicao++, d->codigo, d->nome);
}

/* ---------- leitura ---------- */

void leLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\n");
        exit(0);
    }
    destino[strcspn(destino, "\n")] = '\0';
}

int leInt(const char *pergunta) {
    char linha[MAX_NOME];
    int valor;
    while (1) {
        printf("%s", pergunta);
        leLinha(linha, MAX_NOME);
        if (sscanf(linha, "%d", &valor) == 1)
            return valor;
        printf("Digite um numero\n");
    }
}

Impressora *escolheImpressora(Impressora *lista, const char *pergunta) {
    int id = leInt(pergunta);
    Impressora *imp = buscaImpressora(lista, id);
    if (imp == NULL)
        printf("Impressora %d nao existe\n", id);
    return imp;
}

/* ---------- operações do menu ---------- */

void opExibeFila(Impressora *lista) {
    Impressora *imp = escolheImpressora(lista, "Impressora: ");
    if (imp != NULL)
        exibeFila(imp);
}

/* procura o código em todas as filas, para não repetir a identificação de um documento */
Impressora *buscaDocumento(Impressora *lista, int codigo) {
    for (Impressora *imp = lista; imp != NULL; imp = imp->prox)
        for (Documento *d = imp->fila.inicio; d != NULL; d = d->prox)
            if (d->codigo == codigo)
                return imp;
    return NULL;
}

void enfileira(Fila *f, int codigo, const char *nome) {
    Documento *novo = malloc(sizeof(Documento));
    if (novo == NULL) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    novo->codigo = codigo;
    strncpy(novo->nome, nome, MAX_NOME - 1);
    novo->nome[MAX_NOME - 1] = '\0';
    novo->prox = NULL;
    if (f->inicio == NULL)
        f->inicio = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
    f->qtd++;
}

void opEnvia(Impressora *lista) {
    Impressora *imp = escolheImpressora(lista, "Impressora: ");
    if (imp == NULL)
        return;
    if (!imp->ligada) {
        printf("Impressora %d esta desligada\n", imp->id);
        return;
    }
    int codigo = leInt("Codigo do documento: ");
    Impressora *outra = buscaDocumento(lista, codigo);
    if (outra != NULL) {
        printf("Ja existe um documento com o codigo %d na impressora %d\n", codigo, outra->id);
        return;
    }
    char nome[MAX_NOME];
    printf("Nome do arquivo: ");
    leLinha(nome, MAX_NOME);
    enfileira(&imp->fila, codigo, nome);
    printf("Documento %d enviado para a impressora %d (%d na fila)\n", codigo, imp->id, imp->fila.qtd);
}

/* tira o primeiro documento da fila; quem chama é responsável por dar free */
Documento *desenfileira(Fila *f) {
    Documento *primeiro = f->inicio;
    if (primeiro == NULL)
        return NULL;
    f->inicio = primeiro->prox;
    if (f->inicio == NULL)
        f->fim = NULL;
    primeiro->prox = NULL;
    f->qtd--;
    return primeiro;
}

void opImprime(Impressora *lista) {
    Impressora *imp = escolheImpressora(lista, "Impressora: ");
    if (imp == NULL)
        return;
    if (!imp->ligada) {
        printf("Impressora %d esta desligada\n", imp->id);
        return;
    }
    Documento *d = desenfileira(&imp->fila);
    if (d == NULL) {
        printf("Impressora %d nao tem documentos na fila\n", imp->id);
        return;
    }
    imp->impressas++;
    printf("Impressora %d imprimiu: codigo %d - %s (%d na fila)\n", imp->id, d->codigo, d->nome, imp->fila.qtd);
    free(d);
}

void mostraMenu(void) {
    printf("\n1 - Enviar documento para impressao\n");
    printf("2 - Imprimir\n");
    printf("7 - Exibir fila de impressao de uma impressora\n");
    printf("0 - Sair\n");
}

int main(void) {
    Impressora *lista = NULL;
    for (int i = 0; i < IMPRESSORAS_INICIAIS; i++)
        lista = insereImpressora(lista);

    int opcao;
    do {
        mostraMenu();
        opcao = leInt("Opcao: ");
        switch (opcao) {
        case 1:
            opEnvia(lista);
            break;
        case 2:
            opImprime(lista);
            break;
        case 7:
            opExibeFila(lista);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    liberaTudo(lista);
    return 0;
}
