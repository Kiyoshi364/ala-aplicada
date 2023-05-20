NB. ==================================================
NB. SVD funcs (adapdatas da lista 02)
NB. ==================================================

NB. A entrada é da forma 'A loopmqa_svd_ (B ; C)'
loopmqa_svd_ =: 4 : 0
  A =. [
  oldB =. (0&{::)@]
  oldC =. (1&{::)@]      NB. unused
  newC =. A %. oldB
  newBWithNewC =. ] ;~ A %.&.|: ]
  NB. aqui vêm o retorno
  x (A newBWithNewC newC) y
)

NB. A entrada é da forma 'max_its mqa_svd_ (A ; B ; C)'
NB. ou 'mqa_svd_ (A ; B ; C)'
mqa_svd_ =: (5000 $: ]) : (4 : 0)
  packl =. [ (; <) 1 }. ]
  packr =. {::~&0@]
  loop =. ] (0&<:@(0&{::)@] {:: [ (;<) ]) (0 <:@{:: ]) (;<) [ loopmqa_svd_ 1&{::@]
  unpack =. 1 {:: ]
  unpack x (packr loop^:_ packl) y

  NB. x (packr (] (0&<:@(0&{::)@] {:: [ (;<) ]) (0 <:@{:: ]) (;<) [ loopmqa_svd_ 1&{::@])^:_ packl) y
)

NB. A entrada é da forma NB. 'max_its svd_svd_ A'
NB. ou 'svd_svd_ A'
svd_svd_ =: (5000 $: ]) : (4 : 0)
  A =. y
  B0 =. ? 0 $~ 1 ,~ {. $ A
  C0 =. (0$0)

  U =. 0 $~ 0 ,~ # A
  S =. 0$0
  V =. 0 1$0

  debug =. 2 (1!:2)~ ":

  for_i. i. (>./ 0 , $ A) do.
    P =. x mqa_svd_ (A ; B0 ; C0)
    B =. 0 {:: P
    C =. 1 {:: P

    U =. U ,. (] % +/&.:*:@,) B
    S =. S , C %:@* & (+/@:*:@,) B
    V =. V , (] % +/&.:*:@,) C

    A =. A - (B (+/ . *) C)

    debug (>: i) % (>./ 0 , $ A)
  end.

  U ; S ; V
)

NB. A entrada é da forma 'unsvd (U ; S ; V)'
NB. Ele multiplica tudo para fazer a A
unsvd_svd_ =: 0&{:: (+/ . *) 2&{:: (+/ . *)~ (] * =/~@:i.@#)@(1&{::)

svd =: svd_svd_ :. unsvd_svd_
