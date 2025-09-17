    /*
    *  Author: Ondřej Koumar
    *  Date: 2024-03-18
    */

%option header-file="lex.yy.h" pointer

%{
#include "token.hpp"
#include "lexical_error.hpp"
%}

%{
    TokenType previousToken = t_Eps;
%}

%x COMMENT
%x LINE_COMMENT
%x STRING

%%

\n              { yylineno++; }
[ \t]           { /* ignore whitespace */ }
"if"            { Token::AddToken(t_If, DataType::data_None); }
"while"         { Token::AddToken(t_While, DataType::data_None); }
"for"           { Token::AddToken(t_For, DataType::data_None); }
"return"        { Token::AddToken(t_Return, DataType::data_None); }
";"             { Token::AddToken(t_Semi, DataType::data_None); }
"elseif"        { Token::AddToken(t_Elseif, DataType::data_None); }
"else"          { Token::AddToken(t_Else, DataType::data_None); }
"("             { Token::AddToken(t_LPar, DataType::data_None); }
")"             { Token::AddToken(t_RPar, DataType::data_None); }
"{"             { Token::AddToken(t_LCurl, DataType::data_None); }
"}"             { Token::AddToken(t_RCurl, DataType::data_None); }
"+"             { Token::AddToken(t_Plus, DataType::data_None); }
"*"             { Token::AddToken(t_Mul, DataType::data_None); }
"/"             { Token::AddToken(t_Div, DataType::data_None); }
"=="            { Token::AddToken(t_Eq, DataType::data_None); }
"!="            { Token::AddToken(t_NEq, DataType::data_None); }
"<"             { Token::AddToken(t_Less, DataType::data_None); }
"<="            { Token::AddToken(t_LEq, DataType::data_None); }
">"             { Token::AddToken(t_Greater, DataType::data_None); }
">="            { Token::AddToken(t_GEq, DataType::data_None); }
"="             { Token::AddToken(t_Assign, DataType::data_None); }
"&&"            { Token::AddToken(t_And, DataType::data_None); }
"||"            { Token::AddToken(t_Or, DataType::data_None); }
"!"             { Token::AddToken(t_Excl, DataType::data_None); }
","             { Token::AddToken(t_Comma, DataType::data_None); }
":"             { Token::AddToken(t_Colon, DataType::data_None); }
"."             { Token::AddToken(t_Concat, DataType::data_None); }
"function"      { Token::AddToken(t_Function, DataType::data_None); }
[0-9]+          { Token::AddToken(t_Const, DataType::data_Int); }
[0-9]+"."[0-9]+ { Token::AddToken(t_Const, DataType::data_Float); }
"true"|"false"  { Token::AddToken(t_Const, DataType::data_Bool); }
"int"           { Token::AddToken(t_Int, DataType::data_None); }
"float"         { Token::AddToken(t_Float, DataType::data_None); }
"bool"          { Token::AddToken(t_Bool, DataType::data_None); }
"string"        { Token::AddToken(t_String, DataType::data_None); }
\"(\\.|[^\\"])*\" { Token::AddToken(t_Const, DataType::data_String); }

[a-zA-Z_][a-zA-Z0-9_]* { 
    int next_char = yyinput();
    while (next_char == ' ' || next_char == '\n' || next_char == '\t')
        next_char = yyinput();

    if (next_char == '(')
        Token::AddToken(t_FuncName, DataType::data_String);
    else
        Token::AddToken(t_Variable, DataType::data_String);
    unput(next_char);
}

"-" { 
    if (previousToken == t_Variable || previousToken == t_Const || previousToken == t_RPar)
        Token::AddToken(t_Minus, DataType::data_None);
    else
        Token::AddToken(t_UnMinus, DataType::data_None);
}

"//" {
    BEGIN(LINE_COMMENT); 
}

"/*" {
    BEGIN(COMMENT); 
}

<COMMENT>"*/" {
    BEGIN(INITIAL); 
}

<COMMENT>\n {
    yylineno++;
}

<COMMENT>. {
}

<LINE_COMMENT>"\n" {
    yylineno++;
    BEGIN(INITIAL); 
}

<LINE_COMMENT>. {
}

. { throw LexicalError("Unknown lexeme: " + std::string(yytext) + std::string("on line ") + std::to_string(yylineno) + "\n"); }

%%

int yywrap()
{
    return 1;
}