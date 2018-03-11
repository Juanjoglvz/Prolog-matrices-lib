# Programación Declarativa
Caso de estudio y prácticas
## Caso de estudio
Crear una libreria para Prolog en C para manipular matrices
### Notas

 - Los tipos de retorno tienen que ser tipos de prolog (Por eso se usa PL\_unify\_atom_chars(u, "HelloWorld!");)

- Los nombre de funciones tienen que empezar en minuscula (porque al hacer load\_foreign\_library(Library) trata Library como variable)

- Hay que "installar la funcion" al final usando PL\_register\_foreign(obj, arity, function, flags); http://www.swi-prolog.org/pldoc/man?CAPI=PL\_register\_foreign\_in\_module
