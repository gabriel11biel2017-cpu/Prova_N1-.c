#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARQ "pessoas.dat"

typedef struct {
    char nome[50];
    char cpf[12];
    int idade;
    char email[50];
    int ativo;
} Pessoa;

int cpfExiste(char *cpf) {
    FILE *f = fopen(ARQ, "rb");
    if (!f) return 0;

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (p.ativo == 1 && strcmp(p.cpf, cpf) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void cadastrar() {
    Pessoa p;
    getchar();

    printf("\n--- Cadastro de Pessoa ---\n");

    printf("Nome completo: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("CPF (11 digitos): ");
    scanf("%s", p.cpf);

    if (cpfExiste(p.cpf)) {
        printf("CPF ja cadastrado!\n");
        return;
    }

    printf("Idade: ");
    scanf("%d", &p.idade);

    printf("E-mail: ");
    scanf("%s", p.email);

    p.ativo = 1;

    FILE *f = fopen(ARQ, "ab");
    fwrite(&p, sizeof(Pessoa), 1, f);
    fclose(f);

    printf("Cadastro realizado com sucesso!\n");
}

void listar() {
    FILE *f = fopen(ARQ, "rb");
    if (!f) {
        printf("Nenhum registro encontrado.\n");
        return;
    }

    Pessoa p;
    printf("\n--- Lista de Pessoas ---\n");

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (p.ativo == 1) {
            printf("Nome: %s | CPF: %s | Idade: %d | Email: %s\n",
                   p.nome, p.cpf, p.idade, p.email);
        }
    }

    fclose(f);
}

void buscar() {
    char cpf[12];
    printf("CPF para buscar: ");
    scanf("%s", cpf);

    FILE *f = fopen(ARQ, "rb");
    if (!f) {
        printf("Arquivo inexistente.\n");
        return;
    }

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (p.ativo == 1 && strcmp(p.cpf, cpf) == 0) {
            printf("Encontrado: Nome: %s | Idade: %d | Email: %s\n",
                   p.nome, p.idade, p.email);
            fclose(f);
            return;
        }
    }

    printf("Pessoa nao encontrada.\n");
    fclose(f);
}

void atualizar() {
    char cpf[12];
    printf("CPF para atualizar: ");
    scanf("%s", cpf);

    FILE *f = fopen(ARQ, "rb+");
    if (!f) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (p.ativo == 1 && strcmp(p.cpf, cpf) == 0) {

            printf("Novo nome completo: ");
            getchar();
            fgets(p.nome, sizeof(p.nome), stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';

            printf("Nova idade: ");
            scanf("%d", &p.idade);

            printf("Novo e-mail: ");
            scanf("%s", p.email);

            fseek(f, -sizeof(Pessoa), SEEK_CUR);
            fwrite(&p, sizeof(Pessoa), 1, f);

            fclose(f);
            printf("Atualizado com sucesso!\n");
            return;
        }
    }

    printf("CPF nao encontrado.\n");
    fclose(f);
}

void remover() {
    char cpf[12];
    printf("CPF para remover: ");
    scanf("%s", cpf);

    FILE *f = fopen(ARQ, "rb+");
    if (!f) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (p.ativo == 1 && strcmp(p.cpf, cpf) == 0) {
            p.ativo = 0;
            fseek(f, -sizeof(Pessoa), SEEK_CUR);
            fwrite(&p, sizeof(Pessoa), 1, f);

            fclose(f);
            printf("Removido com sucesso!\n");
            return;
        }
    }

    printf("CPF nao encontrado.\n");
    fclose(f);
}

int main() {
    int op;

    do {
        printf("\n--- Sistema de Cadastro de Pessoas ---\n");
        printf("1. Cadastrar pessoa\n");
        printf("2. Listar todas as pessoas\n");
        printf("3. Buscar por CPF\n");
        printf("4. Atualizar por CPF\n");
        printf("5. Remover por CPF\n");
        printf("6. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: buscar(); break;
            case 4: atualizar(); break;
            case 5: remover(); break;
            case 6: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while (op != 6);

 return 0;
}
