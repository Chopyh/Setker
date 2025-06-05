# Setker - Intérprete de Lenguaje de Programación

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-23-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)

## Descripción

Setker es un intérprete de lenguaje de programación completo implementado en C++23. El proyecto incluye un analizador léxico (tokenizer), analizador sintáctico (parser), evaluador de expresiones y un sistema de ejecución completo que soporta variables, funciones, estructuras de control y más.

## Características del Lenguaje

### Tipos de Datos
- **Números**: Punto flotante de doble precisión
- **Cadenas**: Texto entre comillas dobles
- **Booleanos**: `true` y `false`
- **Nil**: Valor nulo representado como `nil`

### Operadores
- **Aritméticos**: `+`, `-`, `*`, `/`, `%`
- **Comparación**: `<`, `<=`, `>`, `>=`, `==`, `!=`
- **Lógicos**: `and`, `or`, `!`
- **Unarios**: `-` (negación), `!` (negación lógica)
- **Asignación**: `=`

### Estructuras de Control
- **Condicionales**: `if`/`else`
- **Bucles**: `while`, `for`
- **Funciones**: Declaración con `fun` y `return`

### Características Avanzadas
- **Scoping léxico**: Variables locales y globales
- **Closures**: Funciones que capturan variables del entorno
- **Recursión**: Soporte completo para funciones recursivas
- **Funciones nativas**: Como `clock()` para obtener tiempo

## Estructura del Proyecto

```
Setker/
├── src/
│   ├── main.cpp                 # Punto de entrada principal
│   ├── commands/                # Módulos de procesamiento
│   │   ├── Tokenizer.h/.cpp     # Análisis léxico
│   │   ├── Parser.h/.cpp        # Análisis sintáctico
│   │   ├── Evaluator.h/.cpp     # Evaluación de expresiones
│   │   └── Run.h/.cpp           # Ejecución completa
│   └── def/                     # Definiciones y estructuras de datos
│       ├── Tokens.h/.cpp        # Definición de tokens
│       ├── ASTNode.h/.cpp       # Nodos del AST
│       ├── Environment.h/.cpp   # Entorno de variables
│       ├── ErrorCode.h          # Códigos de error
│       └── Keywords.h/.cpp      # Palabras clave del lenguaje
├── docs/                        # Documentación detallada
│   ├── ARCHITECTURE.md         # Arquitectura del sistema
│   ├── DEVELOPMENT.md          # Guía de desarrollo
│   └── GRAMMAR.md              # Gramática del lenguaje
├── examples/                    # Ejemplos de código
├── CMakeLists.txt              # Configuración de CMake
├── README.md                   # Este archivo
└── LICENSE                     # Licencia MIT
```

## Prerrequisitos

- **Compilador C++**: GCC 11+, Clang 14+, o MSVC 2022+ (con soporte para C++23)
- **CMake**: Versión 3.10 o superior
- **Sistema operativo**: Windows, Linux o macOS

## Instrucciones de Compilación

### Opción 1: Compilación Estándar
```bash
# Crear directorio de compilación
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
cmake --build . --config Release
```

### Opción 2: Compilación en Windows con Visual Studio
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Opción 3: Usando Make (Linux/macOS)
```bash
mkdir build && cd build
cmake .. -G "Unix Makefiles"
make -j$(nproc)
```

## Uso del Intérprete

El intérprete de Setker proporciona varios comandos para diferentes etapas del procesamiento:

### Comandos Disponibles

#### `tokenize <archivo>`
Analiza léxicamente el archivo y muestra todos los tokens identificados.
```bash
./Setker tokenize examples/arithmetic.stk
```

#### `parse <archivo>`
Construye y muestra el Árbol de Sintaxis Abstracta (AST).
```bash
./Setker parse examples/factorial.stk
```

#### `evaluate <archivo>`
Evalúa expresiones y muestra los resultados paso a paso.
```bash
./Setker evaluate examples/control_flow.stk
```

#### `run <archivo>`
Ejecuta completamente el programa.
```bash
./Setker run examples/functions.stk
```

#### `help`
Muestra información detallada sobre todos los comandos.
```bash
./Setker help
```

## Ejemplos de Código

### Operaciones Aritméticas
```javascript
var a = 5 + 2;      // 7
var b = a - 3;      // 4
var c = b * 4;      // 16
var d = c / 5;      // 3.2
var e = c % 6;      // 4
print -(e + 1);     // -5
```

### Funciones y Recursión
```javascript
fun factorial(n) {
  if (n <= 1) return 1;
  return n * factorial(n - 1);
}

print factorial(5);  // 120
```

### Estructuras de Control
```javascript
var x = 10;
if (x > 5) {
  print "x es mayor que 5";
} else {
  print "x es menor o igual a 5";
}

for (var i = 0; i < 5; i = i + 1) {
  print i;
}
```

### Closures
```javascript
fun makeCounter() {
  var count = 0;
  fun increment() {
    count = count + 1;
    return count;
  }
  return increment;
}

var counter = makeCounter();
print counter(); // 1
print counter(); // 2
```

## Arquitectura del Sistema

### 1. Análisis Léxico (Tokenizer)
- Convierte el código fuente en una secuencia de tokens
- Maneja comentarios, espacios en blanco y cadenas
- Identifica palabras clave, operadores y literales

### 2. Análisis Sintáctico (Parser)
- Construye un AST basado en la gramática del lenguaje
- Implementa precedencia de operadores
- Maneja asociatividad y agrupación con paréntesis

### 3. Evaluación (Evaluator)
- Recorre el AST y evalúa expresiones
- Maneja el entorno de variables y scope
- Implementa funciones y closures

### 4. Ejecución (Run)
- Integra tokenización, parsing y evaluación
- Proporciona ejecución completa de programas
- Maneja errores y excepciones

## Manejo de Errores

El sistema incluye un manejo robusto de errores:
- **Errores léxicos**: Caracteres no reconocidos, cadenas sin terminar
- **Errores sintácticos**: Sintaxis incorrecta, paréntesis no balanceados
- **Errores de tiempo de ejecución**: Variables no definidas, operaciones inválidas
- **Códigos de salida estándar**: Compatible con herramientas de testing

## Extensibilidad

El proyecto está diseñado para ser fácilmente extensible:
- **Nuevos tipos de datos**: Agregar en `Environment::Value`
- **Nuevos operadores**: Extender `TokenType` y `Parser`
- **Funciones nativas**: Agregar en `Evaluator::evalNode`
- **Optimizaciones**: Implementar en el evaluador

## Testing

Ejecutar ejemplos incluidos:
```bash
# Probar funcionalidad básica
./Setker run examples/arithmetic.stk
./Setker run examples/factorial.stk
./Setker run examples/control_flow.stk

# Verificar análisis
./Setker parse examples/functions.stk
./Setker tokenize examples/for_loop.stk
```

## Contribuciones

Las contribuciones son bienvenidas. Para contribuir:

1. **Fork** el repositorio
2. Crear una **rama de feature** (`git checkout -b feature/nueva-caracteristica`)
3. **Commit** los cambios (`git commit -am 'Agregar nueva característica'`)
4. **Push** a la rama (`git push origin feature/nueva-caracteristica`)
5. Crear un **Pull Request**

### Estilo de Código
- Seguir las convenciones de C++23
- Usar nombres descriptivos para variables y funciones
- Documentar funciones públicas
- Mantener consistencia con el código existente

## Licencia

Este proyecto está licenciado bajo la Licencia MIT. Consulte el archivo [LICENSE](LICENSE) para más detalles.

## Autor

**Javier** - Trabajo de Fin de Grado - Múster en Desarrollo Web

## Agradecimientos

- Inspirado en el libro "Crafting Interpreters" de Robert Nystrom
- Comunidad de C++ por las herramientas y bibliotecas
- Profesores y compañeros del programa de Máster en Desarrollo Web