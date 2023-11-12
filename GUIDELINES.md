# Guia de contribucion
En este documento se retrataran las practicas empleadas al momento de aportar en el proyecto, esto con el fin de mantener un estilo consistente, buenas practicas y mejorar la experiencia de leer/entender el codigo.

## Generalidades
- El idioma del codigo debera ser en ingles.
- Se envolvera todo el codigo bajo el nombre de espacio 'dsc'.
- No se permite el uso de 'using namespace' en el ambito global.
- No se permite el uso de macros.
- No se permite la creacion de variables globales fuera del nombre de espacio 'dsc'.
- Las variables globales siempre deben ser 'constexpr'.
- El orden de inclusion de archivos sera: STL > SFML > DSC

## Estilo
- No abreviar nombres. Ej: intNumArr ✖ | integerNumbersArray ~
- Evitar repeticion. Ej: integerNumberArray ✖ | integers ✔
- Alias / Class / Struct / Tipos parametros de plantilla: snake_case_t
- Variables / Funciones: camelCase
- Variables globales: SCREAMING_SNAKE_CASE
- Estructuras para SFINAE / Conceptos: PascalCase
- Toda funcion / variable debera ser marcada como 'const' cuando sea posible.
- Toda funcion debera ser marcada como 'noexcept' cuando sea posible.
- inline > static > constexpr > calificadores cv > tipo > const *.
- La llave de apertura va en la misma linea de la declaracion. Ej: if(...) {
- Los '#include' van preferiblemente en los archivo *.hpp.
