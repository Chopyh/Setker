# Arquitectura del Intérprete Setker

## Descripción General

Setker es un intérprete de lenguaje de programación implementado siguiendo el patrón clásico de diseño de compiladores e intérpretes. El sistema se divide en cuatro fases principales de procesamiento que transforman el código fuente en ejecución.

## Fases del Procesamiento

### 1. Análisis Léxico (Tokenizer)

**Archivos**: `src/commands/Tokenizer.h/.cpp`

El tokenizer es responsable de convertir el código fuente (cadena de caracteres) en una secuencia de tokens significativos.

#### Funcionalidades:
- **Reconocimiento de tokens**: Identifica palabras clave, operadores, literales, identificadores
- **Manejo de espacios en blanco**: Ignora espacios, tabs y saltos de línea
- **Comentarios**: Procesa comentarios de línea (`//`) y comentarios de bloque (`<| ... |>`)
- **Literales de cadena**: Maneja cadenas entre comillas con caracteres especiales
- **Números**: Reconoce enteros y decimales
- **Validación léxica**: Detecta caracteres inválidos y cadenas sin terminar

#### Flujo de Datos:
```
Código Fuente (string) → Tokenizer → Vector<Token>
```

#### Ejemplo de Transformación:
```javascript
var x = 10 + 5;
```
Se convierte en:
```
VAR var null
IDENTIFIER x null  
EQUAL = null
NUMBER 10 10.0
PLUS + null
NUMBER 5 5.0
SEMICOLON ; null
EOF  null
```

### 2. Análisis Sintáctico (Parser)

**Archivos**: `src/commands/Parser.h/.cpp`

El parser construye un Árbol de Sintaxis Abstracta (AST) a partir de la secuencia de tokens, verificando que la sintaxis sea correcta según la gramática del lenguaje.

#### Características:
- **Gramática recursiva descendente**: Implementa parsing de precedencia de operadores
- **Manejo de precedencia**: Operadores ordenados por prioridad (unarios > multiplicativos > aditivos > comparación > igualdad > lógicos)
- **Asociatividad**: Left-associative para la mayoría, right-associative para asignación
- **Estructuras de control**: if/else, while, for
- **Funciones**: Declaración y llamadas con parámetros
- **Scoping**: Bloques de código con `{}`

#### Gramática (BNF simplificada):
```
program        → statement* EOF
statement      → varDecl | funDecl | ifStmt | whileStmt | forStmt | block | returnStmt | printStmt | exprStmt
expression     → assignment
assignment     → IDENTIFIER "=" assignment | logic_or
logic_or       → logic_and ( "or" logic_and )*
logic_and      → equality ( "and" equality )*
equality       → comparison ( ( "!=" | "==" ) comparison )*
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )*
term           → factor ( ( "-" | "+" ) factor )*
factor         → unary ( ( "/" | "*" | "%" ) unary )*
unary          → ( "!" | "-" ) unary | call
call           → primary ( "(" arguments? ")" )*
primary        → NUMBER | STRING | "true" | "false" | "nil" | IDENTIFIER | "(" expression ")"
```

#### Flujo de Datos:
```
Vector<Token> → Parser → AST (ASTNode*)
```

### 3. Evaluación (Evaluator)

**Archivos**: `src/commands/Evaluator.h/.cpp`

El evaluador recorre el AST y evalúa las expresiones, manteniendo el estado del programa a través del sistema de entornos.

#### Componentes Clave:
- **Tree Walker**: Recorre el AST usando el patrón Visitor
- **Environment**: Sistema de entornos anidados para variables
- **Type System**: Manejo de tipos dinámicos usando std::variant
- **Function Calls**: Soporte para funciones definidas por el usuario y nativas
- **Control Flow**: Implementación de if, while, for y return

#### Tipos de Datos:
```cpp
using Value = std::variant<
    std::monostate,  // nil
    double,          // números
    bool,           // booleanos
    std::string,    // cadenas
    FunctionPtr     // funciones
>;
```

#### Manejo de Entornos:
- **Global Environment**: Variables globales y funciones
- **Local Environments**: Creados para cada bloque y función
- **Lexical Scoping**: Variables resueltas en tiempo de definición
- **Closures**: Funciones capturan su entorno de definición

### 4. Ejecución (Run)

**Archivos**: `src/commands/Run.h/.cpp`

El módulo Run integra todas las fases anteriores para proporcionar ejecución completa de programas.

#### Proceso:
1. Tokenización del código fuente
2. Parsing para generar AST
3. Evaluación del AST con manejo de errores
4. Salida de resultados o errores

## Estructuras de Datos Principales

### Token
```cpp
class Token {
    TokenType type;      // Tipo del token
    std::string lexeme;  // Texto original
    Literal literal;     // Valor si aplicable
};
```

### ASTNode
```cpp
class ASTNode {
    Type type;                                    // Tipo de nodo
    std::string value;                           // Valor asociado
    std::vector<std::unique_ptr<ASTNode>> children; // Hijos
};
```

### Environment
```cpp
class Environment {
    std::unordered_map<std::string, Value> values; // Variables locales
    std::shared_ptr<Environment> enclosing;         // Entorno padre
};
```

## Manejo de Errores

### Tipos de Errores:
1. **Léxicos** (código 65): Caracteres inválidos, cadenas sin terminar
2. **Sintácticos** (código 65): Sintaxis incorrecta, paréntesis no balanceados
3. **Semánticos** (código 70): Variables no definidas, tipos incompatibles
4. **Runtime** (código 70): División por cero, argumentos incorrectos

### Sistema de Excepciones:
```cpp
struct Error {
    ErrorType type;      // Tipo y código de error
    std::string message; // Mensaje descriptivo
};
```

## Patrones de Diseño Utilizados

### 1. Visitor Pattern
- **Uso**: Evaluación del AST
- **Beneficio**: Separación entre estructura de datos y operaciones

### 2. Composite Pattern
- **Uso**: Estructura del AST
- **Beneficio**: Tratamiento uniforme de nodos simples y compuestos

### 3. Chain of Responsibility
- **Uso**: Búsqueda de variables en entornos anidados
- **Beneficio**: Implementación natural del scoping léxico

### 4. Strategy Pattern
- **Uso**: Diferentes comandos del intérprete (tokenize, parse, evaluate, run)
- **Beneficio**: Facilita extensión y testing de componentes

## Extensibilidad

### Agregar Nuevos Tipos de Datos:
1. Extender `Environment::Value` variant
2. Actualizar operadores en `Evaluator`
3. Agregar casos en `toString()` métodos

### Agregar Nuevas Estructuras de Control:
1. Agregar `TokenType` en `Tokens.h`
2. Extender `ASTNode::Type`
3. Implementar parsing en `Parser.cpp`
4. Agregar evaluación en `Evaluator.cpp`

### Agregar Funciones Nativas:
1. Implementar en `Evaluator::evalNode()` case `Call`
2. Seguir patrón de función `clock()`

## Optimizaciones Futuras

### Posibles Mejoras:
1. **Bytecode VM**: Compilar a bytecode para mejor rendimiento
2. **Garbage Collection**: Manejo automático de memoria para objetos
3. **JIT Compilation**: Compilación en tiempo de ejecución
4. **Type Checking**: Análisis estático de tipos
5. **Constant Folding**: Optimización de expresiones constantes

### Herramientas de Desarrollo:
1. **Debugger**: Step-through debugging del AST
2. **Profiler**: Análisis de rendimiento
3. **REPL**: Read-Eval-Print Loop interactivo
4. **LSP**: Language Server Protocol para editores
