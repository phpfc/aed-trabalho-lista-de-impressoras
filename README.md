# Trabalho de listas: lista de impressoras

Trabalho de implementação da disciplina FBI4005 - Algoritmos e Estrutura de Dados I (UCS, 2026/2). O enunciado completo está em [ENUNCIADO.md](ENUNCIADO.md).

## Estruturas

- **Lista de impressoras**: lista encadeada simples. Começa com 3 impressoras e cresce pela operação 8.
- **Fila de impressão**: cada impressora tem a sua, também uma lista encadeada, com ponteiros de início e fim e a quantidade de documentos.
- **Documento**: registro com código (inteiro) e nome do arquivo (string).

Cada impressora guarda ainda se está ligada e quantas impressões já realizou.

## Operações

| Opção | Operação |
|---|---|
| 1 | Enviar documento para impressão (enfileira) |
| 2 | Imprimir (desenfileira) |
| 3 | Cancelar impressão (retira um documento de qualquer posição da fila) |
| 4 | Relatório de uma impressora: documentos na fila e impressões realizadas |
| 5 | Desligar impressora, intercalando a fila dela com a da impressora destino |
| 6 | Priorizar um documento, que passa para a primeira posição da fila |
| 7 | Exibir a fila de impressão de uma impressora |
| 8 | Inserir nova impressora |
| 9 | Extra: religar impressora |
| 10 | Extra: relatório geral de todas as impressoras |
| 11 | Extra: localizar um documento pelo código |

Na operação 5 a intercalação começa por um documento da impressora destino, depois um da desligada, e segue assim até acabarem os documentos da desligada. Os nodos são reaproveitados, só os ponteiros mudam.

O código de um documento é único entre todas as filas, para que não haja dois documentos iguais depois de uma transferência.

## Como compilar e executar

```sh
gcc -Wall -Wextra -std=c99 -o impressoras impressoras.c
./impressoras
```
