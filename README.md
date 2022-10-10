# Atividade 1

## Calculadora

A ideia para essa atividade foi implementar uma calculadora utilizando a biblioteca ImGui: https://lineconquista.github.io/cg-atv1/calc/

É possível realizar as operações de soma, subtração, multiplicação, divisão e porcentagem. Sendo que as ordens dos operadores é respeitado, de forma que divisão, multiplicação e porcentagem são operações realizadas primeiro, da direita para a esquerda. E por fim as operações de soma e subtração.

Há também algumas validações na aplicação:
 - Não é permitido inserir o número 0 após o operador de divisão, por não ser uma operação matemática válida.
 - Não é possível inserir um operador matemático após outro operador, apenas após números
 - Não é possível inserir um operador matemático como primeiro dígito da expressão
 - Não é possível finalizar uma operação com um operador matemático
 - O tamanho máximo de uma expressão, contabilizando com os operadores, é de 30 dígitos (uma possível futura melhoria é aceitar uma quantidade ilimitada)
 - Os dois botões em branco não possuem nenhuma operação/ação atrelada a eles (outra possível melhoria seria adicionar a operação de potência e raíz quadrada)
