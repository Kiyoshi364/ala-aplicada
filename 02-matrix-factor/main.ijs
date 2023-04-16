NB. Note que isso é um arquivo de texto
NB. Estou usando 'j' como uma calculadora
NB. As respostas das contas vão estar comentadas logo abaixo

NB. Observações adicionais:
NB. * 'NB.' inicia um comentario tipo '#' em Julia ou '//' em C
NB. * a extensão usual é '.ijs',
NB.    mas está em '.txt' para ser legível no drive
NB. * Caso queira carregar isso em uma repl execute
NB.    (dentro do jconsole):
NB.    0!:0 < '/path/from/root'

NB. Uma vez carregada na repl defina 'script', ex:
NB. script =: '/path/from/root'
NB. e rode:
NB. reload''
reload = 3 : '0!:0 < script'

NB. ==================================================
NB. Vocabulario
NB. ==================================================

NB. Vou deixar uma cópia de explicações de "coisas estranhas" do J
NB. Cada "bloco" desse deve aparecer, no mínimo,
NB. logo antes da 1a vez em que é usado
NB. Recomendo pesquisar esse arquivo por: 'coisa estranha'
NB. Link das primitivas:
NB. https://code.jsoftware.com/wiki/NuVoc
NB. Sinta-se a vontade de pular o "Vocabulario"

NB. '=.' é atribuição (local)
NB. '=:' é atribuição (global)
NB. A principal diferênça é que global "exporta" a variável
NB. Se a linha começa com ']' ou '[' pode ignorar,
NB. isso é só para mostrar o resultado

NB. '_123' é um literal que representa '- 123'

NB. '1r2' é um literal que representa a fração '1/2'

NB. 'x +/ . * y' é multiplicação de matrizes 'x' e 'y'

NB. '-/ . * y' é o determinante de 'y'

NB. '|: y' é transposta de y

NB. 'x ~: y' é não-igual, em C: 'x != y'

NB. '# y' é o tamanho de 'y'
NB. (quantidade de elementos na última dimensão)

NB. ==================================================
NB. Questao 1
NB. ==================================================

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. '=.' é atribuição (local)
NB. '=:' é atribuição (global)
NB. A principal diferênça é que global "exporta" a variável
NB. Se a linha começa com ']' ou '[' pode ignorar,
NB. isso é só para mostrar o resultado

NB. '_123' é um literal que representa '- 123'

] A_q1a_ =: 4 5 $ 2 10 20 _2 0 3 15 30 _30 0 5 25 50 _5 0 7 35 70 _7 0
NB. 2 10 20  _2 0
NB. 3 15 30 _30 0
NB. 5 25 50  _5 0
NB. 7 35 70  _7 0

] B_q1a_ =: 4 2 $ 2 0 3 3 5 0 7 0
NB. 2 0
NB. 3 3
NB. 5 0
NB. 7 0

] C_q1a_ =: 2 5 $ 1 5 10 _1 0 0 0 0 _9 0
NB. 1 5 10 _1 0
NB. 0 0  0 _9 0

NB. 'x +/ . * y' é multiplicação de matrizes 'x' e 'y'
] diff_q1a_ =: A_q1a_ - (B_q1a_ +/ . * C_q1a_)
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

] A_q1b_ =: 3 5 $ 1 3 5 5000 31 4 5 9 9000 54 3 5 8 8000 53
NB. 1 3 5 5000 31
NB. 4 5 9 9000 54
NB. 3 5 8 8000 53

] B_q1b_ =: 3 3 $ 1 3 5 4 5 9 3 5 8
NB. 1 3 5
NB. 4 5 9
NB. 3 5 8

] C_q1b_ =: 3 5 $ 1 0 0 0 1 0 1 0 0 10 0 0 1 1000 0
NB. 1 0 0    0  1
NB. 0 1 0    0 10
NB. 0 0 1 1000  0

] diff_q1b_ =: A_q1b_ - (B_q1b_ +/ . * C_q1b_)
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0

NB. ==================================================
NB. Questao 2
NB. ==================================================

] U_q2_ =: 4 5 $ 2 10 20 200 _10 _3 _15 _30 _300 15 5 25 50 500 _25 7 35 70 700 _35
NB.  2  10  20  200 _10
NB. _3 _15 _30 _300  15
NB.  5  25  50  500 _25
NB.  7  35  70  700 _35

NB. Fatorando "no olho"
] B_q2_ =: 4 1 $ 2 _3 5 7
NB.  2
NB. _3
NB.  5
NB.  7

] C_q2_ =: 1 5 $ 1 5 10 100 _5
NB. 1 5 10 100 _5

] diff_q2_ =: U_q2_ - (B_q2_ +/ . * C_q2_)
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0

NB. B_q2_ é quanto cada usuário gosta de comédia
NB. C_q2_ é quanto cada filme é de comédia

NB. ==================================================
NB. Questao 3
NB. ==================================================

NB. Não precisa entender isso, olhe a resposta
] UF_q3_ =: (,. -.) 0.9 0.8 0.3 0.6 0
NB. 0.9 0.1
NB. 0.8 0.2
NB. 0.3 0.7
NB. 0.6 0.4
NB.   0   1

] MF_q3_ =: (,. -.) 0.9 0.1 0.5 0 0.2
NB. 0.9 0.1
NB. 0.1 0.9
NB. 0.5 0.5
NB.   0   1
NB. 0.2 0.8

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. '|: y' é transposta de y
] tabela_q3_ =: UF_q3_ +/ . * (|: MF_q3_)
NB. 0.82 0.18 0.5 0.1 0.26
NB. 0.74 0.26 0.5 0.2 0.32
NB. 0.34 0.66 0.5 0.7 0.62
NB. 0.58 0.42 0.5 0.4 0.44
NB.  0.1  0.9 0.5   1  0.8

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

NB. exemplo para 'User A' e 'User B', passo a passo
NB. vetores 'a' e 'b'
] a_q3_ =: 0 { tabela_q3_
NB. 0.82 0.18 0.5 0.1 0.26
] b_q3_ =: 1 { tabela_q3_
NB. 0.74 0.26 0.5 0.2 0.32

NB. a - b
a_q3_ - b_q3_
NB. 0.08 _0.08 0 _0.1 _0.06

NB. 'a - b' "ao quadrado"
*: a_q3_ - b_q3_
NB. 0.0064 0.0064 0 0.01 0.0036

NB. distancia de 'a' e 'b' ao quadrado
+/ *: a_q3_ - b_q3_
NB. 0.0264

NB. distancia de 'a' e 'b'
%: +/ *: a_q3_ - b_q3_
NB. 0.162481

NB. tudo de uma vez
dist_q3_ =: %:@:(+/)@:*:@:-~~
] dAB_q3_ =: (0 { tabela_q3_) dist_q3_ (1 { tabela_q3_)
NB. 0.162481

NB. Mas fazer isso a mão é muito chato
NB. Um pouco de "magia negra" para fazer tudo isso de uma vez
NB. Tabela de distâncias:
] userDists_q3_ =: (dist_q3_ & >)/~ f. <"_1 tabela_q3_
NB.        0 0.162481 0.974885 0.487442  1.46233
NB. 0.162481        0 0.812404 0.324962  1.29985
NB. 0.974885 0.812404        0 0.487442 0.487442
NB. 0.487442 0.324962 0.487442        0 0.974885
NB.  1.46233  1.29985 0.487442 0.974885        0

NB. observe que, como esperado, a diagonal é '0'

NB. A menor distância (ignorando '0') é
NB. 'x ~: y' é não-igual, em C: 'x != y'
] minDist_q3_ =: <./ (#~ ~:&0) , userDists_q3_
NB. 0.162481

NB. Os valores que são iguais a minDist (1: igual, 0: diferente)
minDist_q3_ = userDists_q3_
NB. 0 1 0 0 0
NB. 1 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0
NB. 0 0 0 0 0

NB. Conclui-se que os mais parecidos são 'A' e 'B'

NB. ==================================================
NB. Questao 4
NB. ==================================================

NB. TODO

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

] A_q4a_ =: (,: |.) 0 1 3
NB. 0 1 3
NB. 3 1 0

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

] A_q4b_ =: (,: |.) 0 1 3 2
NB. 0 1 3 2
NB. 2 3 1 0

NB. --------------------------------------------------
NB. Letra c
NB. --------------------------------------------------

NB. Ascii art
NB. '*' são pontos da matriz real
NB. '.' são pontos da reta "que aproxima"

NB. Letra a
NB. 3.5 | *           
NB. 3.0 | *           
NB. 2.5 |..           
NB. 2.0 |  ..         
NB. 1.5 |    .        
NB. 1.0 |    *.       
NB. 0.5 |      ..     
NB. 0.0 |        ..*  
NB.     +-------------
NB.       0  1  2  3  

NB. Letra b
NB. 3.5 |.            
NB. 3.0 | .  *        
NB. 2.5 |  ..         
NB. 2.0 | *  .        
NB. 1.5 |     ..      
NB. 1.0 |       .  *  
NB. 0.5 |        ..   
NB. 0.0 |       *  .  
NB.     +-------------
NB.       0  1  2  3  

NB. ==================================================
NB. Questao 5
NB. ==================================================

] V_q5_ =: 8 4 $ 1 1 1 1 1 2 3 4 11 12 13 14 101 102 103 104 11 21 31 41 2 3 4 5 13 10 7 4 2 2 2 3
NB.   1   1   1   1
NB.   1   2   3   4
NB.  11  12  13  14
NB. 101 102 103 104
NB.  11  21  31  41
NB.   2   3   4   5
NB.  13  10   7   4
NB.   2   2   2   3

NB. "No olho", percebemos que:
NB. v2 = v1 + 10 * v0
(2 { V_q5_) -: (1 { V_q5_) + (10 * (0 { V_q5_))
NB. 1

NB. v3 = v1 + 100 * v0
(3 { V_q5_) -: (1 { V_q5_) + (100 * (0 { V_q5_))
NB. 1

NB. v4 = v0 + 10 * v1
(4 { V_q5_) -: (0 { V_q5_) + (10 * (1 { V_q5_))
NB. 1

NB. v5 = v0 + v1
(5 { V_q5_) -: (0 { V_q5_) + (1 { V_q5_)
NB. 1

NB. v6 = 16 * v0 + (-3) * v1
(6 { V_q5_) -: (16 * (0 { V_q5_)) + _3 * (1 { V_q5_)
NB. 1

NB. v7 = 2 * v0 + [ 0 0 0 1 ]
(7 { V_q5_) -: (2 * (0 { V_q5_)) + 0 0 0 1
NB. 1
NB. Mas não dá para construir [ 0 0 0 1 ] usando v0 e v1
NB. (Falta prova formal?)

NB. Assim, construimos uma base para V
] base_q5_ =: 3 4 $ 1 1 1 1 1 2 3 4 0 0 0 1
NB. 1 1 1 1
NB. 1 2 3 4
NB. 0 0 0 1

NB. '# y' é o tamanho de 'y'
NB. (quantidade de elementos na última dimensão)
] vdim_q5_ =: # base_q5_
NB. 3

NB. Reconstruindo V
] coord_q5_ =: 8 3 $ 1 0 0 0 1 0 10 1 0 100 1 0 1 10 0 1 1 0 16 _3 0 2 0 1
NB.   1  0 0
NB.   0  1 0
NB.  10  1 0
NB. 100  1 0
NB.   1 10 0
NB.   1  1 0
NB.  16 _3 0
NB.   2  0 1

NB. Note que a ordem da multiplicação está "invertida",
NB. pois meus vetores são "deitados"
] V1_q5_ =: coord_q5_ +/ . * base_q5_
NB.   1   1   1   1
NB.   1   2   3   4
NB.  11  12  13  14
NB. 101 102 103 104
NB.  11  21  31  41
NB.   2   3   4   5
NB.  13  10   7   4
NB.   2   2   2   3

NB. ==================================================
NB. Questao 6
NB. ==================================================

NB. Notação:
NB. A^t: transposta de A

NB. Premissas:
NB. (B C)^t <==> (C^t) (B^t)

NB. Suponha que o posto de A é p, com as matrizes B e C.
NB. Pela premissa, sabemos que: A^t = (C^t) (B^t).
NB. Então, p é "um bom candidato" a posto de A^t.

NB. Analizando outras matrizes D e E tal que A^t = D E,
NB. temos alguns casos comparando sua "dimensão interna"
NB. (vamos chamar de q) com p:

NB. Caso 1 (q > p):
NB. não é "interessante", já que não "ganha" do (C^t) e (B^t)

NB. Caso 2 (q = p):
NB. não é "interessante", já que não "empata" com (C^t) (B^t)

NB. Caso 3 (q < p):
NB. não pode existir, pois pela definição de posto,
NB. p é o menor valor tal que "bla-bla-bla",
NB. mas acabamos de encontrar um melhor!
NB. Note que: A = E D

NB. Assim concluímos que:
NB. Se p é posto de A, p também é posto de A^t

NB. ==================================================
NB. Questao 7
NB. ==================================================

NB. Começa com 0 0 só para os indices ficarem iguais
] A_q7_ =: |: 7 2 $ 0 0 1 0 3 1 0 1 2 3 0 _3 _4 0
NB. 0 1 3 0 2  0 _4
NB. 0 0 1 1 3 _3  0

] base_q7_ =: |: 2 4 { |: A_q7_
NB. 3 2
NB. 1 3

NB. Descobrir isso é um problema de resolver um sistema linear
NB. (para cada vetor)
NB. mas eu fui chutando valores até "dar certo"

NB. Cada sistema têm a cara:
NB. { a2[j] * c[0] + a4[j] * c[1] = ai[j]
NB. onde i identifica um vetor
NB. e j identifica uma coordenada do vetor

NB. '1r2' é um literal que representa a fração '1/2'
] coord_q7_ =: |: 7 2 $ 0 0 3r7 _1r7 1 0 _2r7 3r7 0 1 6r7 _9r7 _12r7 4r7
NB. 0  3r7 1 _2r7 0 _6r7 _12r7
NB. 0 _1r7 0  3r7 1  9r7   4r7

NB. Converte para float
x:^:_1 coord_q7_
NB. 0  0.428571 1 _0.285714 0 _0.857143 _1.71429
NB. 0 _0.142857 0  0.428571 1   1.28571 0.571429

] diff_q7_ =: A_q7_ - (base_q7_ +/ . * coord_q7_)
NB. 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0

NB. ==================================================
NB. Questao 8
NB. ==================================================

NB. Letra a
NB. Os dois pacientes não têm sintomas em comum

NB. Letra b
NB. Não têm um paciente com os dois sintomas ao mesmo tempo

NB. Letra c
NB. Os dois pacientes têm os mesmos sintomas

NB. Letra d
NB. Se um paciente possui um sintoma,
NB. também possui o outro,
NB. e se um paciente não possui um sintoma,
NB. também não possui o outro.

NB. ==================================================
NB. Questao 9
NB. ==================================================

NB. Bandeira da Grecia
] A_q9_ =: 9 15 $ 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
NB. 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
NB. 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1
NB. 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0
NB. 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
NB. 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
NB. 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
NB. 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
NB. 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

NB. Ascii art
< A_q9_ { ' #'
NB. +---------------+
NB. |## ############|
NB. |## ##          |
NB. |     ##########|
NB. |## ##          |
NB. |## ############|
NB. |               |
NB. |###############|
NB. |               |
NB. |###############|
NB. +---------------+

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. TODO

NB. Rascunho
NB. 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
NB. 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1
NB. 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
NB. 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

NB. TODO

NB. --------------------------------------------------
NB. Letra c
NB. --------------------------------------------------

NB. TODO

NB. --------------------------------------------------
NB. Letra d
NB. --------------------------------------------------

NB. TODO

NB. ==================================================
NB. Questao 10
NB. ==================================================

NB. TODO

NB. ==================================================
NB. Questao 11
NB. ==================================================

NB. TODO

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

NB. --------------------------------------------------
NB. Letra c
NB. --------------------------------------------------

NB. ==================================================
NB. Questao 12
NB. ==================================================

image_q12_ =: 5 5 $ 0 1 0 1 0 0 1 0 1 0 0 0 0.5 0 0 1 0 0 0 1 0 1 1 1 0
NB. 0 1   0 1 0
NB. 0 1   0 1 0
NB. 0 0 0.5 0 0
NB. 1 0   0 0 1
NB. 0 1   1 1 0

NB. Ascii art
(2 * image_q12_) { ' *#'
NB.  # # 
NB.  # # 
NB.   *  
NB. #   #
NB.  ### 

NB. TODO

NB. ==================================================
NB. Questao 13
NB. ==================================================

NB. TODO

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

NB. ==================================================
NB. Questao 14
NB. ==================================================

NB. TODO

NB. ==================================================
NB. Questao 15
NB. ==================================================

NB. Nota: por motivos de conta (com a ajuda do computador)
NB. podemos usar q1, q2, q3 quaisquer
NB. (desde que sejam unitários e perpendiculares entre si).
NB. Por simplicidade, nos exemplos computacionais,
NB. q1, q2, q3 vão ser a base canônica

NB. Intuitivamente, isso acontece porque continuamos a ter
NB. três valores que não se misturam,
NB. e no futuro ainda podemos "substituir"
NB. o "valor de verdade" de q1, q2, q3

] base_q15_ =: 3 3 $ 1 0 0 0
NB. 1 0 0
NB. 0 1 0
NB. 0 0 1
] q1_q15_ =: 0 { |: base_q15_
NB. 1 0 0
] q2_q15_ =: 1 { |: base_q15_
NB. 0 1 0
] q3_q15_ =: 2 { |: base_q15_
NB. 0 0 1

] coords_q15_ =: 3 3 $ 3 4 5 0 7 6 0 0 9
NB. 3 4 5
NB. 0 7 6
NB. 0 0 9

NB. --------------------------------------------------
NB. Letra a
NB. --------------------------------------------------

NB. Observando o resultado da multiplicação:
] A_q15_ =: base_q15_ +/ . * coords_q15_
NB. 3 4 5
NB. 0 7 6
NB. 0 0 9

NB. Vêmos que:
NB. a1 = 3 * q1
] a1_q15_ =: 0 { |: A_q15_
NB. 3 0 0

NB. a2 = 4 * q1 + 7 * q2
] a2_q15_ =: 1 { |: A_q15_
NB. 4 7 0

NB. a3 = 5 * q1 + 6 * q2 + 9 * q3
] a3_q15_ =: 2 { |: A_q15_
NB. 5 6 9

NB. Podemos combinar a1 e a2 para dar (5 * q1 + 6 * q2)
NB. E necessariamente vai ficar faltando 9 * q3,
NB. pois q1, q2, q3 são perpendiculares entre si.
a3_q15_ - (11r21 * a1_q15_) + (6r7 * a2_q15_)
NB. 0 0 9

NB. Dessa forma, a distância entre a3 e o plano a1 a2:
NB. 9 * |q3| = 9 * 1 = 9

NB. --------------------------------------------------
NB. Letra b
NB. --------------------------------------------------

NB. |a1| = ((3 * |q1|)^2 + (0 * |q2|)^2 + (0 * |q3|)^2)^1/2
NB. ((3 * |q1|)^2)^1/2
NB. ((3 * 1)^2)^1/2
NB. (3^2)^1/2
NB. 3

*: a1_q15_
NB. 9 0 0
+/ *: a1_q15_
NB. 9
%: +/ *: a1_q15_
NB. 3

NB. --------------------------------------------------
NB. Letra c
NB. --------------------------------------------------

NB. Na letra a, descobrimos que
NB. a projeção de q3 no plano a1 a2 é
NB. (5 * q1 + 6 * q2)

NB. |5 * q1 + 6 * q2|
NB. ((5 * |q1|)^2 + (6 * |q2|)^2)^1/2
NB. (5^2 + 6^2)^1/2
NB. (25 + 36)^1/2
NB. (25 + 36)^1/2
NB. 61^1/2
NB. 7.81025

*: 5 6 0
NB. 25 36 0
+/ *: 5 6 0
NB. 61
%: +/ *: 5 6 0
NB. 7.81025

NB. --------------------------------------------------
NB. Letra d
NB. --------------------------------------------------

NB. (3 * 7 * 9) + (4 * 6 * 0) + (5 * 0 * 0) - (0 * 7 * 5) - (0 * 6 * 3) - (9 * 0 * 4)
NB. 3 * 7 * 9
NB. 189

NB. '-/ . * y' é o determinante de 'y'
-/ . * A_q15_
NB. 189

NB. --------------------------------------------------
NB. Letra e
NB. --------------------------------------------------

NB. Semelhante com a projeção de a3 no plano a1 a2,
NB. podemos combinar a1 para achar (4 * q1)
NB. E necessariamente vai ficar faltando 7 * q2,
NB. pois q1, q2 são perpendiculares entre si.
a2_q15_ - 4r3 * a1_q15_
NB. 0 7 0

NB. |4/3 * q1|
NB. ((4/3 * |q1|)^2)^1/2
NB. (4/3^2)^1/2
NB. 4/3

%: +/ *: 4r3 0 0
NB. 4r3

NB. --------------------------------------------------
NB. Letra f
NB. --------------------------------------------------

NB. |a3|
NB. ((5 * |q1|)^2 + (6 * |q2|)^2 + (9 * |q3|)^2)^1/2
NB. (5^2 + 6^2 + 9^2)^1/2
NB. (5^2 + 6^2 + 9^2)^1/2
NB. (25 36 81)^1/2
NB. (142)^1/2
NB. 11.9164

*: a3_q15_
NB. 25 36 81
+/ *: a3_q15_
NB. 142
%: +/ *: a3_q15_
NB. 11.9164

NB. --------------------------------------------------
NB. Letra g
NB. --------------------------------------------------

NB. Queremos uma matriz B, tal que:
NB. A B = B A = I

NB. *Chute*
NB. Tem um teorema que fala:
NB. Se uma matriz quadrada A tem <propriedade>
NB. então a sua inversa A^_1 é a transposta A^t.

NB. *Chute2*
NB. [ q1 | q2 | q3 ] tem <propriedade>,
NB. pois q1, q2, q3 são unitários e perpendiculares entre si.

NB. Dessa forma, a inversa de [ q1 | q2 | q3 ] é
NB. [ q1 | q2 | q3 ]^t

] baset_q15_ =: |: base_q15_
NB. 1 0 0
NB. 0 1 0
NB. 0 0 1

NB. Nota: Nossa base é a sua transposta

NB. --------------------------------------------------
NB. Letra h
NB. --------------------------------------------------

NB. TODO

