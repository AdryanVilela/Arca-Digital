// Trabalho de Programacao em C
// Sistema de Biblioteca;
// Autor: Adryan Vilela;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// quantidade maxima que o sistema suporta
#define MAX_LIVROS 200
#define MAX_USUARIOS 100
#define MAX_EMPRESTIMOS 200

// struct para guardar datas
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// struct do livro com todos os campos pedidos
typedef struct {
    int codigo;
    char titulo[101];
    char autor[81];
    char editora[61];
    int anoPublicacao;
    int exemplares;
    int emprestado;
} Livro;

// struct do usuario
typedef struct {
    int matricula;
    char nome[101];
    char curso[51];
    char telefone[16];
    Data dataCadastro;
} Usuario;

// struct do emprestimo
typedef struct {
    int codigo;
    int matriculaUsuario;
    int codigoLivro;
    Data dataEmprestimo;
    Data dataDevolucao;
    int ativo;
    int renovacoes;
} Emprestimo;

// variaveis globais - vetores que guardam os dados
Livro livros[MAX_LIVROS];
Usuario usuarios[MAX_USUARIOS];
Emprestimo emprestimos[MAX_EMPRESTIMOS];
;;;
// contadores de quantos tem cadastrado
int totalLivros = 0;
int totalUsuarios = 0;
int totalEmprestimos = 0;

// funcao pra limpar o buffer do teclado (senao da problema com scanf e fgets)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// pega a data de hoje do computador
Data obterDataAtual() {
    Data hoje;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    hoje.dia = tm_info->tm_mday;
    hoje.mes = tm_info->tm_mon + 1;  // mes comeca em 0 entao soma 1
    hoje.ano = tm_info->tm_year + 1900;

    return hoje;
}

// Adiciona dias a uma data (para calcular data de devolucao)
Data adicionarDias(Data data, int dias) {
    // Array com dias de cada mes
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    data.dia += dias;

    // Se passou do ultimo dia do mes, vai pro proximo
    while (data.dia > diasMes[data.mes]) {
        data.dia -= diasMes[data.mes];
        data.mes++;

        // Se passou de dezembro, vai pro proximo ano
        if (data.mes > 12) {
            data.mes = 1;
            data.ano++;
        }
    }

    return data;
}

// Verifica se uma data ja passou (para checar atrasos)
int dataPassou(Data data) {
    Data hoje = obterDataAtual();

    if (data.ano < hoje.ano) return 1;
    if (data.ano > hoje.ano) return 0;

    if (data.mes < hoje.mes) return 1;
    if (data.mes > hoje.mes) return 0;

    if (data.dia < hoje.dia) return 1;

    return 0;
}

// Pausa a execucao e espera o usuario apertar Enter
void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// cria a pasta dados se nao existir
void criarPastaDados() {
    #ifdef _WIN32
        system("mkdir dados 2>nul");
    #else
        system("mkdir -p dados 2>/dev/null");
    #endif
}

// --- funcoes pra salvar e carregar os arquivos ---

// Salva todos os livros no arquivo
void salvarLivros() {
    FILE *arquivo = fopen("dados/livros.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de livros!\n");
        return;
    }

    // Primeira linha: quantidade de livros
    fprintf(arquivo, "%d\n", totalLivros);

    // Salva cada livro em uma linha
    for (int i = 0; i < totalLivros; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%d;%d;%d\n",
            livros[i].codigo,
            livros[i].titulo,
            livros[i].autor,
            livros[i].editora,
            livros[i].anoPublicacao,
            livros[i].exemplares,
            livros[i].emprestado);
    }

    fclose(arquivo);
}

// Salva todos os usuarios no arquivo
void salvarUsuarios() {
    FILE *arquivo = fopen("dados/usuarios.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de usuarios!\n");
        return;
    }

    fprintf(arquivo, "%d\n", totalUsuarios);

    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%d;%d;%d\n",
            usuarios[i].matricula,
            usuarios[i].nome,
            usuarios[i].curso,
            usuarios[i].telefone,
            usuarios[i].dataCadastro.dia,
            usuarios[i].dataCadastro.mes,
            usuarios[i].dataCadastro.ano);
    }

    fclose(arquivo);
}

// Salva todos os emprestimos no arquivo
void salvarEmprestimos() {
    FILE *arquivo = fopen("dados/emprestimos.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de emprestimos!\n");
        return;
    }

    fprintf(arquivo, "%d\n", totalEmprestimos);

    for (int i = 0; i < totalEmprestimos; i++) {
        fprintf(arquivo, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",
            emprestimos[i].codigo,
            emprestimos[i].matriculaUsuario,
            emprestimos[i].codigoLivro,
            emprestimos[i].dataEmprestimo.dia,
            emprestimos[i].dataEmprestimo.mes,
            emprestimos[i].dataEmprestimo.ano,
            emprestimos[i].dataDevolucao.dia,
            emprestimos[i].dataDevolucao.mes,
            emprestimos[i].dataDevolucao.ano,
            emprestimos[i].ativo,
            emprestimos[i].renovacoes);
    }

    fclose(arquivo);
}

// Salva tudo de uma vez (backup)
void salvarTudo() {
    salvarLivros();
    salvarUsuarios();
    salvarEmprestimos();
    printf("Dados salvos com sucesso!\n");
}

// Cria um backup com data e hora no nome
void fazerBackup() {
    Data hoje = obterDataAtual();
    char nomeBackup[50];

    // nome do arquivo de backup com a data
    sprintf(nomeBackup, "dados/backup_%02d%02d%04d.txt", hoje.dia, hoje.mes, hoje.ano);

    FILE *backup = fopen(nomeBackup, "w");

    if (backup == NULL) {
        printf("Erro ao criar backup!\n");
        return;
    }

    // Salva tudo em um arquivo so
    fprintf(backup, "=== BACKUP DA BIBLIOTECA ===\n");
    fprintf(backup, "Data: %02d/%02d/%04d\n\n", hoje.dia, hoje.mes, hoje.ano);

    fprintf(backup, "LIVROS: %d\n", totalLivros);
    for (int i = 0; i < totalLivros; i++) {
        fprintf(backup, "Codigo: %d | Titulo: %s | Autor: %s\n",
            livros[i].codigo, livros[i].titulo, livros[i].autor);
    }

    fprintf(backup, "\nUSUARIOS: %d\n", totalUsuarios);
    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(backup, "Matricula: %d | Nome: %s\n",
            usuarios[i].matricula, usuarios[i].nome);
    }

    fprintf(backup, "\nEMPRESTIMOS ATIVOS: \n");
    for (int i = 0; i < totalEmprestimos; i++) {
        if (emprestimos[i].ativo) {
            fprintf(backup, "Codigo: %d | Usuario: %d | Livro: %d\n",
                emprestimos[i].codigo, emprestimos[i].matriculaUsuario,
                emprestimos[i].codigoLivro);
        }
    }

    fclose(backup);
    printf("Backup criado: %s\n", nomeBackup);
}


// Carrega livros do arquivo
void carregarLivros() {
    FILE *arquivo = fopen("dados/livros.txt", "r");

    // Se o arquivo nao existe, nao tem problema - comeca do zero
    if (arquivo == NULL) {
        totalLivros = 0;
        return;
    }

    fscanf(arquivo, "%d\n", &totalLivros);

    for (int i = 0; i < totalLivros; i++) {
        fscanf(arquivo, "%d;%100[^;];%80[^;];%60[^;];%d;%d;%d\n",
            &livros[i].codigo,
            livros[i].titulo,
            livros[i].autor,
            livros[i].editora,
            &livros[i].anoPublicacao,
            &livros[i].exemplares,
            &livros[i].emprestado);
    }

    fclose(arquivo);
}

// Carrega usuarios do arquivo
void carregarUsuarios() {
    FILE *arquivo = fopen("dados/usuarios.txt", "r");

    if (arquivo == NULL) {
        totalUsuarios = 0;
        return;
    }

    fscanf(arquivo, "%d\n", &totalUsuarios);

    for (int i = 0; i < totalUsuarios; i++) {
        fscanf(arquivo, "%d;%100[^;];%50[^;];%15[^;];%d;%d;%d\n",
            &usuarios[i].matricula,
            usuarios[i].nome,
            usuarios[i].curso,
            usuarios[i].telefone,
            &usuarios[i].dataCadastro.dia,
            &usuarios[i].dataCadastro.mes,
            &usuarios[i].dataCadastro.ano);
    }

    fclose(arquivo);
}

// Carrega emprestimos do arquivo
void carregarEmprestimos() {
    FILE *arquivo = fopen("dados/emprestimos.txt", "r");

    if (arquivo == NULL) {
        totalEmprestimos = 0;
        return;
    }

    fscanf(arquivo, "%d\n", &totalEmprestimos);

    for (int i = 0; i < totalEmprestimos; i++) {
        fscanf(arquivo, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",
            &emprestimos[i].codigo,
            &emprestimos[i].matriculaUsuario,
            &emprestimos[i].codigoLivro,
            &emprestimos[i].dataEmprestimo.dia,
            &emprestimos[i].dataEmprestimo.mes,
            &emprestimos[i].dataEmprestimo.ano,
            &emprestimos[i].dataDevolucao.dia,
            &emprestimos[i].dataDevolucao.mes,
            &emprestimos[i].dataDevolucao.ano,
            &emprestimos[i].ativo,
            &emprestimos[i].renovacoes);
    }

    fclose(arquivo);
}

// Carrega todos os dados na inicializacao
void carregarTudo() {
    carregarLivros();
    carregarUsuarios();
    carregarEmprestimos();
}

// --- funcoes de busca ---

// Procura um livro pelo codigo - retorna a posicao no vetor ou -1
int buscarLivroPorCodigo(int codigo) {
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].codigo == codigo) {
            return i;
        }
    }
    return -1;  // nao encontrou
}

// Procura um usuario pela matricula
int buscarUsuarioPorMatricula(int matricula) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].matricula == matricula) {
            return i;
        }
    }
    return -1;
}

// Mostra livros que contem um texto no titulo
void buscarLivroPorTitulo(char *titulo) {
    int encontrou = 0;

    printf("\n=== Resultados da busca por titulo ===\n");

    for (int i = 0; i < totalLivros; i++) {
        // strstr verifica se titulo esta dentro do titulo do livro
        if (strstr(livros[i].titulo, titulo) != NULL) {
            printf("Codigo: %d | Titulo: %s | Autor: %s | Exemplares: %d\n",
                livros[i].codigo, livros[i].titulo, livros[i].autor,
                livros[i].exemplares);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum livro encontrado com esse titulo.\n");
    }
}

// Mostra livros de um determinado autor
void buscarLivroPorAutor(char *autor) {
    int encontrou = 0;

    printf("\n=== Resultados da busca por autor ===\n");

    for (int i = 0; i < totalLivros; i++) {
        if (strstr(livros[i].autor, autor) != NULL) {
            printf("Codigo: %d | Titulo: %s | Autor: %s\n",
                livros[i].codigo, livros[i].titulo, livros[i].autor);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum livro encontrado desse autor.\n");
    }
}

// Mostra usuarios que contem um texto no nome
void buscarUsuarioPorNome(char *nome) {
    int encontrou = 0;

    printf("\n=== Resultados da busca por nome ===\n");

    for (int i = 0; i < totalUsuarios; i++) {
        if (strstr(usuarios[i].nome, nome) != NULL) {
            printf("Matricula: %d | Nome: %s | Curso: %s\n",
                usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum usuario encontrado com esse nome.\n");
    }
}



// Busca avancada - procura por varios criterios ao mesmo tempo
void buscaAvancada() {
    char titulo[101] = "";
    char autor[81] = "";
    int anoMin = 0, anoMax = 9999;
    int somenteDisponiveis = 0;
    int encontrou = 0;

    printf("\n=== BUSCA AVANCADA DE LIVROS ===\n");
    printf("(Deixe em branco para ignorar o criterio)\n\n");

    printf("Titulo contem: ");
    limparBuffer();
    fgets(titulo, 100, stdin);
    titulo[strcspn(titulo, "\n")] = 0;  // remove o \n

    printf("Autor contem: ");
    fgets(autor, 80, stdin);
    autor[strcspn(autor, "\n")] = 0;

    printf("Ano minimo (0 para ignorar): ");
    scanf("%d", &anoMin);

    printf("Ano maximo (0 para ignorar): ");
    scanf("%d", &anoMax);
    if (anoMax == 0) anoMax = 9999;

    printf("Somente disponiveis? (1=Sim, 0=Nao): ");
    scanf("%d", &somenteDisponiveis);

    printf("\n=== Resultados ===\n");

    for (int i = 0; i < totalLivros; i++) {
        // Verifica cada criterio
        int passou = 1;

        if (strlen(titulo) > 0 && strstr(livros[i].titulo, titulo) == NULL) {
            passou = 0;
        }

        if (strlen(autor) > 0 && strstr(livros[i].autor, autor) == NULL) {
            passou = 0;
        }

        if (livros[i].anoPublicacao < anoMin || livros[i].anoPublicacao > anoMax) {
            passou = 0;
        }

        if (somenteDisponiveis && livros[i].exemplares == 0) {
            passou = 0;
        }

        if (passou) {
            printf("Codigo: %d | Titulo: %s | Autor: %s | Ano: %d | Disp: %d\n",
                livros[i].codigo, livros[i].titulo, livros[i].autor,
                livros[i].anoPublicacao, livros[i].exemplares);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum livro encontrado com esses criterios.\n");
    }
}

// --- funcoes de cadastro ---

// Cadastra um novo livro no sistema
void cadastrarLivro() {
    // Verifica se ainda cabe mais livros
    if (totalLivros >= MAX_LIVROS) {
        printf("Limite de livros atingido!\n");
        return;
    }

    Livro novo;

    printf("\n=== CADASTRO DE LIVRO ===\n");

    printf("Codigo do livro: ");
    scanf("%d", &novo.codigo);

    // Verifica se ja existe
    if (buscarLivroPorCodigo(novo.codigo) != -1) {
        printf("Erro: Ja existe um livro com esse codigo!\n");
        return;
    }

    limparBuffer();

    printf("Titulo: ");
    fgets(novo.titulo, 100, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = 0;

    printf("Autor: ");
    fgets(novo.autor, 80, stdin);
    novo.autor[strcspn(novo.autor, "\n")] = 0;

    printf("Editora: ");
    fgets(novo.editora, 60, stdin);
    novo.editora[strcspn(novo.editora, "\n")] = 0;

    printf("Ano de publicacao: ");
    scanf("%d", &novo.anoPublicacao);

    printf("Numero de exemplares: ");
    scanf("%d", &novo.exemplares);

    novo.emprestado = 0;  // comeca como disponivel

    // Adiciona no vetor
    livros[totalLivros] = novo;
    totalLivros++;

    salvarLivros();  // salva no arquivo

    printf("\nLivro cadastrado com sucesso!\n");
}

// Cadastra um novo usuario
void cadastrarUsuario() {
    if (totalUsuarios >= MAX_USUARIOS) {
        printf("Limite de usuarios atingido!\n");
        return;
    }

    Usuario novo;

    printf("\n=== CADASTRO DE USUARIO ===\n");

    printf("Matricula: ");
    scanf("%d", &novo.matricula);

    if (buscarUsuarioPorMatricula(novo.matricula) != -1) {
        printf("Erro: Ja existe um usuario com essa matricula!\n");
        return;
    }

    limparBuffer();

    printf("Nome completo: ");
    fgets(novo.nome, 100, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Curso: ");
    fgets(novo.curso, 50, stdin);
    novo.curso[strcspn(novo.curso, "\n")] = 0;

    printf("Telefone: ");
    fgets(novo.telefone, 15, stdin);
    novo.telefone[strcspn(novo.telefone, "\n")] = 0;

    // Pega a data de hoje como data de cadastro
    novo.dataCadastro = obterDataAtual();

    usuarios[totalUsuarios] = novo;
    totalUsuarios++;

    salvarUsuarios();

    printf("\nUsuario cadastrado com sucesso!\n");
    printf("Data de cadastro: %02d/%02d/%04d\n",
        novo.dataCadastro.dia, novo.dataCadastro.mes, novo.dataCadastro.ano);
}


// --- funcoes de emprestimo ---

// Realiza um novo emprestimo
void realizarEmprestimo() {
    if (totalEmprestimos >= MAX_EMPRESTIMOS) {
        printf("Limite de emprestimos atingido!\n");
        return;
    }

    Emprestimo novo;
    int matricula, codigoLivro;
    int posUsuario, posLivro;

    printf("\n=== REALIZAR EMPRESTIMO ===\n");

    printf("Matricula do usuario: ");
    scanf("%d", &matricula);

    // Verifica se usuario existe
    posUsuario = buscarUsuarioPorMatricula(matricula);
    if (posUsuario == -1) {
        printf("Erro: Usuario nao encontrado!\n");
        return;
    }

    printf("Usuario: %s\n", usuarios[posUsuario].nome);

    printf("Codigo do livro: ");
    scanf("%d", &codigoLivro);

    // Verifica se livro existe
    posLivro = buscarLivroPorCodigo(codigoLivro);
    if (posLivro == -1) {
        printf("Erro: Livro nao encontrado!\n");
        return;
    }

    printf("Livro: %s\n", livros[posLivro].titulo);

    // Verifica se tem exemplares disponiveis
    if (livros[posLivro].exemplares <= 0) {
        printf("Erro: Nao ha exemplares disponiveis deste livro!\n");
        return;
    }

    // Cria o emprestimo
    novo.codigo = totalEmprestimos + 1;  // codigo sequencial
    novo.matriculaUsuario = matricula;
    novo.codigoLivro = codigoLivro;
    novo.dataEmprestimo = obterDataAtual();
    novo.dataDevolucao = adicionarDias(novo.dataEmprestimo, 7);  // 7 dias
    novo.ativo = 1;
    novo.renovacoes = 0;

    // Diminui quantidade de exemplares
    livros[posLivro].exemplares--;
    if (livros[posLivro].exemplares == 0) {
        livros[posLivro].emprestado = 1;  // marca como emprestado
    }

    emprestimos[totalEmprestimos] = novo;
    totalEmprestimos++;

    salvarLivros();
    salvarEmprestimos();

    printf("\nEmprestimo realizado com sucesso!\n");
    printf("Codigo do emprestimo: %d\n", novo.codigo);
    printf("Data de devolucao: %02d/%02d/%04d\n",
        novo.dataDevolucao.dia, novo.dataDevolucao.mes, novo.dataDevolucao.ano);
}

// Realiza a devolucao de um livro
void realizarDevolucao() {
    int codigoEmprestimo;
    int encontrou = -1;

    printf("\n=== REALIZAR DEVOLUCAO ===\n");

    printf("Codigo do emprestimo: ");
    scanf("%d", &codigoEmprestimo);

    // Procura o emprestimo
    for (int i = 0; i < totalEmprestimos; i++) {
        if (emprestimos[i].codigo == codigoEmprestimo) {
            encontrou = i;
            break;
        }
    }

    if (encontrou == -1) {
        printf("Erro: Emprestimo nao encontrado!\n");
        return;
    }

    if (!emprestimos[encontrou].ativo) {
        printf("Erro: Este emprestimo ja foi devolvido!\n");
        return;
    }

    // Pega informacoes para mostrar
    int posLivro = buscarLivroPorCodigo(emprestimos[encontrou].codigoLivro);
    int posUsuario = buscarUsuarioPorMatricula(emprestimos[encontrou].matriculaUsuario);

    printf("\nUsuario: %s\n", usuarios[posUsuario].nome);
    printf("Livro: %s\n", livros[posLivro].titulo);

    // Verifica se esta em atraso
    if (dataPassou(emprestimos[encontrou].dataDevolucao)) {
        printf("ATENCAO: Devolucao em atraso!\n");
    }

    // Efetua a devolucao
    emprestimos[encontrou].ativo = 0;
    livros[posLivro].exemplares++;
    livros[posLivro].emprestado = 0;  // tem pelo menos 1 disponivel agora

    salvarLivros();
    salvarEmprestimos();

    printf("\nDevolucao realizada com sucesso!\n");
}

// Renova um emprestimo (adiciona mais 7 dias)
void renovarEmprestimo() {
    int codigoEmprestimo;
    int encontrou = -1;

    printf("\n=== RENOVAR EMPRESTIMO ===\n");

    printf("Codigo do emprestimo: ");
    scanf("%d", &codigoEmprestimo);

    for (int i = 0; i < totalEmprestimos; i++) {
        if (emprestimos[i].codigo == codigoEmprestimo) {
            encontrou = i;
            break;
        }
    }

    if (encontrou == -1) {
        printf("Erro: Emprestimo nao encontrado!\n");
        return;
    }

    if (!emprestimos[encontrou].ativo) {
        printf("Erro: Este emprestimo ja foi devolvido!\n");
        return;
    }

    // Limite de 2 renovacoes
    if (emprestimos[encontrou].renovacoes >= 2) {
        printf("Erro: Limite de renovacoes atingido (maximo 2)!\n");
        return;
    }

    // Renova - adiciona 7 dias a partir de hoje
    emprestimos[encontrou].dataDevolucao = adicionarDias(obterDataAtual(), 7);
    emprestimos[encontrou].renovacoes++;

    salvarEmprestimos();

    printf("\nEmprestimo renovado com sucesso!\n");
    printf("Nova data de devolucao: %02d/%02d/%04d\n",
        emprestimos[encontrou].dataDevolucao.dia,
        emprestimos[encontrou].dataDevolucao.mes,
        emprestimos[encontrou].dataDevolucao.ano);
    printf("Renovacoes restantes: %d\n", 2 - emprestimos[encontrou].renovacoes);
}


// --- funcoes de listagem e relatorios ---

// Lista todos os livros cadastrados
void listarLivros() {
    printf("\n=== LISTA DE LIVROS ===\n");
    printf("Total: %d livros\n\n", totalLivros);

    if (totalLivros == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalLivros; i++) {
        printf("----------------------------------------\n");
        printf("Codigo: %d\n", livros[i].codigo);
        printf("Titulo: %s\n", livros[i].titulo);
        printf("Autor: %s\n", livros[i].autor);
        printf("Editora: %s\n", livros[i].editora);
        printf("Ano: %d\n", livros[i].anoPublicacao);
        printf("Exemplares disponiveis: %d\n", livros[i].exemplares);
        printf("Status: %s\n", livros[i].exemplares > 0 ? "Disponivel" : "Indisponivel");
    }
    printf("----------------------------------------\n");
}

// Lista todos os usuarios cadastrados
void listarUsuarios() {
    printf("\n=== LISTA DE USUARIOS ===\n");
    printf("Total: %d usuarios\n\n", totalUsuarios);

    if (totalUsuarios == 0) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        printf("----------------------------------------\n");
        printf("Matricula: %d\n", usuarios[i].matricula);
        printf("Nome: %s\n", usuarios[i].nome);
        printf("Curso: %s\n", usuarios[i].curso);
        printf("Telefone: %s\n", usuarios[i].telefone);
        printf("Cadastrado em: %02d/%02d/%04d\n",
            usuarios[i].dataCadastro.dia,
            usuarios[i].dataCadastro.mes,
            usuarios[i].dataCadastro.ano);
    }
    printf("----------------------------------------\n");
}

// Lista apenas os emprestimos que ainda estao ativos
void listarEmprestimosAtivos() {
    int temAtivos = 0;

    printf("\n=== EMPRESTIMOS ATIVOS ===\n\n");

    for (int i = 0; i < totalEmprestimos; i++) {
        if (emprestimos[i].ativo) {
            int posLivro = buscarLivroPorCodigo(emprestimos[i].codigoLivro);
            int posUsuario = buscarUsuarioPorMatricula(emprestimos[i].matriculaUsuario);

            printf("----------------------------------------\n");
            printf("Codigo do emprestimo: %d\n", emprestimos[i].codigo);
            printf("Usuario: %s (Mat: %d)\n",
                usuarios[posUsuario].nome, emprestimos[i].matriculaUsuario);
            printf("Livro: %s\n", livros[posLivro].titulo);
            printf("Data do emprestimo: %02d/%02d/%04d\n",
                emprestimos[i].dataEmprestimo.dia,
                emprestimos[i].dataEmprestimo.mes,
                emprestimos[i].dataEmprestimo.ano);
            printf("Devolucao prevista: %02d/%02d/%04d\n",
                emprestimos[i].dataDevolucao.dia,
                emprestimos[i].dataDevolucao.mes,
                emprestimos[i].dataDevolucao.ano);

            // Avisa se esta em atraso
            if (dataPassou(emprestimos[i].dataDevolucao)) {
                printf("*** ATRASADO ***\n");
            }

            temAtivos = 1;
        }
    }

    if (!temAtivos) {
        printf("Nenhum emprestimo ativo no momento.\n");
    }
    printf("----------------------------------------\n");
}

// Relatorio de usuarios com emprestimos em atraso
void relatorioAtrasos() {
    int temAtrasos = 0;

    printf("\n=== USUARIOS COM EMPRESTIMOS EM ATRASO ===\n\n");

    for (int i = 0; i < totalEmprestimos; i++) {
        // So verifica emprestimos ativos que estao atrasados
        if (emprestimos[i].ativo && dataPassou(emprestimos[i].dataDevolucao)) {
            int posLivro = buscarLivroPorCodigo(emprestimos[i].codigoLivro);
            int posUsuario = buscarUsuarioPorMatricula(emprestimos[i].matriculaUsuario);

            printf("----------------------------------------\n");
            printf("Usuario: %s\n", usuarios[posUsuario].nome);
            printf("Telefone: %s\n", usuarios[posUsuario].telefone);
            printf("Livro: %s\n", livros[posLivro].titulo);
            printf("Deveria devolver em: %02d/%02d/%04d\n",
                emprestimos[i].dataDevolucao.dia,
                emprestimos[i].dataDevolucao.mes,
                emprestimos[i].dataDevolucao.ano);

            temAtrasos = 1;
        }
    }

    if (!temAtrasos) {
        printf("Nenhum emprestimo em atraso. Otimo!\n");
    }
    printf("----------------------------------------\n");
}

// Relatorio de livros mais emprestados
void relatorioLivrosMaisEmprestados() {
    // Array para contar emprestimos de cada livro
    int contagem[MAX_LIVROS] = {0};

    printf("\n=== LIVROS MAIS EMPRESTADOS ===\n\n");

    if (totalLivros == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    // Conta quantas vezes cada livro foi emprestado
    for (int i = 0; i < totalEmprestimos; i++) {
        int posLivro = buscarLivroPorCodigo(emprestimos[i].codigoLivro);
        if (posLivro != -1) {
            contagem[posLivro]++;
        }
    }

    // Ordena e mostra os 5 mais emprestados (usando bubble sort simples)
    // Cria um array de indices
    int indices[MAX_LIVROS];
    for (int i = 0; i < totalLivros; i++) {
        indices[i] = i;
    }

    // Ordena por contagem (decrescente)
    for (int i = 0; i < totalLivros - 1; i++) {
        for (int j = 0; j < totalLivros - i - 1; j++) {
            if (contagem[indices[j]] < contagem[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    // Mostra os 5 primeiros (ou menos se nao tiver 5)
    int mostrar = totalLivros < 5 ? totalLivros : 5;

    for (int i = 0; i < mostrar; i++) {
        int idx = indices[i];
        printf("%d. %s - %d emprestimo(s)\n",
            i + 1, livros[idx].titulo, contagem[idx]);
    }
}


// --- menus do sistema ---

// Submenu de livros
void menuLivros() {
    int opcao;
    char busca[101];
    int codigo;

    do {
        limparTela();
        printf("\n========================================\n");
        printf("         MENU DE LIVROS\n");
        printf("========================================\n");
        printf("1. Cadastrar novo livro\n");
        printf("2. Listar todos os livros\n");
        printf("3. Buscar por codigo\n");
        printf("4. Buscar por titulo\n");
        printf("5. Buscar por autor\n");
        printf("6. Busca avancada\n");
        printf("0. Voltar ao menu principal\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarLivro();
                pausar();
                break;
            case 2:
                listarLivros();
                pausar();
                break;
            case 3:
                printf("Digite o codigo: ");
                scanf("%d", &codigo);
                int pos = buscarLivroPorCodigo(codigo);
                if (pos != -1) {
                    printf("\nLivro encontrado:\n");
                    printf("Titulo: %s\n", livros[pos].titulo);
                    printf("Autor: %s\n", livros[pos].autor);
                    printf("Exemplares: %d\n", livros[pos].exemplares);
                } else {
                    printf("Livro nao encontrado!\n");
                }
                pausar();
                break;
            case 4:
                printf("Digite parte do titulo: ");
                limparBuffer();
                fgets(busca, 100, stdin);
                busca[strcspn(busca, "\n")] = 0;
                buscarLivroPorTitulo(busca);
                pausar();
                break;
            case 5:
                printf("Digite o nome do autor: ");
                limparBuffer();
                fgets(busca, 80, stdin);
                busca[strcspn(busca, "\n")] = 0;
                buscarLivroPorAutor(busca);
                pausar();
                break;
            case 6:
                buscaAvancada();
                pausar();
                break;
        }
    } while (opcao != 0);
}

// Submenu de usuarios
void menuUsuarios() {
    int opcao;
    char busca[101];
    int matricula;

    do {
        limparTela();
        printf("\n========================================\n");
        printf("         MENU DE USUARIOS\n");
        printf("========================================\n");
        printf("1. Cadastrar novo usuario\n");
        printf("2. Listar todos os usuarios\n");
        printf("3. Buscar por matricula\n");
        printf("4. Buscar por nome\n");
        printf("0. Voltar ao menu principal\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarUsuario();
                pausar();
                break;
            case 2:
                listarUsuarios();
                pausar();
                break;
            case 3:
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                int pos = buscarUsuarioPorMatricula(matricula);
                if (pos != -1) {
                    printf("\nUsuario encontrado:\n");
                    printf("Nome: %s\n", usuarios[pos].nome);
                    printf("Curso: %s\n", usuarios[pos].curso);
                    printf("Telefone: %s\n", usuarios[pos].telefone);
                } else {
                    printf("Usuario nao encontrado!\n");
                }
                pausar();
                break;
            case 4:
                printf("Digite parte do nome: ");
                limparBuffer();
                fgets(busca, 100, stdin);
                busca[strcspn(busca, "\n")] = 0;
                buscarUsuarioPorNome(busca);
                pausar();
                break;
        }
    } while (opcao != 0);
}

// Submenu de emprestimos
void menuEmprestimos() {
    int opcao;

    do {
        limparTela();
        printf("\n========================================\n");
        printf("         MENU DE EMPRESTIMOS\n");
        printf("========================================\n");
        printf("1. Realizar emprestimo\n");
        printf("2. Realizar devolucao\n");
        printf("3. Renovar emprestimo\n");
        printf("4. Listar emprestimos ativos\n");
        printf("0. Voltar ao menu principal\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                realizarEmprestimo();
                pausar();
                break;
            case 2:
                realizarDevolucao();
                pausar();
                break;
            case 3:
                renovarEmprestimo();
                pausar();
                break;
            case 4:
                listarEmprestimosAtivos();
                pausar();
                break;
        }
    } while (opcao != 0);
}


// Submenu de relatorios
void menuRelatorios() {
    int opcao;

    do {
        limparTela();
        printf("\n========================================\n");
        printf("         MENU DE RELATORIOS\n");
        printf("========================================\n");
        printf("1. Livros mais emprestados\n");
        printf("2. Usuarios com atraso\n");
        printf("3. Fazer backup\n");
        printf("0. Voltar ao menu principal\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                relatorioLivrosMaisEmprestados();
                pausar();
                break;
            case 2:
                relatorioAtrasos();
                pausar();
                break;
            case 3:
                fazerBackup();
                pausar();
                break;
        }
    } while (opcao != 0);
}

// Menu principal - primeira tela do sistema
void menuPrincipal() {
    int opcao;

    do {
        limparTela();
        printf("\n========================================\n");
        printf("   SISTEMA DE GERENCIAMENTO DE BIBLIOTECA\n");
        printf("========================================\n");
        printf("1. Gerenciar Livros\n");
        printf("2. Gerenciar Usuarios\n");
        printf("3. Emprestimos e Devolucoes\n");
        printf("4. Relatorios\n");
        printf("5. Salvar dados\n");
        printf("0. Sair do sistema\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menuLivros();
                break;
            case 2:
                menuUsuarios();
                break;
            case 3:
                menuEmprestimos();
                break;
            case 4:
                menuRelatorios();
                break;
            case 5:
                salvarTudo();
                pausar();
                break;
            case 0:
                printf("\nSalvando dados antes de sair...\n");
                salvarTudo();
                printf("Obrigado por usar o sistema! Ate logo.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                pausar();
        }
    } while (opcao != 0);
}

// funcao principal

int main() {
    // cria a pasta dados se nao existir
    criarPastaDados();

    // Carrega os dados salvos anteriormente
    printf("Carregando dados...\n");
    carregarTudo();
    printf("Dados carregados: %d livros, %d usuarios, %d emprestimos\n",
        totalLivros, totalUsuarios, totalEmprestimos);


    printf("\nIniciando o sistema...\n");


    menuPrincipal();

    return 0;
}
