%%%%%%Hechos%%%%%%%
familia(persona(antonio, foix, fecha(7, febrero, 1950), trabajo(renfe, 1200)),
persona(maria, lopez, fecha(17, enero, 1952), trabajo(sus_labores, 0)),
[persona(patricia, foix, fecha(10, junio, 1970), trabajo(estudiante, 0)),
persona(juan, foix, fecha(30, mayo, 1972), trabajo(estudiante,0))] ).

familia( persona(manuel, monterde, fecha(15, marzo, 1934), trabajo(profesor, 2000)),
persona(pilar, gonzalez, fecha(9, julio, 1940), trabajo(maestra, 1900)),
[ persona(manolo, monterde, fecha(10, febrero, 1964), trabajo(arquitecto, 5000)),
persona(javier, monterde, fecha(24, noviembre, 1968), trabajo(estudiante, 0)) ] ).

familia( persona(jose, benitez, fecha(3, septiembre, 1958), trabajo(profesor, 2000)),
persona(aurora, carvajal, fecha(29, agosto, 1972), trabajo(maestra, 1900)),
[ persona(jorge, benitez, fecha(6, noviembre, 1997), trabajo(desocupado, 0))] ).

familia( persona(jacinto, gil, fecha(7, junio, 1958), trabajo(minero, 1850)),
persona(guillermina, diaz, fecha(12, enero, 1957), trabajo(sus_labores, 0)),
[ persona(carla, gil, fecha(1, agosto, 1958), trabajo(oficinista, 1500)),
persona(amalia, gil, fecha(6, abril, 1962), trabajo(deliniante, 1900)), 
persona(irene, gil, fecha(3, mayo, 1970), trabajo(estudiante, 0)) ] ).

familia( persona(ismael, ortega, fecha(7, junio, 1966), trabajo(carpintero, 2350)),
persona(salvadora, diaz, fecha(12, enero, 1968), trabajo(sus_labores, 0)),
[] ).

familia( persona(pedro, ramirez, fecha(7, junio, 1966), trabajo(en_paro,0)),
persona(teresa, fuentes, fecha(12, enero, 1968), trabajo(administrativa, 1250)),
[] ).

%%%%%%%Reglas%%%%%%%%%%%

padre(P) :- familia(P, _, _).
madre(P) :- familia(_, P, _).
hijo(P) :- familia(_, _, H),
			member(P, H).
existe(P) :- padre(P); madre(P); hijo(P).
f_nacimiento(P, F) :- existe(P), P = persona(_, _, F, _).
salario(P, S) :- existe(P), 
					P = persona(_, _, _, Trabajo), 
					Trabajo = trabajo(_, S).
					
ej3a(Nombre, Apellido) :- existe(P), P = persona(Nombre, Apellido, _, _).

ej3b(Nombre_Hijo, Apellidos_Hijo) :- hijo(P), f_nacimiento(P, F),
										F = fecha(_, _, Y),
										Y > 1980,
										P = persona(Nombre_Hijo, Apellidos_Hijo, _, _).



ej3c(Nombre_Mujer, Apellido_Mujer) :- madre(P), salario(P, S),
										S > 0,
										P = persona(Nombre_Mujer, Apellido_Mujer, _, _).
										
ej3d(Nombre, Apellidos) :- existe(P), salario(P, S), f_nacimiento(P, F),
							S =:= 0,
							F = fecha(_, _, Y),
							Y < 1960,
							P = persona(Nombre, Apellidos, _, _).
							
ej3e(Nombre, Apellidos) :- existe(P), salario(P, S), f_nacimiento(P, F),
							S >= 800, S =< 1300,
							F = fecha(_, _, Y),
							Y > 1950,
							P = persona(Nombre, Apellidos, _, _).