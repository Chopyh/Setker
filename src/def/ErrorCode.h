#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <string>

namespace TokenTree {
    // Tipo que agrupa un nombre de error y un código asociado
    struct ErrorType {
        std::string name;
        int code;
    };

    // Define excepción genérica usando ErrorType
    struct Error {
        ErrorType type;
        std::string message;
        Error(const ErrorType &type, const std::string &msg = "")
            : type(type), message(msg.empty() ? type.name : msg) {}
    };

    namespace ErrorCodes {
        // Códigos y nombres de errores predefinidos
        inline const ErrorType InvalidAssignmentTarget     {"InvalidAssignmentTarget",     70};
        inline const ErrorType OperandMustBeNumber         {"OperandMustBeNumber",         70};
        inline const ErrorType OperandsMustBeNumbers       {"OperandsMustBeNumbers",       70};
        inline const ErrorType ArgumentCountMismatch       {"ArgumentCountMismatch",       70};
        inline const ErrorType CallOnNonFunction           {"CallOnNonFunction",           70};
        inline const ErrorType RuntimeError                {"RuntimeError",                70};
        // Alias para códigos genéricos
        inline const ErrorType ParseError         {"ParseError", 65};
        // Alias genéricos adicionales se pueden agregar aquí
    }
}

#endif // ERRORCODE_H
