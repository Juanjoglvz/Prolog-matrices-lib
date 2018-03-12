%%% HECHOS

padre(teraj,abraham).
padre(teraj,najor).
padre(teraj,haran).
padre(teraj,sarai).
padre(abraham,isaac).
madre(sarai,isaac).
padre(haran,lot).
padre(haran,melca).
padre(haran,jesca).
padre(abraham,ismael).
madre(agar,ismael).
padre(batuel,rebeca).
padre(najor,batuel).
madre(melca,batuel).
padre(najor,laban).
madre(melca,laban).
padre(isaac,esau).
madre(rebeca,jacob).

casado(abraham,sarai).
casado(najor,melca).
casado(isaac,rebeca).

hombre(teraj).
hombre(abraham).
hombre(najor).
hombre(haran).
hombre(isaac).
hombre(lot).
hombre(batual).
hombre(laban).
hombre(esau).
hombre(jacob).
mujer(sarai).
mujer(melca).
mujer(jesca).
mujer(agar).
mujer(rebeca).



%%% REGLAS

ascendiente_directo(X, Y) :- (padre(X, Y); madre(X, Y)).

ascendiente(X, Z) :- ascendiente_directo(X, Z).
ascendiente(X, Z) :- ascendiente_directo(X, Y), ascendiente(Y, Z).

hijo(X,Y) :- hombre(X), ascendiente_directo(Y,X).
hija(X,Y) :- mujer(X), ascendiente_directo(Y,X).

descendiente_directo(X, Z) :- (padre(Z, X); madre(Z, X)).

descendiente(X, Z) :- descendiente_directo(X, Z).
descendiente(X, Z) :- descendiente_directo(X, Y), descendiente(Y, Z).

abuelo(X, Z) :- descendiente_directo(Z, Y), descendiente_directo(Y, X), hombre(X).
abuela(X, Z) :- descendiente_directo(Z, Y), descendiente_directo(Y, X), mujer(X).

hermano(X, Y) :- (padre(Z, X); madre(Z, X)), (padre(Z, Y); madre(Z, Y)).
hermana(X, Y) :- (padre(Z, X); madre(Z, X)), (padre(Z, Y); madre(Z, Y)).

tio(X, Y) :- hermano(X, Z), (padre(Z, Y); madre(Z,Y)), hombre(X).
tia(X, Y) :- hermano(X, Z), (padre(Z, Y); madre(Z,Y)), mujer(X).

sobrino(X, Y) :- (padre(Z, X); madre(Z, X)), hermano(Z, Y), hombre(X).
sobrina(X, Y) :- (padre(Z, X); madre(Z, X)), hermano(Z, Y), mujer(X).

primo(X, Y) :- (tio(X, Z); tia(X,Z)), (padre(Z, Y); madre(Z, Y)), hombre(X).
prima(X, Y) :- (tio(X, Z); tia(X,Z)), (padre(Z, Y); madre(Z, Y)), mujer(X).

incesto(X, Y) :- (descendiente(X, Y); abuelo(X, Y); hermano(X, Y); 
					tio(X, Y); sobrino(X, Y); primo(X, Y),
					casado(X, Y).
incesto(X, Y) :- incesto(Y, X).
