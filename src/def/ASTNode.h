#ifndef ASTNODE_H
#define ASTNODE_H

#include <memory>
#include <string>
#include <vector>
#include <memory_resource>

namespace TokenTree {
    class ASTNode {
    public:
        enum class Type { Number, BinaryOp, String, Boolean, Nil, PrintStmt, IfStmt, WhileStmt, ReturnStmt, Function, Call, Program, VarDecl, Identifier };
    private:
        Type type;
        std::string value;
        std::vector<std::unique_ptr<ASTNode>> children;
    public:
        ASTNode(Type type, std::string value);
        void addChild(std::unique_ptr<ASTNode> child);
        std::string toString() const;
        Type getType() const;
        const std::string& getValue() const;
        const std::vector<std::unique_ptr<ASTNode>>& getChildren() const;
    };
}

#endif // ASTNODE_H
