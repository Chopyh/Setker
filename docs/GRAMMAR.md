# Gramática del Lenguaje Setker

## Descripción General

Setker es un lenguaje de programación dinámicamente tipado que soporta programación procedural con funciones de primera clase. Esta documentación describe la gramática formal del lenguaje en notación BNF extendida.

## Notación BNF Utilizada

- `|` : Alternativa (OR)
- `*` : Cero o más repeticiones
- `+` : Una o más repeticiones
- `?` : Opcional (cero o una ocurrencia)
- `()` : Agrupación
- `MAYÚSCULAS` : Terminales (tokens)
- `minúsculas` : No terminales (reglas de producción)

## Gramática Completa

### Programa Principal
```bnf
program        → declaration* EOF

declaration    → funDecl
               | varDecl  
               | statement

funDecl        → "fun" IDENTIFIER "(" parameters? ")" block
parameters     → IDENTIFIER ( "," IDENTIFIER )*

varDecl        → "var" IDENTIFIER ( "=" expression )? ";"
```

### Declaraciones y Sentencias
```bnf
statement      → exprStmt
               | forStmt
               | ifStmt  
               | printStmt
               | returnStmt
               | whileStmt
               | block

exprStmt       → expression ";"
forStmt        → "for" "(" ( varDecl | exprStmt | ";" ) 
                          expression? ";" 
                          expression? ")" statement
ifStmt         → "if" "(" expression ")" statement ( "else" statement )?
printStmt      → "print" expression ";"
returnStmt     → "return" expression? ";"
whileStmt      → "while" "(" expression ")" statement
block          → "{" declaration* "}"
```

### Expresiones (por precedencia, de menor a mayor)
```bnf
expression     → assignment

assignment     → IDENTIFIER "=" assignment
               | logic_or

logic_or       → logic_and ( "or" logic_and )*
logic_and      → equality ( "and" equality )*
equality       → comparison ( ( "!=" | "==" ) comparison )*
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )*
term           → factor ( ( "-" | "+" ) factor )*
factor         → unary ( ( "/" | "*" | "%" ) unary )*

unary          → ( "!" | "-" ) unary
               | call

call           → primary ( "(" arguments? ")" )*
arguments      → expression ( "," expression )*

primary        → "true" | "false" | "nil"
               | NUMBER | STRING  
               | IDENTIFIER
               | "(" expression ")"
```

## Tokens (Elementos Léxicos)

### Literales
```bnf
NUMBER         → DIGIT+ ( "." DIGIT+ )?
STRING         → "\"" <cualquier_caracter_excepto_">* "\""
IDENTIFIER     → ALPHA ( ALPHA | DIGIT )*

ALPHA          → "a" ... "z" | "A" ... "Z" | "_"
DIGIT          → "0" ... "9"
```

### Palabras Clave
```
and    class   else   false  for    fun    if     nil
or     print   return super  this   true   var    while
```

### Operadores y Delimitadores
```
(  )  {  }  [  ]     // Agrupación
,  .  ;  :           // Separadores  
-  +  /  *  %        // Aritméticos
!  !=  =  ==         // Lógicos/Comparación
>  >=  <  <=         // Comparación
```

### Comentarios
```bnf
line_comment   → "//" <cualquier_caracter_excepto_newline>* newline
block_comment  → "<|" <cualquier_caracter>* "|>"
```

## Precedencia de Operadores

| Precedencia | Operadores | Asociatividad | Descripción |
|-------------|------------|---------------|-------------|
| 1 (mayor)   | `!`, `-` (unario) | Right | Negación lógica, negación aritmética |
| 2           | `*`, `/`, `%` | Left | Multiplicación, división, módulo |
| 3           | `+`, `-` | Left | Suma, resta |
| 4           | `>`, `>=`, `<`, `<=` | Left | Comparación relacional |
| 5           | `==`, `!=` | Left | Comparación de igualdad |
| 6           | `and` | Left | AND lógico |
| 7           | `or` | Left | OR lógico |
| 8 (menor)   | `=` | Right | Asignación |

## Semántica del Lenguaje

### Tipos de Datos
1. **nil**: Valor nulo único
2. **bool**: `true` o `false`
3. **number**: Números de punto flotante de doble precisión
4. **string**: Cadenas de caracteres Unicode
5. **function**: Funciones definidas por el usuario

### Coerción de Tipos
- **Truthiness**: `false` y `nil` son falsy, todo lo demás es truthy
- **String concatenation**: `+` entre strings realiza concatenación
- **Numeric operations**: Requieren operandos numéricos (excepto `+` con strings)

### Scoping
- **Lexical scoping**: Las variables se resuelven en el ámbito donde se definen
- **Block scope**: Los bloques `{}` crean nuevos ámbitos
- **Function scope**: Las funciones crean sus propios ámbitos
- **Global scope**: Variables definidas fuera de cualquier bloque

### Funciones
- **First-class citizens**: Las funciones son valores que pueden ser asignados a variables
- **Closures**: Las funciones capturan variables de su ámbito de definición
- **Parameters**: Pasaje por valor para todos los tipos
- **Return values**: `return` sin expresión devuelve `nil`

## Ejemplos de Construcciones

### Declaración de Variables
```javascript
var x;              // x = nil
var y = 10;         // y = 10
var name = "Alice"; // name = "Alice"
```

### Expresiones Aritméticas
```javascript
var result = (5 + 3) * 2 - 1;  // result = 15
var remainder = 17 % 5;         // remainder = 2
var negative = -result;         // negative = -15
```

### Expresiones Lógicas
```javascript
var isValid = true and (x > 0 or y < 10);
var isInvalid = !isValid;
var condition = x == y or name != "Bob";
```

### Estructuras de Control
```javascript
// Condicional
if (x > 0) {
    print "Positive";
} else {
    print "Non-positive";
}

// Bucle while
var i = 0;
while (i < 5) {
    print i;
    i = i + 1;
}

// Bucle for
for (var j = 0; j < 3; j = j + 1) {
    print "Iteration: " + j;
}
```

### Funciones
```javascript
// Función simple
fun greet(name) {
    print "Hello, " + name + "!";
}

// Función con retorno
fun add(a, b) {
    return a + b;
}

// Función recursiva
fun factorial(n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Closure
fun makeCounter() {
    var count = 0;
    fun increment() {
        count = count + 1;
        return count;
    }
    return increment;
}
```

### Llamadas a Funciones
```javascript
greet("World");                    // Hello, World!
var sum = add(5, 3);              // sum = 8
var fact = factorial(5);          // fact = 120
var counter = makeCounter();
print counter();                  // 1
print counter();                  // 2
```

## Funciones Nativas

### `clock()`
Retorna el tiempo actual en segundos desde epoch Unix.
```javascript
var start = clock();
// ... código a medir ...
var elapsed = clock() - start;
print "Elapsed time: " + elapsed + " seconds";
```

## Manejo de Errores

### Errores de Compilación (Código 65)
- Sintaxis incorrecta
- Tokens no reconocidos
- Paréntesis/llaves no balanceados
- Cadenas sin terminar

### Errores de Ejecución (Código 70)
- Variable no definida
- Llamada a no-función
- Número incorrecto de argumentos
- Operaciones entre tipos incompatibles
- Asignación a target inválido

## Limitaciones Actuales

1. **No hay arrays/listas**: Solo tipos primitivos y funciones
2. **No hay objetos/clases**: Aunque la gramática incluye `class`, no está implementado
3. **No hay importación de módulos**: Todo debe estar en un archivo
4. **Garbage collection manual**: No hay manejo automático de memoria para closures
5. **Sin manejo de excepciones**: Los errores terminan la ejecución

## Extensiones Futuras

### Posibles Adiciones:
1. **Arrays**: `[1, 2, 3]` y operaciones de índice
2. **Maps/Objects**: `{key: value}` structures
3. **Classes**: Programación orientada a objetos
4. **Modules**: Sistema de importación/exportación
5. **Exception handling**: `try`/`catch`/`throw`
6. **Lambda expressions**: Funciones anónimas más concisas
7. **String interpolation**: `"Hello, #{name}!"`
8. **Multiple assignment**: `var a, b = 1, 2;`
