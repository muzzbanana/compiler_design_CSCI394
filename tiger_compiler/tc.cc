#include <cstdio>
#include <cstdlib>

#include "ast.hh"
#include "semantic.hh"
#include "semantic_checks.hh"
#include "catch.hpp"
#include "tiger.tab.h"
#include "irtree.hh"

using namespace tiger;

int yyparse(ASTNode::ASTptr *out);
extern FILE *yyin;
extern int yylineno;

void print_instrs(FILE *out, InstructionList il) {
    for (auto a : il) {
        fprintf(out, "%s\n", a->toStr().c_str());
    }
}


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <output-file> <input-file>\n", argv[0]);
        exit(1);
    }

    FILE *inputfile = fopen(argv[2], "r");
    if (inputfile == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        exit(1);
    }

    FILE *outputfile = fopen(argv[1], "w");
    if (outputfile == NULL) {
        fprintf(stderr, "Unable to open output file\n");
        exit(1);
    }

    yyin = inputfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);

    if (ir == NULL) {
        fprintf(stderr, "Syntax error.\n");
        exit(1);
    }

    const ProgramFragment *fr = ir->vectorize();
    print_instrs(outputfile, fr->munch());

    delete output;
    fclose(inputfile);
    fclose(outputfile);
}


