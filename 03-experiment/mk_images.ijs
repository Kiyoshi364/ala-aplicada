NB. ==================================================
NB. Faz desenhos / gráficos
NB. ==================================================

NB. Isso é tipo um 'import'
0!:0 < './main.ijs'

NB. load 'plot'

myplot =: 4 : 0
  fname =. (('-' ,. ~ [) {~"_1 ' _' +./@:(=/) ]) x
  pd 'reset'
  pd 'type point'
  pd 'title ' , x
  pd y
  pd 'pdf images/' , fname , '.pdf'
)

mypartplot =: 4 : 0
  mask =. 1 <"1@:|:@:({::"1) ] {~ [ (] i. startswithmask) 0 {"1 ]
  x myplot (0&{:: mask 1&{::) y
)

'normal 1,2' myplot <"1 |: 2 {."1 v_normal_
'center 1,2' myplot <"1 |: 2 {."1 v_center_

docs01_normal_ =: docs ,. <"1 (0 1 {&.|: u_normal_) * s_normal_
docs01_center_ =: docs ,. <"1 (0 1 {&.|: u_center_) * s_center_

'Threads normal' mypartplot 'Thread/' ,&< docs01_normal_
'atomic normal' mypartplot 'atomic/' ,&< docs01_normal_
'c normal' mypartplot 'c/' ,&< docs01_normal_
'compress/deflate normal' mypartplot 'compress/deflate/' ,&< docs01_normal_
'x normal' mypartplot 'x' ,&< docs01_normal_

'Threads center' mypartplot 'Thread/' ,&< docs01_center_
'atomic center' mypartplot 'atomic/' ,&< docs01_center_
'c center' mypartplot 'c/' ,&< docs01_center_
'compress/deflate center' mypartplot 'compress/deflate/' ,&< docs01_center_
'x center' mypartplot 'x' ,&< docs01_center_
