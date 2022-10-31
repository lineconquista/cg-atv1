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
 - Inicialmente é necessário apertar ENTER para começar o jogo.
 - Durante o jogo é possível apertar a tecla P para pausa-lo.
 - Quando o jogo for encerrado, é possível apertar ENTER para começar um novo jogo.
 
### Código

O código é composto por window.cpp, objects.cpp, collector.cpp e starlayers.cpp.

#### Starlayers

Utilizei o código desenvolvido na aula do jogo Asteroids para compor o background do meu jogo dessa atividade.

#### Collector

Possui como uma das variáveis privadas a m_translation que controla a posição x e y do collector na tela, m_currentColor que define a cor do collector e m_distance que controla a distância que será percorrida ao apertar as teclas de movimentação.

- create: Inicialização padrão do collector.
- paint: Renderização utilizando a variável m_sides para gerar um círculo e m_scale para controlar o tamanho do objeto.
- destroy: Remove o programa, buffers e vertex utilizados.
- update: Nessa função validamos o input que foi recebido do usuário, movimentamos o collector de acordo com o input utilizando a variável m_translation.x e m_distance. A cada frame que a tecla permanece apertada incrementamos m_distance de forma que o collector se mova cada vez mais rápido. No final chamamos a função check() para validar se houve colisão entre algum objeto e o collector, se houver colisão incrementamos os pontos em 15 e apagamos o objeto do array.

#### Object

Possui como uma das variáveis privadas a colors que é um array de possíveis cores que o objeto assumirá e a m_currentColor que é a cor definida aleatoriamente entre as opções contidas no array colors.

- create: Inicialização padrão do object.
- paint: Inicialmente nessa função validamos se possuímos a quantidade definida de objetos na tela, se o número atual for menor do que o esperado criamos um novo objeto com coordenada y no topo da tela e x definida de forma aleatória e adicionamos no array objects. Em seguida percorremos o array objects para renderizarmos em tela cada um.
- destroy: Remove o programa, buffers e vertex utilizados.
- update: Dado um período de tempo e se houver objetos no array, validamos a quantidade de pontos do jogo para definir o estado ou a velocidade com que os objetos caem na tela. Caso a pontuação seja negativa o jogo irá encerrar, mas caso seja positiva para cada range de pontos os objetos caem de forma mais rápida ou devagar. Por exemplo, se a pontuação for até 49, a distância percorrida será entre 0.01f e 0.08f, se for até 120 será entre 0.03f e 0.08f etc. Por fim, se o objeto cair, não for coletado e ultrapassar o limite da tela, removemos do array e subtraímos 10 pontos do usuário.

#### Window

Utilizado para inicializar objects, starlayers e collector e chamar as respectivas funções (update, destroy, create, paint). Além disso valida o estado do jogo para exibir as mensagens na tela referente ao Pause, GameOver e Novo Jogo.

 
 
