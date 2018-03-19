sus(X, Y, [], []) :- !.

sus(X, Y, [Z|T], L) :- Z = X, L = [Y|Lr], sus(X, Y, T, Lr).

sus(X, Y, [Z|T], L) :- Z \= X, L = [Z|Lr], sus(X, Y, T, Lr).
	
par(N) :- X is N mod 2, X =:= 0.
descomponer(N, A, B) :- between(0, N, A), between(0, N, B), par(A), par(B), N is A + B.

descomponerSinPares(N, A, B) :- between(0, N, A), between(0, N, B), N is A + B.
numBrandreth(A, C) :- between(32, 99, A), C is A * A,
					numToList(C, [X|[Y]]),
					descomponerSinPares(A, X, Y).
					



%%%%%%%Use with caution, only generates lists and not the other way around%%%%%%%%%%%%
numToList(Num, List) :- 
    numToList_r(Num, ListRev), 
    reverse(ListRev, List).
	
numToList_r(0, []) :- !.
numToList_r(N, [A|As]) :- A is N mod 100, N1 is floor(N / 100), numToList_r(N1, As).
