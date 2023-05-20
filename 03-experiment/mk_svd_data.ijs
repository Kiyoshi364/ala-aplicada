NB. ==================================================
NB. Gerando SVD
NB. ==================================================

NB. Esse script genera os arquivos:
NB. - normal.gen.ijs
NB. - center.gen.ijs

NB. Isso é tipo um 'import'
0!:0 < './load_data.ijs'
0!:0 < './svd.ijs'

LF =. 10 { a.
rawfmt =. [ , ' =: ".;._2 (0 : 0)' , LF , ')' ,~ LF ,@:,.~ ":@:]
fmtU =. ('u_' , '_' ,~ [) rawfmt 0 {:: ]
fmtS =. ('s_' , '_' ,~ [) rawfmt 1 (] $~ 1 , $)@{:: ]
fmtV =. ('v_' , '_' ,~ [) rawfmt 2 {:: ]

format2file =. ('.gen.ijs' <@,~ [) (1!:2)~ (fmtU , LF , fmtS , LF , LF ,~ fmtV)

'normal' format2file normalsvd =: svd matrix

centered =: (] -"1 (+/ % #)) matrix

NB. Isso deve retornar 1 para indicar que "está centralizada"
NB. ([: *./ 1e_10 >&| +/ % #) centered

'center' format2file centeredsvd =: svd centered
