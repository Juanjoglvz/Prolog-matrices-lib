# Programación Declarativa
Caso de estudio y prácticas
## Caso de estudio
Escribir procedimientos en lenguaje C para manipular matrices y crear una librería que pueda integrarse en un programa SWI-Prolog.
### Notas

- Los tipos de retorno tienen que ser tipos de prolog (Por eso se usa PL_unify_atom_chars(u, "HelloWorld!");)

- Los nombre de funciones tienen que empezar en minuscula (porque al hacer load_foreign_library(Library) trata Library como variable)

- Hay que "instalar la funcion" al final usando PL_register_foreign(obj, arity, function, flags); http://www.swi-prolog.org/pldoc/man?CAPI=PL\register\foreign\in\module
