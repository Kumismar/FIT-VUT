/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 21:56
 * @ Modified time: 2024-04-30 15:06
 */

#include "symbol_handler.hpp"
#include "ast.hpp"
#include "ast_node.hpp"
#include "ast_node_factory.hpp"
#include "grammar_factory.hpp"
#include "internal_error.hpp"
#include "logger.hpp"
#include "nonterminal.hpp"
#include "stack_item.hpp"
#include "token.hpp"

SymbolHandler::SymbolHandler(AnalysisStack& pushdown)
    : stack(pushdown)
{
}

void SymbolHandler::Expand(bool parsingFunction, LLTableIndex& tableItem)
{
    Logger* logger = Logger::GetInstance();
    Symbol* stackTop = this->stack.front();
    auto* stackNT = dynamic_cast<Nonterminal*>(stackTop->Clone());

    logger->AddLeftSide(stackTop);

    // If parsing function call, do not pop Expression from stack, Precedence parser is in charge of it.
    if (!(parsingFunction && *stackNT == NonterminalType::nt_Expression)) {
        delete stackTop;
        this->stack.pop_front();
    }

    // Get grammar based on grammar number in table item and push expanded nonterminal to stack.
    Grammar* grammar = GrammarFactory::CreateGrammar(tableItem.grammarNumber);
    this->expandedRightSide = grammar->Expand(tableItem.ruleNumber);
    this->pushRule(stackNT);

    logger->AddRightSide(this->expandedRightSide);
    logger->PrintRule();

    delete grammar;
    delete stackNT;
}

void SymbolHandler::Pop()
{
    Token* inputToken = inputTape.front();

    // AST nodes gradually add information to themselves based on tokens parsed.
    ASTNode* context = AST::GetInstance()->GetCurrentContext();
    if (context != nullptr) {
        context->ProcessToken(*inputToken);
    }
    Logger::GetInstance()->AddTokenToRecents(*inputToken);

    delete inputTape.front();
    inputTape.pop_front();
    delete this->stack.front();
    this->stack.pop_front();
}

bool SymbolHandler::returnedEpsilon()
{
    Symbol* front = this->expandedRightSide.front();
    if (this->expandedRightSide.size() == 1 && front->GetSymbolType() == SymbolType::symb_Token) {
        auto* t = dynamic_cast<Token*>(front);
        if (t == nullptr) {
            throw InternalError("Dynamic cast to Token* failed, real type: " + std::string(typeid(front).name()) + "\n");
        }
        return *t == TokenType::t_Eps;
    }
    return false;
}

void SymbolHandler::pushRule(Nonterminal* stackNT)
{
    AST* ast = AST::GetInstance();
    // If right side is not epsilon, it will be pushed and ASTNode will be created.
    if (!this->returnedEpsilon()) {
        ASTNode* node = ASTNodeFactory::CreateASTNode(*stackNT, *inputTape.front());
        ASTNode* context = ast->GetCurrentContext();
        // If nonterminal doesnt have corresponding AST node, nullptr is returned.
        if (node != nullptr && context != nullptr /* unit tests only */) {
            context->LinkNode(node, *stackNT);
            ast->PushContext(node);
        }

        // Push the expanded rule to the stack.
        for (Symbol* item: this->expandedRightSide) {
            this->stack.push_front(item->Clone());
        }
    }
}
