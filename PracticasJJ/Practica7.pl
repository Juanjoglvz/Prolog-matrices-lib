%%Cosa interesante:
cosa(L) :- familia(persona(mario, perez), persona(maria, perez), hijos(persona(mauro, perez)), trabajo(nosehulio)) =.. L.
cosa2(F, L) :- F =.. L.
cosa3(F) :- F =.. [familia, persona(mario, perez), persona(maria, perez), hijos(persona(mauro, perez)), trabajo(nosehulio)].


ficha(F) :- fichaRec(F, 0).

fichaRec(F, T) :- atomic(F), !,
				N is 8 * T, length(L, N), spaceList(L), name(C, L), write(C),
				name(F, O), append(O, [44, 10], Output), 
				name(FinalOutput, Output), write(FinalOutput).
				
fichaRec(F, T) :- functor(F, Name, _),
				N is 8 * T, length(L, N), spaceList(L), name(C, L), write(C),
				name(Name, O), append([42], O, Output1), append(Output1, [58, 10], Output),
				name(FinalOutput, Output), write(FinalOutput),
				F =.. [_|Lista], T2 is T + 1, fichaCall(Lista, T2).

fichaCall([], _).
fichaCall([H|T], N) :- fichaRec(H, N), fichaCall(T, N).
			
spaceList([]).
spaceList([H|T]) :- H = 32, spaceList(T).

