NB. ==================================================
NB. Lendo arquivo
NB. ==================================================

rawdocs =: 1!:1 < 'tokens/docs.txt'
rawtokens =: 1!:1 < 'tokens/tokens.txt'
rawmatrix =: 1!:1 < 'tokens/matrix.txt'

docs =: <;._2 rawdocs
tokens =: <;._2 rawtokens
matrix =: ".;._2 rawmatrix
