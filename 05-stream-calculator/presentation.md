# Streams

Sequências Infinitas de **coisas**

```
(a, b, c, d, e, f, ...)
```

## Aparecem em Computação

* Stream de dados
  * Vídeo
  * Áudio
  * ...

* Teoria de Processamento de Sinais
  * Filtros, conversores

---
## Aparecem em Matemática

* Séries de Potência/Polinômios
  ```
  a0*x^0 + a1*x^1 + a2*x^2 + ...
  ```
  * Série de Taylor
    ```
    f(x) = f(a) + f'(a)*(x-a)^1/1! + f''(a) * (x-a)^2/2! + ...
    ```
* Problemas de contagem (sequências aparecem como solução)
  * Fibonacci
    ```
    0, 1, 1, 2, 3, 5, 8, ...
    ```
  * Números Triangulares
    ```
    1, 3, 6, 10, ..., n*(n+1)/2, ...
    ```

---
# Como podemos definir uma Stream?

Vamos fixar um conjunto de valores `A`

`A^w` vai ser o conjunto de **Streams de A**.

1. Definição por "função"
  ```
  A^w = { S ; S in { 0, 1, 2, ... } -> A }
  ```

Podemos fingir que `S` é uma lista infinita
  ```
  S = (s0, s1, s2, s3, s4, ...)
  ```

---
# Operações com Streams de A

```
S = (s0, s1, s2, s3, s4, ...)
```

1. Indexar: pedir o i-ésimo elemento
  ```
  S[i] := si
  ```
> Nota: `S[0]` também é chamado de **Head** de S

2. Tail/Derivar/ShiftL: jogar a 1a **coisa** fora
  ```
  S' := (s1, s2, s3, s4, s5, ...)

  S'[i] := S[i+1]
  ```

3. Cons/Integrar/ShiftR: colocar uma **coisa** no início
  ```
  a : S := (a, s0, s1, s2, s3, ...)

  (a : S)[0] := a
  (a : S)[i] := S[i-1]
  ```

---
# Equações Diferenciais

Podemos usar "Equações Diferenciais" para definir Streams

Dizemos quem é **Head** e **Tail** de `S`:

##### "Sempre" `a`:
```
  { S[0] = a
  { S'   = S
```

##### "Sempre" `a` e `b`:
```
  { S [0] = a
  { S'[0] = b (= S[1])
  { S''   = S
```

##### "Sempre" `a` e `b` (2):
```
  { S[0] = a
  { T[0] = b
  { S'   = T
  { T'   = S
```

---
# Equações Diferenciais (2)

##### Pares de `S`:
```
  { par(S)[0] = S[0]
  { par(S)'   = par(S'')
```

##### Impares de `S`:
```
  { impar(S)[0] = S[1]
  { impar(S)'   = impar(S'')
```

##### Zip de `S` e `T`:
```
  { zip(S, T)[0] = S[0]
  { zip(S, T)'   = zip(T, S')
```

##### Double de `S`:
```
  { double(S)[0] = S[0]
  { double(S)[1] = S[0]
  { double(S)''  = double(S')
```

---
## Cola de Equações Diferenciais

* "Sempre" `a`:
  ```
  (a, a, a, ...)
  ```

* "Sempre" `a` e `b`:
  ```
  (a, b, a, b, a, b, ...)
  ```

* Pares de `S`:
  ```
  (s0, s2, s4, s6, ...)
  ```

* Impares de `S`:
  ```
  (s1, s3, s5, s7, ...)
  ```

* Zip de `S` e `T`:
  ```
  (s0, t0, s1, t1, s2, t2, ...)
  ```

* Double de `S`:
  ```
  (s0, s0, s1, s1, s2, s2, ...)
  ```

---
# Igualdade de Streams

Como que mostramos que duas streams são iguais?
Exemplo:

```
  par(zip(S, T)) = S
```
```
  impar(zip(S, T)) = T
```
```
  zip(par(S), impar(S)) = S
```
```
  par(double(S)) = impar(double(S)) = S
```
```
  double(S) = zip(S, S)
```

Provas disso usam **Coindução**:
* Parece **Indução**, mas sem caso base
* É muita _magia negra_
* Não vou focar nisso

---
# Equivalência de Streams e Autômatos

Um autômato é uma tripla `<Q, out, next>`:
* `Q`: Conjunto de Estados (possívelmente infinito)
* `out`: Função `Q -> A`
* `next`: Função `Q -> Q`

> `out` está relacionado com **Head**

> `next` está relacionado com **Tail**

##### Autômato final

`<A^w, head, tail>`:
```
  head(S) := S[0]
```
```
  tail(S) := S'
```

---
# Streams de Números / Cálculo de Streams

Vamos usar `Números Reais` no lugar de `A`.

## Insersão <_> : Número -> Stream

Um Real `r` induz uma stream `<r>`:
```
  <r> = (r, 0, 0, 0, ...)
```

Dessa forma:
```
  <0> = (0, 0, 0, 0, ...)
  <1> = (1, 0, 0, 0, ...)
  <50> = (50, 0, 0, 0, ...)
  <0.5> = (0.5, 0, 0, 0, ...)
```

---
## Soma

Definição da **soma** de streams `S` e `T`
```
  { (S + T)[0] = S[0] + T[0]
  { (S + T)'   = S'   + T'
```

É equivalente a somar elemento a elemento:
```
  S + T := (s0 + t0, s1 + t1, s2 + t2, ...)
```

Têm uma _fórmula fechada_:
```
  (S + T)[i] := S[i] + T[i]
```

Observe o comportamento com um real `r`:
```
  <r> + S = (r + s0, s1, s2, s3, ...)
```

---
## Produto

Definição da **produto** de streams `S` e `T`
```
  { (S * T)[0] = S[0] * T[0]
  { (S * T)'   = (<S[0]> * T') + (S' * T)
```

É parecido com "um chuveirinho":
```
  S := (s0, s1, s2, s3, ...)
  T := (t0, t1, t2, t3, ...)
  S * T := (s0 * t0, s1 * t0 + s0 * t1, s0 * t2 + s1 * t1 + s2 * t0, ...)
```

Têm uma _fórmula fechada_:
```
  (S * T)[i] := sum (j in 0..=i): S[j] * T[i-j]
```

Observe o comportamento com um real `r`:
```
  <r> * S = (r * s0, r * s1, r * s2, r * s3, ...)
```

---
## Polinômios / Stream X

Streams se parecem muito com polinômios
```
  P(x) := s0 + s1 * x + s2 * x * x + s3 * x * x * x + ...
  S := (s0, s1, s2, s3, ...)
```

Conseguimos achar um `X` para isso funcionar

```
  X            := (0,  1, 0, 0, ...)

  <s0>          = (s0, 0, 0, 0, ...)
  <s1> * x      = (0, s1, 0, 0, ...)
  <s2> * x * x  = (0, 0, s2, 0, ...)
```

O multiplicar por `X` shifta `S`
para direita e põe um 0 no início.

> Equivalente a fazer `(0 : S)`

---
## Recorrência Linear

##### Fibonacci

```
F = (0 + 1 * X) + (F * X) + (F * X * X)
```

Isso quer dizer que
```
    ( 0,  1,  0,  0,  0, ...)
  + ( 0, f0, f1, f2, f3, ...)
  + ( 0,  0, f0, f1, f2, ...)
  = (f0, f1, f2, f3, f4, ...)
```

Observando as colunas conseguimos calcular os `fi`s
```
  f0 = 0
  f1 = 1
  f2 = 1
  ...
```

No final fica assim
```
F = (0, 1, 1, 2, 3, 5, 8, ...)
```

---
## Recorrência Linear (2)

Seria muito legal se pudessemos isolar o F, né?
```
F = (0 + 1 * X) + (F * X) + (F * X * X)
F = X + (F * X) + (F * X * X)
F = X + F * (X + X * X)
F - F * (X + X * X) = X

F * (1 - X - X * X) = X
```

Agora falta "jogar aquilo para o outro lado"

```
           X
F = ---------------
     1 - X - X * X
```

Podemos fazer isso?

SIM!

---
## Inversa

Queremos que a inversa "neutralize" a multiplicação
```
  S * S% = <1> = S% * S
```

Essa equação magica funciona:
```
  { (S%)[0] = - 1 / S[0]
  { (S%)'   = <-1/S[0]> * S' * S%
```

> **Nota:** não está definida para `S[0] == 0`.

---
# Calculadora

Brinque com a calculadora!

---
# Streams Racionais

Se `A` "tem cara de número" (é um corpo),
`A^w` é uma Stream Racional.

```
```

Em outras palavras,
conseguimos **somar**, **multiplicar** e **inverter** streams.

```
```

> Uma Stream Racional "tem cara de número" (é um corpo)

---
# Funções de Stream para Stream

**TODO**

---
# Circuitos

Podemos:
* Somar
* Duplicar
* Multiplicar por escalar
* Registrador

## Loop Simples

O circuito pode voltar
se passar por um **Registrador**

---
# Streams Racionais e Circuitos com Loop Simples

Variáveis:
* `c` é um **Circuito com Loop Simples Finito**
* `R` é uma **Stream Racional** `R`
* `f` é uma **Função de Streams para Streams**

Temos que:

* (a) Dada um `c`, existe uma `R` tal que a função
```
  f(S) = R * S
```
é implementada por `c`.

* (b) Se `f` é da forma:
```
  f(S) := R * S
```
então existe uma `c` que implementa `f`.
