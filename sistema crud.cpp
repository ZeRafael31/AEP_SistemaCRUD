#include <stdio.h>//opa
#include <string.h>
#include <stdlib.h>

#define FILENAME "usuarios"
#define CHAVE_CRIPTOGRAFIA 3

typedef struct {
    char nome[50];
    int idade;
} Usuario;

void criptografar(char *texto) {
	    for (int i = 0; i < strlen(texto); i++){
        texto[i] += CHAVE_CRIPTOGRAFIA;
    }
}

void descriptografar(char *texto) {
    for (int i = 0; i < strlen(texto); i++) {
        texto[i] -= CHAVE_CRIPTOGRAFIA;
    }
}

void adicionar_usuario() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    Usuario usuario;
    printf("Digite o nome do usuario: ");
    scanf(" %[^\n]s", usuario.nome);
    printf("Digite a idade do usuario: ");
    scanf("%d", &usuario.idade);

    criptografar(usuario.nome);
    fprintf(file, "%s %d\n", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
    fclose(file);
    printf("Usuario adicionado com sucesso!\n");
}

void listar_usuarios() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }
    Usuario usuario;
    printf("\nLista de Usuarios:\n");
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        descriptografar(usuario.nome);
        usuario.idade -= CHAVE_CRIPTOGRAFIA;
        printf("Nome: %s | Idade: %d\n", usuario.nome, usuario.idade);
    }
    fclose(file);
}

void atualizar_usuario() {
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[50];
    printf("Digite o nome do usuario para atualizar: ");
    scanf(" %[^\n]s", nome);

    int encontrado = 0;
    long pos;
    Usuario usuario;
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        pos = ftell(file);
        descriptografar(usuario.nome);
        if (strcmp(usuario.nome, nome) == 0) {
            encontrado = 1;
            printf("Digite o novo nome: ");
            scanf(" %[^\n]s", usuario.nome);
            printf("Digite a nova idade: ");
            scanf("%d", &usuario.idade);

            criptografar(usuario.nome);
            fseek(file, pos - strlen(usuario.nome) - 4, SEEK_SET);
            fprintf(file, "%s %d\n", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
            printf("Usuario atualizado com sucesso!\n");
            break;
        }
    }
    if (!encontrado) {
        printf("Usuario nao encontrado.\n");
    }
    fclose(file);
}

void excluir_usuario() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(file);
        return;
    }

    char nome[50];
    printf("Digite o nome do usuario para excluir: ");
    scanf(" %[^\n]s", nome);

    int encontrado = 0;
    Usuario usuario;
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        descriptografar(usuario.nome);
        if (strcmp(usuario.nome, nome) != 0) {
            criptografar(usuario.nome);
            fprintf(tempFile, "%s %d\n", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
        } else {
            encontrado = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Usuario excluido com sucesso!\n");
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

int main() {
    int opcao; //parte principal|menu

    do {
        printf("\n---Menu de opcoes---\n");
        printf("1. Adicionar Usuario\n");
        printf("2. Listar Usuarios\n");
        printf("3. Atualizar Usuario\n");
        printf("4. Excluir Usuario\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_usuario();
                break;
            case 2:
                listar_usuarios();
                break;
            case 3:
                atualizar_usuario();
                break;
            case 4:
                excluir_usuario();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
