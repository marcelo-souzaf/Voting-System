# Voting-System

Trabalho da A1 de Projeto e Análise de Algoritmos

## Compilando a aplicação

Foi utilizado o C++11 para a compilação do programa, através do WSL Ubuntu 20.04.

Software com interface interativa CLI:

```bash
g++ --std=c++11 app.cpp -I/usr/include/python3.8 -lpython3.8 -o app
```

Entre as opções, serão dados intervalos de tempo para a apreciação dos resultados. Diversos tratamentos de erros de inserção do usuário foram tratados.

Software para testes com quantidade massiva de dados:

```bash
g++ --std=c++11 benchmark.cpp -I/usr/include/python3.8 -lpython3.8 -o benchmark
```
## Arquitetura do projeto


Implementações e headers. (Depois colocar resultado do comando `tree` no terminal)

```
.
├── LICENSE
├── README.md
├── app.cpp
├── benchmark.cpp
├── \src
    ├── date.hpp
    ├── stack.hpp
    ├── timeMeasurement.hpp
    ├── vote.hpp
    └── voting.hpp
├── \lib
    ├── Vector.hpp
    ├── dbg_view.natvis
    └── matplotlibcpp.h
```

(Recado do adame, comenta os códigos, não precisa ser muito, mas o date.hpp por exemplo tá bem medonho)
(Também não esquece de testar bem todas as opções do app.cpp, inclusive colocando entradas erradas, a prova de idiota)