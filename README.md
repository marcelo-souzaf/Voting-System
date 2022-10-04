# Voting-System

Trabalho da A1 de Projeto e Análise de Algoritmos

## Compilando a aplicação

Foram utilizados C++11 e Python 3.8 para a compilação do programa, através do WSL Ubuntu 20.04.

Software com interface interativa CLI:

```bash
g++ --std=c++11 -O2 app.cpp -I/usr/include/python3.8 -lpython3.8 -o app
```

Entre as opções, serão dados intervalos de tempo para a apreciação dos resultados. Diversos tratamentos de erros de inserção do usuário foram tratados.

Software para testes com quantidade massiva de dados:

```bash
g++ --std=c++11 -O2 benchmark.cpp -I/usr/include/python3.8 -lpython3.8 -o benchmark
```
## Arquitetura do projeto

Implementações e headers. (Depois colocar resultado do comando `tree` no terminal)

```
.
├── LICENSE
├── README.md
├── Sistema de Votação.pdf
├── app.cpp
├── benchmark.cpp
├── dbg_view.natvis
├── include
│   ├── Vector.hpp
│   ├── date.hpp
│   ├── matplotlibcpp.h
│   ├── stack.hpp
│   ├── vote.hpp
│   └── voting.hpp
├── votes.cpp
└── voting.hpp
```

O arquivo plot.cpp é focado em fazer testes intensivos e mostrar resultados graficamente, enquanto o arquivo app.cpp foi feito para interações do usuário/administrador com o sistema, aceitando apenas inserções manuais.

Mais documentação e opções de interação não foram possíveis devido a problemas sérios de prazo restante.
