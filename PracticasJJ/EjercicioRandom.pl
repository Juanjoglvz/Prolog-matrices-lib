cosaPar(N) :- X is N mod 2, X =:= 0.

cosa([], []).

cosa([X|L], [X|L2]) :- cosaPar(X), cosa(L, L2).
cosa([X|L], L2) :- not(cosaPar(X)), cosa(L, L2).

cosa2(L, N) :- member(N, L), cosaPar(N).

cosa3(L, X) :- setof(N , cosa2(L, N), X).

