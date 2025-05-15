#include <iostream>
#include <sstream>
#include <antlr4-runtime.h>
#include <MathLexer.h>
#include <MathParser.h>
int main(int argc, char const *argv[])
{
    std::istringstream ifs("12");
    antlr4::ANTLRInputStream input(ifs);
    MathLexer lexer(&input);
    antlr4::CommonTokenStream ct(&lexer);
    MathParser parser(&ct);
    std::cout << parser.start()->toStringTree(true);
    return 0;
}
