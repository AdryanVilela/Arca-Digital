# Sistema de Gerenciamento de Biblioteca

Sistema simples para gerenciar uma biblioteca, feito em C puro.
Trabalho da disciplina de Programação em C.

## O que o sistema faz

- Cadastra livros e usuários
- Faz empréstimos e devoluções
- Controla prazo de devolução (7 dias)
- Permite renovar empréstimo (máximo 2 vezes)
- Busca livros por título, autor ou código
- Gera relatórios de atrasos e livros mais emprestados
- Salva tudo em arquivos .txt

## Como instalar

### Pré-requisitos

Você precisa ter um compilador C instalado. Pode ser:

- **Dev-C++** (Windows) - [Download aqui](https://sourceforge.net/projects/orwelldevcpp/)
- **Code::Blocks** (Windows/Linux) - [Download aqui](https://www.codeblocks.org/downloads/)
- **GCC** (Windows/Linux) - [Download aqui](https://www.codeblocks.org/downloads/)

### Compilando

**No Dev-C++:**
1. Abra o arquivo `biblioteca.c`
2. Aperte `F11` ou vá em `Executar > Compilar e Executar`

**No Code::Blocks:**
1. Crie um novo projeto Console Application em C
2. Adicione o arquivo `biblioteca.c`
3. Aperte `F9` para compilar e executar

**No terminal (GCC):**
```bash
gcc biblioteca.c -o biblioteca
./biblioteca
```

**No Windows com GCC:**
```bash
gcc biblioteca.c -o biblioteca.exe
biblioteca.exe
```

## Como usar

Quando abrir o programa, vai aparecer o menu principal:

```
========================================
   SISTEMA DE GERENCIAMENTO DE BIBLIOTECA
========================================
1. Gerenciar Livros
2. Gerenciar Usuarios
3. Emprestimos e Devolucoes
4. Relatorios
5. Salvar dados
0. Sair do sistema
========================================
```

É só digitar o número da opção e apertar Enter.

### Cadastrando um livro

1. Menu principal → opção `1` (Gerenciar Livros)
2. Opção `1` (Cadastrar novo livro)
3. Digite os dados pedidos (código, título, autor, etc)

### Cadastrando um usuário

1. Menu principal → opção `2` (Gerenciar Usuarios)
2. Opção `1` (Cadastrar novo usuario)
3. Digite os dados (matrícula, nome, curso, telefone)

### Fazendo um empréstimo

1. Menu principal → opção `3` (Emprestimos)
2. Opção `1` (Realizar emprestimo)
3. Digite a matrícula do usuário
4. Digite o código do livro

O sistema calcula automaticamente a data de devolução (7 dias).

### Devolvendo um livro

1. Menu principal → opção `3` (Emprestimos)
2. Opção `2` (Realizar devolucao)
3. Digite o código do empréstimo

## Onde ficam os dados

Os arquivos são salvos na pasta `dados/`:

```
dados/
├── livros.txt       <- dados dos livros
├── usuarios.txt     <- dados dos usuários
├── emprestimos.txt  <- dados dos empréstimos
└── backup_DDMMAAAA.txt <- backups
```

**Importante:** Não edite esses arquivos manualmente!

## Estrutura do projeto

```
biblioteca/
├── biblioteca.c           <- código fonte principal
├── README.md              <- este arquivo
└── dados/                 <- pasta criada para organizaçao
    ├── livros.txt
    ├── usuarios.txt
    └── emprestimos.txt
```

## Funcionalidades

| Funcionalidade | Descrição |
|----------------|-----------|
| Cadastro de livros | Código, título, autor, editora, ano, exemplares |
| Cadastro de usuários | Matrícula, nome, curso, telefone |
| Empréstimo | Vincula usuário ao livro, prazo de 7 dias |
| Devolução | Registra devolução, avisa se tem atraso |
| Renovação | Estende prazo em 7 dias (máximo 2x) |
| Busca simples | Por código, título, autor ou nome |
| Busca avançada | Múltiplos critérios ao mesmo tempo |
| Relatório de atrasos | Lista quem não devolveu no prazo |
| Livros populares | Top 5 livros mais emprestados |
| Backup | Salva cópia dos dados com data |

## Requisitos atendidos

- [x] Estruturas de dados (structs)
- [x] Funções modularizadas
- [x] Manipulação de arquivos
- [x] Menu com validação de entrada
- [x] Busca e relatórios

## Autor

Desenvolvido por Adryan Vilela

## Licença

Projeto acadêmico - uso livre para estudos.
