# Guía de Desarrollo para Setker

## Configuración del Entorno de Desarrollo

### Prerrequisitos
- **C++ Compiler**: GCC 11+, Clang 14+, o MSVC 2022+
- **CMake**: 3.10 o superior
- **Git**: Para control de versiones
- **IDE recomendado**: Visual Studio Code con extensiones de C++

### Configuración en Visual Studio Code

#### Extensiones Recomendadas:
```json
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-vscode.cmake-tools",
        "ms-vscode.cpptools-extension-pack",
        "twxs.cmake",
        "ms-vscode.makefile-tools"
    ]
}
```

#### Configuración de tareas (`.vscode/tasks.json`):
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Setker",
            "type": "shell",
            "command": "cmake",
            "args": ["--build", "build", "--config", "Debug"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        },
        {
            "label": "Configure CMake",
            "type": "shell",
            "command": "cmake",
            "args": ["-B", "build", "-S", "."],
            "group": "build"
        }
    ]
}
```

## Estándares de Código

### Estilo de Codificación
- **Indentación**: 4 espacios (no tabs)
- **Llaves**: Estilo BSD/Allman
- **Nombres de variables**: camelCase
- **Nombres de funciones**: camelCase
- **Nombres de clases**: PascalCase
- **Constantes**: UPPER_CASE
- **Archivos**: PascalCase para clases, lowercase para utilidades

### Ejemplo de Estilo:
```cpp
namespace TokenTree {
    class MyClass {
    private:
        int memberVariable;
        static const int CONSTANT_VALUE = 42;
        
    public:
        void myFunction(const std::string& parameter) 
        {
            if (condition) 
            {
                doSomething();
            }
            else 
            {
                doSomethingElse();
            }
        }
    };
}
```

### Documentación
- **Doxygen**: Usar comentarios estilo Doxygen para todas las funciones públicas
- **Archivos**: Cada archivo debe tener un header con descripción, autor y fecha
- **Funciones complejas**: Comentarios inline para lógica no obvia

```cpp
/**
 * @brief Descripción breve de la función
 * @param param1 Descripción del parámetro
 * @return Descripción del valor de retorno
 * @throws ExceptionType Cuándo se lanza esta excepción
 */
ReturnType functionName(ParamType param1);
```

## Flujo de Trabajo de Desarrollo

### Branches
- **main**: Código estable y probado
- **develop**: Integración de nuevas características
- **feature/nombre**: Desarrollo de características específicas
- **bugfix/nombre**: Corrección de errores
- **hotfix/nombre**: Correcciones urgentes

### Workflow de Contribución:
1. **Fork** del repositorio
2. **Crear branch**: `git checkout -b feature/nueva-caracteristica`
3. **Desarrollar** con commits frecuentes y descriptivos
4. **Testing** exhaustivo
5. **Pull Request** con descripción detallada
6. **Code Review** por mantainers
7. **Merge** después de aprobación

### Mensajes de Commit:
```
tipo(ámbito): descripción breve

Descripción más detallada si es necesaria.

- Cambio específico 1
- Cambio específico 2

Fixes #123
```

Tipos: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## Testing

### Estructura de Tests:
```
tests/
├── unit/           # Tests unitarios
├── integration/    # Tests de integración
├── examples/       # Tests con archivos .stk
└── fixtures/       # Datos de prueba
```

### Testing Manual:
```bash
# Compilar
cmake --build build

# Tests básicos
./build/Setker tokenize examples/arithmetic.stk
./build/Setker parse examples/factorial.stk
./build/Setker evaluate examples/control_flow.stk
./build/Setker run examples/functions.stk

# Verificar códigos de salida
echo $?  # Linux/macOS
echo $LASTEXITCODE  # PowerShell
```

### Tests Automatizados (futuros):
```cpp
// Ejemplo de test unitario
TEST(TokenizerTest, BasicArithmetic) {
    std::string input = "1 + 2";
    auto result = Tokenizer::getTokens(input);
    
    EXPECT_EQ(result.exitCode, 0);
    EXPECT_EQ(result.tokens.size(), 4); // NUMBER, PLUS, NUMBER, EOF
    EXPECT_EQ(result.tokens[0].getType(), TokenType::NUMBER);
}
```

## Debugging

### Técnicas de Debugging:
1. **Print Debugging**: Usar std::cout para valores intermedios
2. **AST Visualization**: Usar comando `parse` para ver estructura
3. **Token Analysis**: Usar comando `tokenize` para verificar lexing
4. **Step Evaluation**: Usar comando `evaluate` para ver evaluación paso a paso

### GDB/LLDB (Linux/macOS):
```bash
# Compilar con símbolos de debug
cmake -DCMAKE_BUILD_TYPE=Debug -B build

# Ejecutar con debugger
gdb ./build/Setker
(gdb) run tokenize examples/arithmetic.stk
(gdb) bt  # backtrace si hay crash
```

### Visual Studio Debugger (Windows):
- Configurar launch.json para debugging en VS Code
- Usar breakpoints en código C++
- Inspeccionar variables y call stack

## Añadir Nuevas Características

### 1. Nuevo Tipo de Token:
```cpp
// En Tokens.h
enum class TokenType {
    // ... existing tokens ...
    NEW_TOKEN,  // Agregar aquí
};

// En Tokenizer.cpp - función valorateTokens()
case 'nuevo_caracter':
    tokens.emplace_back(TokenType::NEW_TOKEN, "lexeme");
    break;

// En Tokens.cpp - función print()
case TokenType::NEW_TOKEN: ss << "NEW_TOKEN"; break;
```

### 2. Nueva Construcción Sintáctica:
```cpp
// En ASTNode.h
enum class Type {
    // ... existing types ...
    NewConstruct,  // Agregar aquí
};

// En Parser.cpp - nueva función parse
static std::unique_ptr<ASTNode> parseNewConstruct(/*params*/) {
    // Implementar parsing
    auto node = std::make_unique<ASTNode>(ASTNode::Type::NewConstruct, "value");
    // Agregar hijos si es necesario
    return node;
}

// Llamar desde parseStatement() o donde corresponda
```

### 3. Nueva Funcionalidad de Evaluación:
```cpp
// En Evaluator.cpp - función evalNode()
case Type::NewConstruct: {
    // Implementar evaluación
    // Manejar hijos si los hay
    // Retornar Value apropiado
    return result;
}
```

### 4. Manejo de Errores:
```cpp
// En ErrorCode.h
namespace ErrorCodes {
    inline const ErrorType NewError {"NewError", 70};
}

// Usar en código:
throw Error(ErrorCodes::NewError, "Mensaje específico");
```

## Optimización de Rendimiento

### Profiling:
```bash
# Compilar con optimización
cmake -DCMAKE_BUILD_TYPE=Release -B build

# Profiling con perf (Linux)
perf record ./build/Setker run large_program.stk
perf report

# Profiling con Instruments (macOS)
instruments -t "Time Profiler" ./build/Setker run large_program.stk
```

### Optimizaciones Comunes:
1. **Memory allocation**: Usar memory pools para ASTNodes
2. **String operations**: Evitar copias innecesarias
3. **Token creation**: Reservar capacidad en vectores
4. **AST traversal**: Optimizar recorrido del árbol

## Deployment

### Release Build:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --config Release
```

### Packaging:
```bash
# Crear directorio de distribución
mkdir -p dist/bin dist/examples dist/docs

# Copiar archivos necesarios
cp build/Setker dist/bin/
cp examples/*.stk dist/examples/
cp README.md LICENSE dist/
cp docs/*.md dist/docs/

# Crear archivo tar/zip para distribución
tar -czf setker-v1.0.tar.gz dist/
```

## Recursos Adicionales

### Referencias:
- [Crafting Interpreters](https://craftinginterpreters.com/) - Libro base
- [Modern C++ Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - Estilo de código
- [CMake Documentation](https://cmake.org/documentation/) - Build system
- [Doxygen Manual](https://www.doxygen.nl/manual/) - Documentación

### Herramientas Útiles:
- **clang-format**: Formateo automático de código
- **cppcheck**: Análisis estático de código
- **valgrind**: Detección de memory leaks (Linux)
- **AddressSanitizer**: Detección de errores de memoria
- **Compiler Explorer**: Análisis de código assembly generado

### Comunidad:
- Issues en GitHub para reportar bugs
- Discussions para preguntas y propuestas
- Wiki para documentación extendida
- Discord/Slack para comunicación en tiempo real (si disponible)
