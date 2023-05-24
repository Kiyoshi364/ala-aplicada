NB. Note que isso é um arquivo de texto
NB. Estou usando 'j' como uma calculadora

NB. Observações adicionais:
NB. * 'NB.' inicia um comentario tipo '#' em Julia ou '//' em C
NB. * Caso queira carregar isso em uma repl execute
NB.    (dentro do jconsole):
NB.    0!:0 < 'path/to/main.ijs'

NB. Uma vez carregada na repl defina 'script', ex:
NB. script =: 'path/to/main.ijs'
NB. e rode:
NB. reload''
reload =: 3 : '0!:0 < script'

NB. Observe que existe um site para rodar J:
NB. https://jsoftware.github.io/j-playground/bin/html2/

NB. Uncomment this to use '+|-' for boxed
9!:7 '+++++++++|-'
NB. Uncomment this to use OEM chars for boxed
NB. 9!:7 [ 218 194 191 195 197 180 192 193 217 179 196 { a.
NB. Uncomment this to use unicode for boxed
NB. 9!:7 [ (16 + i. 11) { a.

NB. ==================================================
NB. Vocabulario
NB. ==================================================

NB. Dessa vez não tenho tempo para
NB. fazer uma coisa bonitinha,
NB. por favor olhe a última atividade
NB. (em ../02-matrix-factor/main.ijs)
NB. ou no NuVoc
NB. (em https://code.jsoftware.com/wiki/NuVoc)

NB. ==================================================
NB. Global Utils
NB. ==================================================

NB. 'prefix' startswithmask (str1 ; str2 ; str3 ; ...)
startswithmask =: ] #~ [ *./@:="1 #@[ {."1 >@]

NB. ==================================================
NB. Lendo arquivo
NB. ==================================================

NB. Isso é tipo um 'import'
0!:0 < './load_data.ijs'
0!:0 < './svd.ijs'

NB. Esses são a quantidade de caracteres em cada arquivo
$ rawdocs
NB. 9582

$ rawtokens
NB. 1993

$ rawmatrix
NB. 111534

NB. Mostra 10 primeiros arquivos processados
10 {. ,. docs
NB. +---------------------+
NB. |Thread/Condition.zig |
NB. +---------------------+
NB. |Thread/Futex.zig     |
NB. +---------------------+
NB. |Thread/Mutex.zig     |
NB. +---------------------+
NB. |Thread/ResetEvent.zig|
NB. +---------------------+
NB. |Thread/RwLock.zig    |
NB. +---------------------+
NB. |Thread/Semaphore.zig |
NB. +---------------------+
NB. |atomic/Atomic.zig    |
NB. +---------------------+
NB. |atomic/queue.zig     |
NB. +---------------------+
NB. |atomic/stack.zig     |
NB. +---------------------+
NB. |c/darwin/aarch64.zig |
NB. +---------------------+

NB. Mostra 10 primeiros tokens
10 {. ,. tokens
NB. +----------------------------+
NB. |tk_builtin                  |
NB. +----------------------------+
NB. |tk_bang                     |
NB. +----------------------------+
NB. |tk_pipe                     |
NB. +----------------------------+
NB. |tk_pipe_pipe                |
NB. +----------------------------+
NB. |tk_pipe_equal               |
NB. +----------------------------+
NB. |tk_equal                    |
NB. +----------------------------+
NB. |tk_equal_equal              |
NB. +----------------------------+
NB. |tk_equal_angle_bracket_right|
NB. +----------------------------+
NB. |tk_bang_equal               |
NB. +----------------------------+
NB. |tk_l_paren                  |
NB. +----------------------------+

NB. Mostra inicio 10x20 da matrix
20 {."1 ] 10 {. matrix
NB. 11  8 24 0 0  94  8  6  6 161 161 149 1 0 103 103  8  8 280 4
NB. 67 22 46 0 1 186 36 56 19 396 396 315 5 0 194 194 10 10 589 3
NB. 12  4 12 0 0  47  6  0  3 109 109  78 1 0  59  59  7  7 142 2
NB.  8  7  6 0 0  46  7  1  1 113 113  93 0 0  56  56  1  1 159 1
NB. 21  9  8 0 0  75 18 10  2 168 168 137 0 0  65  65  3  3 249 2
NB.  4  0  4 0 0  19  2  0  0  25  25  29 0 0  12  12  1  1  45 2
NB. 74  1 85 0 0  61 15 34  6 457 457 175 0 0 129 129 52 52 400 0
NB. 17 12 22 0 0 103 14  0  4 188 188 153 0 0  69  69  9  9 282 3
NB. 18  0 10 0 0  51  3  0  3  76  76  62 0 0  41  41  2  2 117 3
NB.  0  0  0 0 0   2  0  0  0   0   0   2 0 0   2   2  1  1   0 0

NB. ==================================================
NB. Análise exploratória
NB. ==================================================

NB. Dimensões

$ docs
NB. 467

$ tokens
NB. 112

$ matrix
NB. 467 112


NB. Tem alguma coluna vazia (token não usado)
+./ matrix > 0
NB. 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

NB. As colunas vazias
] emptycols =: (-. # i.@#) +./ matrix > 0
NB. 42 43 57 63

NB. Os tokens das colunas vazias
,. emptycols { tokens
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe                              |
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe_equal                        |
NB. +----------------------------------------------+
NB. |tk_angle_bracket_angle_bracket_left_pipe_equal|
NB. +----------------------------------------------+
NB. |tk_keyword_addrspace                          |
NB. +----------------------------------------------+

NB. Média de tokens por documento
(+/ % #) +/"1 matrix
NB. 2255.42

NB. Variância de tokens por documento
(<:@# %~ (+/ % #) *:@- ]) +/"1 matrix
NB. 1.26895e7

NB. Mediana de tokens por documento
(((= <.) {:: ] ;~ <. , >.)@-:@# (+/ % #)@:{ /: { ]) +/"1 matrix
NB. 905.5

NB. Moda de tokens por documento
(((= <.) {:: ] ;~ <. , >.)@-:@# (+/ % #)@:{ /: ~.@{ ]) +/"1 matrix
NB. 905.5

NB. ==================================================
NB. Análise do svd ("do jeito que está")
NB. ==================================================

NB. Isso é tipo um 'import'
0!:0 < './normal.gen.ijs'

NB. Assert que resultado dá 'matrix'
NB. 'norm_frob( matrix - U*S*V )'
+/&.:*: , matrix - unsvd_svd_ (u_normal_ ; s_normal_ ; v_normal_)
NB. 0.0542695

NB. Quantos valores são maiores que 1e_5
+/ 1e_5 < s_normal_
NB. 104

NB. 'norm_frob( matrix - U*S*V )'
NB. Usando 103 linhas/colunas
+/&.:*: , matrix - unsvd_svd_ ((103 {."1 u_normal_) ; (103 {. s_normal_) ; (103 {. v_normal_))
NB. 0.230284

NB. 'norm_frob( matrix - U*S*V )'
NB. Usando 104 linhas/colunas
+/&.:*: , matrix - unsvd_svd_ ((104 {."1 u_normal_) ; (104 {. s_normal_) ; (104 {. v_normal_))
NB. 0.0542695

NB. 'norm_frob( matrix - U*S*V )'
NB. Usando 105 linhas/colunas
+/&.:*: , matrix - unsvd_svd_ ((105 {."1 u_normal_) ; (105 {. s_normal_) ; (105 {. v_normal_))
NB. 0.0542695

NB. Em cada componente (v0, v1, v2, ...),
NB. (Os dados estão nas linhas)
NB. ordena os tokens de MAIS importantes para MENOS
NB. (importantes em tamanho)
NB. Aqui temos os indexes, não os valores
topv_normal_ =: \:"1 *: v_normal_

NB. Os 10 tokens MAIS importantes para cada componente
top10v_normal_ =: 10 {."1 topv_normal_

NB. Printa para os 10 tokens MAIS importantes
NB. para os primeiros componente
10 {. top10v_normal_
NB.  48 18   9 10  11   5  45 14  15  76
NB.  18 48  11  5  45 109  49 14  15   7
NB.  48  9  10 11   5  45  18 76 109 105
NB.   5 96  76 48   9  10 109 49  18  11
NB.   5 96  49 76 109  48  18  9  10  11
NB.   5 45   7  9  10 105  49 11 109  18
NB.   7 16  17 98   5  82  14 15  11   9
NB. 105 11  16 17  45  14  15 18  98  96
NB.  14 15 105 11   7  18  45  0  16  17
NB.  45 16  17  7  82  98  14 15  37  76

NB. Então para v0, o token 48 é o mais importante!

NB. Os valores são únicos?
(~. -: ]) , 10 {. top10v_normal_
NB. 0

NB. '0' são os valores que já apareceram antes
($ $ [: ~: ,) 10 {. top10v_normal_
NB. 1 1 1 1 1 1 1 1 1 1
NB. 0 0 0 0 0 1 1 0 0 1
NB. 0 0 0 0 0 0 0 0 0 1
NB. 0 1 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 1 1 1 0 1 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 1 0 0
NB. 0 0 0 0 0 0 0 0 1 0

NB. Os 10 tokens MAIS importantes para v0
,. tokens {~ 0 { top10v_normal_
NB. +----------------+
NB. |tk_comma        |
NB. +----------------+
NB. |tk_period       |
NB. +----------------+
NB. |tk_l_paren      |
NB. +----------------+
NB. |tk_r_paren      |
NB. +----------------+
NB. |tk_semicolon    |
NB. +----------------+
NB. |tk_equal        |
NB. +----------------+
NB. |tk_colon        |
NB. +----------------+
NB. |tk_l_brace      |
NB. +----------------+
NB. |tk_r_brace      |
NB. +----------------+
NB. |tk_keyword_const|
NB. +----------------+

NB. Os 10 tokens MENOS importantes para cada componente
bot10v_normal_ =: _10 |.@{."1 topv_normal_

NB. Printa para os 10 tokens MENOS importantes
NB. para os primeiros componente
10 {. bot10v_normal_
NB. 63 57 43 42 99 83 29  36  13 30
NB. 63 57 43 42 99 83 29  36  34 13
NB. 63 57 43 42 99 83 34  36  29 67
NB. 63 57 43 42 13 99 83  29  30 36
NB. 63 57 43 42 90 13 34  38 104 99
NB. 63 57 43 42 29 36 99  83  90 34
NB. 63 57 43 42 99 83 29 104  90 36
NB. 63 57 43 42 99 83 36  35  29 90
NB. 63 57 43 42 36 97 99  83  30 29
NB. 63 57 43 42 36 53 99  83  29 13

NB. Nota: as colunas vazias são as MENOS importantes
emptycols
NB. 42 43 57 63

NB. Os 10 tokens MENOS importantes para v0
,. tokens {~ 0 { bot10v_normal_
NB. +----------------------------------------------+
NB. |tk_keyword_addrspace                          |
NB. +----------------------------------------------+
NB. |tk_angle_bracket_angle_bracket_left_pipe_equal|
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe_equal                        |
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe                              |
NB. +----------------------------------------------+
NB. |tk_keyword_linksection                        |
NB. +----------------------------------------------+
NB. |tk_keyword_export                             |
NB. +----------------------------------------------+
NB. |tk_plus_pipe                                  |
NB. +----------------------------------------------+
NB. |tk_minus_pipe_equal                           |
NB. +----------------------------------------------+
NB. |tk_percent_equal                              |
NB. +----------------------------------------------+
NB. |tk_plus_pipe_equal                            |
NB. +----------------------------------------------+

NB. Lista de pares: valor da projeção na 1a componente, doc
docsv0_normal_ =: (\: { docs ;"0~ ]) (0 { |: u_normal_) * s_normal_

NB. Os 10 valores maiores:
10 {. docsv0_normal_
NB. +-------+---------------------------------------+
NB. |398.153|Thread/Condition.zig                   |
NB. +-------+---------------------------------------+
NB. |351.892|Thread/Futex.zig                       |
NB. +-------+---------------------------------------+
NB. |80.9119|atomic/Atomic.zig                      |
NB. +-------+---------------------------------------+
NB. |65.0046|Thread/Mutex.zig                       |
NB. +-------+---------------------------------------+
NB. |59.3488|Thread/RwLock.zig                      |
NB. +-------+---------------------------------------+
NB. |51.2347|Thread/ResetEvent.zig                  |
NB. +-------+---------------------------------------+
NB. |34.6307|atomic/queue.zig                       |
NB. +-------+---------------------------------------+
NB. |26.831 |c/tokenizer.zig                        |
NB. +-------+---------------------------------------+
NB. |24.4865|c/darwin.zig                           |
NB. +-------+---------------------------------------+
NB. |20.628 |compress/deflate/huffman_bit_writer.zig|
NB. +-------+---------------------------------------+

NB. Nota: idealmente eu, a priori,
NB. já teria uma coloração para os documentos
NB. mas o mais próximo disso que tenho agora
NB. são as pastas onde eles estão.

NB. Os 6 arquivos da pasta de 'Thread':
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'Thread/' startswithmask ]) 1 {"1 ]) docsv0_normal_
NB. +--+-------+---------------------+
NB. |0 |398.153|Thread/Condition.zig |
NB. +--+-------+---------------------+
NB. |1 |351.892|Thread/Futex.zig     |
NB. +--+-------+---------------------+
NB. |3 |65.0046|Thread/Mutex.zig     |
NB. +--+-------+---------------------+
NB. |4 |59.3488|Thread/RwLock.zig    |
NB. +--+-------+---------------------+
NB. |5 |51.2347|Thread/ResetEvent.zig|
NB. +--+-------+---------------------+
NB. |20|7.78785|Thread/Semaphore.zig |
NB. +--+-------+---------------------+

NB. Os 3 arquivos da pasta de 'atomic':
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'atomic/' startswithmask ]) 1 {"1 ]) docsv0_normal_
NB. +--+-------+-----------------+
NB. |2 |80.9119|atomic/Atomic.zig|
NB. +--+-------+-----------------+
NB. |6 |34.6307|atomic/queue.zig |
NB. +--+-------+-----------------+
NB. |11|12.9739|atomic/stack.zig |
NB. +--+-------+-----------------+

NB. Os 17 arquivos da pasta de 'c',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'c/' startswithmask ]) 1 {"1 ]) docsv0_normal_
NB. +--+--------+--------------------+
NB. |7 |26.831  |c/tokenizer.zig     |
NB. +--+--------+--------------------+
NB. |8 |24.4865 |c/darwin.zig        |
NB. +--+--------+--------------------+
NB. |10|14.3151 |c/freebsd.zig       |
NB. +--+--------+--------------------+
NB. |12|12.6383 |c/dragonfly.zig     |
NB. +--+--------+--------------------+
NB. |13|11.3699 |c/openbsd.zig       |
NB. +--+--------+--------------------+
NB. |14|10.4063 |c/solaris.zig       |
NB. +--+--------+--------------------+
NB. |15|9.78023 |c/netbsd.zig        |
NB. +--+--------+--------------------+
NB. |18|8.15717 |c/haiku.zig         |
NB. +--+--------+--------------------+
NB. |24|4.54433 |c/linux.zig         |
NB. +--+--------+--------------------+
NB. |33|1.83628 |c/wasi.zig          |
NB. +--+--------+--------------------+
NB. |36|1.51798 |c/windows.zig       |
NB. +--+--------+--------------------+
NB. |40|1.20931 |c/darwin/x86_64.zig |
NB. +--+--------+--------------------+
NB. |54|0.711182|c/minix.zig         |
NB. +--+--------+--------------------+
NB. |58|0.552048|c/darwin/aarch64.zig|
NB. +--+--------+--------------------+
NB. |64|0.399385|c/emscripten.zig    |
NB. +--+--------+--------------------+
NB. |68|0.346221|c/fuchsia.zig       |
NB. +--+--------+--------------------+
NB. |70|0.293982|c/hermit.zig        |
NB. +--+--------+--------------------+

NB. Eles estão um pouco longe

NB. Os 15 arquivos da pasta de 'compress/deflate',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'compress/deflate/' startswithmask ]) 1 {"1 ]) docsv0_normal_
NB. +--+---------+---------------------------------------+
NB. |9 |20.628   |compress/deflate/huffman_bit_writer.zig|
NB. +--+---------+---------------------------------------+
NB. |19|7.79343  |compress/deflate/compressor.zig        |
NB. +--+---------+---------------------------------------+
NB. |21|7.03392  |compress/deflate/decompressor.zig      |
NB. +--+---------+---------------------------------------+
NB. |25|4.48492  |compress/deflate/dict_decoder.zig      |
NB. +--+---------+---------------------------------------+
NB. |26|4.30082  |compress/deflate/deflate_fast.zig      |
NB. +--+---------+---------------------------------------+
NB. |27|4.07851  |compress/deflate/compressor_test.zig   |
NB. +--+---------+---------------------------------------+
NB. |35|1.6555   |compress/deflate/huffman_code.zig      |
NB. +--+---------+---------------------------------------+
NB. |39|1.32417  |compress/deflate/token.zig             |
NB. +--+---------+---------------------------------------+
NB. |43|1.1191   |compress/deflate/deflate_fast_test.zig |
NB. +--+---------+---------------------------------------+
NB. |62|0.456167 |compress/deflate/bits_utils.zig        |
NB. +--+---------+---------------------------------------+
NB. |85|0.0758988|compress/deflate/mem_utils.zig         |
NB. +--+---------+---------------------------------------+
NB. |87|0.0546395|compress/deflate/deflate_const.zig     |
NB. +--+---------+---------------------------------------+

NB. Os 10 valores menores:
_10 {. docsv0_normal_
NB. +-----------+---------------------+
NB. |1.54694e_62|http.zig             |
NB. +-----------+---------------------+
NB. |1.23343e_62|x/os/socket_posix.zig|
NB. +-----------+---------------------+
NB. |1.20739e_62|x/os/socket.zig      |
NB. +-----------+---------------------+
NB. |1.20588e_62|std.zig              |
NB. +-----------+---------------------+
NB. |1.06004e_62|once.zig             |
NB. +-----------+---------------------+
NB. |9.5427e_63 |x/os/io.zig          |
NB. +-----------+---------------------+
NB. |8.7532e_63 |zig.zig              |
NB. +-----------+---------------------+
NB. |2.80772e_63|x/net/ip.zig         |
NB. +-----------+---------------------+
NB. |2.53002e_63|start_windows_tls.zig|
NB. +-----------+---------------------+
NB. |8.06665e_64|x.zig                |
NB. +-----------+---------------------+

NB. Os 9 arquivos que começam com 'x',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'x' startswithmask ]) 1 {"1 ]) docsv0_normal_
NB. +---+-----------+-----------------------+
NB. |444|6.45159e_62|x/net/bpf.zig          |
NB. +---+-----------+-----------------------+
NB. |451|1.96968e_62|x/os/net.zig           |
NB. +---+-----------+-----------------------+
NB. |452|1.88528e_62|x/net/tcp.zig          |
NB. +---+-----------+-----------------------+
NB. |455|1.85051e_62|x/os/socket_windows.zig|
NB. +---+-----------+-----------------------+
NB. |458|1.23343e_62|x/os/socket_posix.zig  |
NB. +---+-----------+-----------------------+
NB. |459|1.20739e_62|x/os/socket.zig        |
NB. +---+-----------+-----------------------+
NB. |462|9.5427e_63 |x/os/io.zig            |
NB. +---+-----------+-----------------------+
NB. |464|2.80772e_63|x/net/ip.zig           |
NB. +---+-----------+-----------------------+
NB. |466|8.06665e_64|x.zig                  |
NB. +---+-----------+-----------------------+

NB. ==================================================
NB. Análise do svd (centralizado)
NB. ==================================================

NB. Isso é tipo um 'import'
0!:0 < './center.gen.ijs'

NB. 'centered' é a 'matrix' centralizada
NB. A média dos docs tem todas os tokens menores que 1e_13
*./ 1e_13 < | (+/ % #) centered
NB. 1

NB. Assert que resultado dá 'centered'
NB. 'norm_frob( matrix - U*S*V )'
+/&.:*: , centered - unsvd_svd_ (u_center_ ; s_center_ ; v_center_)
NB. 0.0584999

NB. Quantos valores são maiores que 1e_5
+/ 1e_5 < s_center_
NB. 104

NB. 'norm_frob( centered - U*S*V )'
NB. Usando 103 linhas/colunas
+/&.:*: , centered - unsvd_svd_ ((103 {."1 u_center_) ; (103 {. s_center_) ; (103 {. v_center_))
NB. 0.230474

NB. 'norm_frob( centered - U*S*V )'
NB. Usando 104 linhas/colunas
+/&.:*: , centered - unsvd_svd_ ((104 {."1 u_center_) ; (104 {. s_center_) ; (104 {. v_center_))
NB. 0.0584999

NB. 'norm_frob( centered - U*S*V )'
NB. Usando 105 linhas/colunas
+/&.:*: , centered - unsvd_svd_ ((105 {."1 u_center_) ; (105 {. s_center_) ; (105 {. v_center_))
NB. 0.0584999

NB. Em cada componente (v0, v1, v2, ...),
NB. (Os dados estão nas linhas)
NB. ordena os tokens de MAIS importantes para MENOS
NB. (importantes em tamanho)
NB. Aqui temos os indexes, não os valores
topv_center_ =: \:"1 *: v_center_

NB. Eles são iguais?
topv_center_ -: topv_normal_
NB. 0

NB. Os 10 tokens MAIS importantes para cada componente
top10v_center_ =: 10 {."1 topv_center_

NB. Printa para os 10 tokens MAIS importantes
NB. para os primeiros componente
10 {. top10v_center_
NB.  48 18   9 10  11   5  45 14  15  76
NB.  18 48  11  5  45 109  49 14  15   7
NB.  48  9  10 11   5  45 109 18  76 105
NB.   5 96  76 48   9  10 109 18  49  11
NB.   5 96  49 76 109  48  18  9  10  11
NB.   5 45   7  9  10 105  49 11 109  18
NB.   7 16  17 98  82   5  14 15  11   9
NB. 105 16  17 11  14  15  45 18  98  96
NB.  14 15 105 11   7  18  45  0  16  17
NB.  45 16  17  7  82  98  14 15  37  11

NB. '1' são valores iguais entre o centralizado e o normal
(10 {. top10v_center_) = (10 {. top10v_normal_)
NB. 1 1 1 1 1 1 1 1 1 1
NB. 1 1 1 1 1 1 1 1 1 1
NB. 1 1 1 1 1 1 0 0 0 1
NB. 1 1 1 1 1 1 1 0 0 1
NB. 1 1 1 1 1 1 1 1 1 1
NB. 1 1 1 1 1 1 1 1 1 1
NB. 1 1 1 1 0 0 1 1 1 1
NB. 1 0 0 0 0 0 0 1 1 1
NB. 1 1 1 1 1 1 1 1 1 1
NB. 1 1 1 1 1 1 1 1 1 0

NB. Os valores são únicos?
(~. -: ]) , 10 {. top10v_center_
NB. 0

NB. '0' são os valores que já apareceram antes
($ $ [: ~: ,) 10 {. top10v_center_
NB. 1 1 1 1 1 1 1 1 1 1
NB. 0 0 0 0 0 1 1 0 0 1
NB. 0 0 0 0 0 0 0 0 0 1
NB. 0 1 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 1 1 1 1 0 0 0 0 0
NB. 0 0 0 0 0 0 0 0 0 0
NB. 0 0 0 0 0 0 0 1 0 0
NB. 0 0 0 0 0 0 0 0 1 0

NB. Os 10 tokens MAIS importantes para v0
,. tokens {~ 0 { top10v_center_
NB. +----------------+
NB. |tk_comma        |
NB. +----------------+
NB. |tk_period       |
NB. +----------------+
NB. |tk_l_paren      |
NB. +----------------+
NB. |tk_r_paren      |
NB. +----------------+
NB. |tk_semicolon    |
NB. +----------------+
NB. |tk_equal        |
NB. +----------------+
NB. |tk_colon        |
NB. +----------------+
NB. |tk_l_brace      |
NB. +----------------+
NB. |tk_r_brace      |
NB. +----------------+
NB. |tk_keyword_const|
NB. +----------------+

NB. Os 10 tokens MENOS importantes para cada componente
bot10v_center_ =: _10 |.@{."1 topv_center_

NB. Printa para os 10 tokens MENOS importantes
NB. para os primeiros componente
10 {. bot10v_center_
NB. 63 57 43 42 29  36  38 34 104 30
NB. 63 57 43 42 99  83  29 36  34 13
NB. 63 57 43 42 97  29  36 99  83 67
NB. 63 57 43 42 29  30  36 41  99 83
NB. 63 57 43 42 90 104  29 99  13 83
NB. 63 57 43 42 29  36  35 90  41 13
NB. 63 57 43 42 29  36 104 90  97 99
NB. 63 57 43 42 36  29  99 35  83 90
NB. 63 57 43 42 36  99  83 30  97 29
NB. 63 57 43 42 36  99  83 58  41 29

NB. Nota: as colunas vazias são as MENOS importantes
emptycols
NB. 42 43 57 63

NB. Os 10 tokens MENOS importantes para v0
,. tokens {~ 0 { bot10v_center_
NB. +----------------------------------------------+
NB. |tk_keyword_addrspace                          |
NB. +----------------------------------------------+
NB. |tk_angle_bracket_angle_bracket_left_pipe_equal|
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe_equal                        |
NB. +----------------------------------------------+
NB. |tk_asterisk_pipe                              |
NB. +----------------------------------------------+
NB. |tk_plus_pipe                                  |
NB. +----------------------------------------------+
NB. |tk_minus_pipe_equal                           |
NB. +----------------------------------------------+
NB. |tk_asterisk_equal                             |
NB. +----------------------------------------------+
NB. |tk_minus_percent_equal                        |
NB. +----------------------------------------------+
NB. |tk_keyword_threadlocal                        |
NB. +----------------------------------------------+
NB. |tk_plus_pipe_equal                            |
NB. +----------------------------------------------+

NB. Lista de pares: valor da projeção na 1a componente, doc
docsv0_center_ =: (\: { docs ;"0~ ]) (0 { |: u_center_) * s_center_

NB. Os 10 valores maiores:
10 {. docsv0_center_
NB. +-------+---------------------+
NB. |221.832|Thread/Condition.zig |
NB. +-------+---------------------+
NB. |139.124|Thread/Mutex.zig     |
NB. +-------+---------------------+
NB. |99.1465|Thread/ResetEvent.zig|
NB. +-------+---------------------+
NB. |81.334 |Thread/Semaphore.zig |
NB. +-------+---------------------+
NB. |47.1457|Thread/RwLock.zig    |
NB. +-------+---------------------+
NB. |43.1411|c/darwin/aarch64.zig |
NB. +-------+---------------------+
NB. |34.4779|c/darwin/x86_64.zig  |
NB. +-------+---------------------+
NB. |32.0734|atomic/stack.zig     |
NB. +-------+---------------------+
NB. |24.5427|c/emscripten.zig     |
NB. +-------+---------------------+
NB. |19.0746|atomic/queue.zig     |
NB. +-------+---------------------+

NB. Nota: idealmente eu, a priori,
NB. já teria uma coloração para os documentos
NB. mas o mais próximo disso que tenho agora
NB. são as pastas onde eles estão.

NB. Os 6 arquivos da pasta de 'Thread':
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'Thread/' startswithmask ]) 1 {"1 ]) docsv0_center_
NB. +---+--------+---------------------+
NB. |0  |221.832 |Thread/Condition.zig |
NB. +---+--------+---------------------+
NB. |1  |139.124 |Thread/Mutex.zig     |
NB. +---+--------+---------------------+
NB. |2  |99.1465 |Thread/ResetEvent.zig|
NB. +---+--------+---------------------+
NB. |3  |81.334  |Thread/Semaphore.zig |
NB. +---+--------+---------------------+
NB. |4  |47.1457 |Thread/RwLock.zig    |
NB. +---+--------+---------------------+
NB. |466|_133.818|Thread/Futex.zig     |
NB. +---+--------+---------------------+

NB. Os 3 arquivos da pasta de 'atomic':
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'atomic/' startswithmask ]) 1 {"1 ]) docsv0_center_
NB. +---+--------+-----------------+
NB. |7  |32.0734 |atomic/stack.zig |
NB. +---+--------+-----------------+
NB. |9  |19.0746 |atomic/queue.zig |
NB. +---+--------+-----------------+
NB. |465|_24.4867|atomic/Atomic.zig|
NB. +---+--------+-----------------+

NB. Os 17 arquivos da pasta de 'c',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'c/' startswithmask ]) 1 {"1 ]) docsv0_center_
NB. +---+---------+--------------------+
NB. |5  |43.1411  |c/darwin/aarch64.zig|
NB. +---+---------+--------------------+
NB. |6  |34.4779  |c/darwin/x86_64.zig |
NB. +---+---------+--------------------+
NB. |8  |24.5427  |c/emscripten.zig    |
NB. +---+---------+--------------------+
NB. |10 |19.0019  |c/fuchsia.zig       |
NB. +---+---------+--------------------+
NB. |11 |17.3726  |c/hermit.zig        |
NB. +---+---------+--------------------+
NB. |12 |13.968   |c/minix.zig         |
NB. +---+---------+--------------------+
NB. |13 |10.6519  |c/wasi.zig          |
NB. +---+---------+--------------------+
NB. |16 |4.37565  |c/windows.zig       |
NB. +---+---------+--------------------+
NB. |19 |2.7326   |c/linux.zig         |
NB. +---+---------+--------------------+
NB. |55 |0.0511927|c/haiku.zig         |
NB. +---+---------+--------------------+
NB. |449|_3.58343 |c/netbsd.zig        |
NB. +---+---------+--------------------+
NB. |451|_3.64223 |c/dragonfly.zig     |
NB. +---+---------+--------------------+
NB. |456|_5.70234 |c/openbsd.zig       |
NB. +---+---------+--------------------+
NB. |457|_5.71785 |c/solaris.zig       |
NB. +---+---------+--------------------+
NB. |459|_6.3872  |c/freebsd.zig       |
NB. +---+---------+--------------------+
NB. |462|_16.1594 |c/darwin.zig        |
NB. +---+---------+--------------------+
NB. |463|_18.1354 |c/tokenizer.zig     |
NB. +---+---------+--------------------+

NB. Os 15 arquivos da pasta de 'compress/deflate',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'compress/deflate/' startswithmask ]) 1 {"1 ]) docsv0_center_
NB. +---+---------+---------------------------------------+
NB. |14 |5.84288  |compress/deflate/deflate_const.zig     |
NB. +---+---------+---------------------------------------+
NB. |15 |4.98577  |compress/deflate/bits_utils.zig        |
NB. +---+---------+---------------------------------------+
NB. |17 |3.05699  |compress/deflate/mem_utils.zig         |
NB. +---+---------+---------------------------------------+
NB. |18 |2.74873  |compress/deflate/deflate_fast_test.zig |
NB. +---+---------+---------------------------------------+
NB. |25 |1.38915  |compress/deflate/token.zig             |
NB. +---+---------+---------------------------------------+
NB. |26 |1.34493  |compress/deflate/huffman_code.zig      |
NB. +---+---------+---------------------------------------+
NB. |437|_0.162457|compress/deflate/compressor_test.zig   |
NB. +---+---------+---------------------------------------+
NB. |443|_0.803005|compress/deflate/deflate_fast.zig      |
NB. +---+---------+---------------------------------------+
NB. |446|_1.29131 |compress/deflate/dict_decoder.zig      |
NB. +---+---------+---------------------------------------+
NB. |453|_3.89195 |compress/deflate/decompressor.zig      |
NB. +---+---------+---------------------------------------+
NB. |454|_4.002   |compress/deflate/compressor.zig        |
NB. +---+---------+---------------------------------------+
NB. |464|_21.1523 |compress/deflate/huffman_bit_writer.zig|
NB. +---+---------+---------------------------------------+

NB. Os 10 valores menores:
_10 {. docsv0_center_
NB. +--------+---------------------------------------+
NB. |_5.71785|c/solaris.zig                          |
NB. +--------+---------------------------------------+
NB. |_5.79389|crypto/pcurves/p256/p256_scalar_64.zig |
NB. +--------+---------------------------------------+
NB. |_6.3872 |c/freebsd.zig                          |
NB. +--------+---------------------------------------+
NB. |_9.6918 |crypto/pcurves/p384/p384_scalar_64.zig |
NB. +--------+---------------------------------------+
NB. |_9.91862|crypto/pcurves/p384/p384_64.zig        |
NB. +--------+---------------------------------------+
NB. |_16.1594|c/darwin.zig                           |
NB. +--------+---------------------------------------+
NB. |_18.1354|c/tokenizer.zig                        |
NB. +--------+---------------------------------------+
NB. |_21.1523|compress/deflate/huffman_bit_writer.zig|
NB. +--------+---------------------------------------+
NB. |_24.4867|atomic/Atomic.zig                      |
NB. +--------+---------------------------------------+
NB. |_133.818|Thread/Futex.zig                       |
NB. +--------+---------------------------------------+

NB. Os 9 arquivos que começam com 'x',
NB. print: (sorted_index ; valor ; doc)
,. (] (] ;"_1 {~) ] (] i. 'x' startswithmask ]) 1 {"1 ]) docsv0_center_
NB. +---+------------+-----------------------+
NB. |323|3.74081e_62 |x.zig                  |
NB. +---+------------+-----------------------+
NB. |325|2.81542e_62 |x/net/ip.zig           |
NB. +---+------------+-----------------------+
NB. |331|1.7445e_62  |x/os/io.zig            |
NB. +---+------------+-----------------------+
NB. |334|1.23337e_62 |x/os/socket.zig        |
NB. +---+------------+-----------------------+
NB. |335|1.01054e_62 |x/os/socket_posix.zig  |
NB. +---+------------+-----------------------+
NB. |336|8.13289e_63 |x/net/tcp.zig          |
NB. +---+------------+-----------------------+
NB. |337|4.15495e_63 |x/os/net.zig           |
NB. +---+------------+-----------------------+
NB. |338|8.07177e_64 |x/os/socket_windows.zig|
NB. +---+------------+-----------------------+
NB. |344|_3.78948e_62|x/net/bpf.zig          |
NB. +---+------------+-----------------------+

NB. ==================================================
NB. Desenhos
NB. ==================================================

NB. Olhe 'images/'


NB. ==================================================
NB. Conclusão
NB. ==================================================

NB. Não tive muita conclusão

NB. O sigma tinha 104 valores significativos

NB. Os meus documentos não estinham uma cor bem definida

NB. Não tive tempo de rodar K-means

NB. Não sei colorir os pontos
NB. (fazer isso ajudaria mais
NB. se eu já tivesse os documentos coloridos)

NB. De alguma forma "mágica" a primeira dimensão
NB. foi ótima para achar os códigos da pasta 'Threads/'
NB. e da pasta 'x/' (ficou melhor depois de centralizado)

NB. Talvez normalizar os dados ajude mais.
NB. Imagino que arquivos muito grandes/pequenos
NB. podem ter recebido algum tratatamento especial.
