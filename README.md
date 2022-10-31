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
 
 
 
# Atividade 2

## Collector

A ideia para essa atividade foi implementar um jogo: https://lineconquista.github.io/cg-atv1/collector/

Nesse jogo você controla o que irei chamar de Collector, na parte inferior da tela, na qual é possível movimenta-lo para a esquerda e direita com as teclas left, right, a e d. Segurando elas o Collector ganha velocidade, ao soltar volta a sua velocidade normal. 

Ao longo do jogo objetos caem do topo da tela até a base, o objetivo é se mover para coleta-los quando há a colisão, assim uma vez que o Collector colide com um dos objetos o jogador ganha 15 pontos (visualização no topo de tela), porém se deixar o objeto cair e não coleta-lo perde 10 pontos. Se o usuário chegar a uma pontuação negativa, o jogo é encerrado. Além disso, conforme o jogador ganha pontos os objetos irão cair mais rapidamente.

Por fim, há também o controle de alguns estados do jogo:
 - Inicialmente é necessário apertar ENTER para começar o jogo
 - Durante o jogo é possível apertar a tecla P para pausa-lo
 - Quando o jogo for encerrado, é possível apertar ENTER para começar um novo jogo.


 
 
