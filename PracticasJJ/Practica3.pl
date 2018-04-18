
partir([], [], []).
partir([X|L], L1, L2) :- partir(L, L1, L2, X).

partir([], [], [], N).
partir([H|T], [H|T1], L2, N) :- H =< N, partir(T, T1, L2, N).
partir([H|T], L1, [H|T2], N) :- H > N, partir(T, L1, T2, N).


quicksort([], []).
quicksort(L, OL) :- L = [N|T], partir(L, L1, L2),
						quicksort(L1, LO1), quicksort(L2, LO2), 
						append(LO1, [N], X), append(X, LO2, OL).


						
						
mergesort([T], [T]).
mergesort(L, OL) :- append(L1, L2, L), length(L, X), length(L1, Y), Y is X//2,
					mergesort(L1, OL1), mergesort(L2, OL2), merge(OL1, OL2, OL).

merge([], [], []).
merge(L, [], L).
merge([], L, L).

merge([T1|L1], [T2|L2], [T1|OL]) :- T1 =< T2, merge(L1, [T2|L2], OL).
merge([T1|L1], [T2|L2], [T2|OL]) :- T1 > T2, merge([T1|L1], L2, OL).