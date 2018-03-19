lista_de_prueba(L) :- length(L, 10000).
prueba(L) :- L = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10].

speedup(S, G1, G2) :- T1 is cputime, G1, T2 is cputime, T is T2 - T1,
					T3 is cputime, G2, T4 is cputime, TT is T4 - T3,
					S is TT - T.

invertir(L, I) :- invertir1(L, I).

invertir1([], []).
invertir1([H|T], L) :- invertir1(T, Z), append(Z, [H], L).


invertir2(L, I) :- inv(L, [], I).

inv([], I, I).
inv([H|T], A, L) :- inv(T, [H|A], L).

longitud1(L, N) :- l1(L, N).

l1([], 0).
l1([X|A], N) :- l1(A, Z), N is Z+1.

longitud2(L, N) :- l2(L, 0, N).

l2([], A, A).
l2([H|T], A, N) :- Z is A+1, l2(T, Z, N).

suma1([], 0).
suma1([H|T], N) :- suma1(T, Z), N is Z + H.

suma2(L, N) :- s2(L, 0, N).

s2([], A, A).
s2([H|T], A, N) :- B is A + H, s2(T, B, N).


fib1(1,1).
fib1(2,1).
fib1(N,F) :- N>2, H1 is N - 1, H2 is N - 2,
			fib1(H1,N1), fib1(H2, N2),
			F is N1 + N2.
			
fib2(N,F) :- 

