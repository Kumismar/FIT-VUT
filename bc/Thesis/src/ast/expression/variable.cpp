/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-08 11:02
 * @ Modified time: 2024-04-16 13:50
 */

#include "variable.hpp"
#include "operand.hpp"
#include "token.hpp"

Variable::Variable(Token& token)
{
    this->nodeType = NodeType::nodeExpression;
    this->data.type = token.GetDataType();
    if (this->data.type == DataType::data_String) {
        this->name = std::string((*token.GetData().stringVal));
        return;
    }
}

void Variable::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"Variable: " << this->name << "\"];\n";
}