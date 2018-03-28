/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_TIGER_TAB_H_INCLUDED
# define YY_YY_TIGER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "tiger.y" /* yacc.c:1909  */

    using namespace tiger;

#line 48 "tiger.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    STR = 259,
    NAME = 260,
    ARRAY = 261,
    IF = 262,
    THEN = 263,
    ELSE = 264,
    WHILE = 265,
    FOR = 266,
    TO = 267,
    DO = 268,
    LET = 269,
    IN = 270,
    END = 271,
    OF = 272,
    BREAK = 273,
    NIL = 274,
    FUNCTION = 275,
    VAR = 276,
    TYPE = 277,
    IMPORT = 278,
    PRIMITIVE = 279,
    CLASS = 280,
    EXTENDS = 281,
    METHOD = 282,
    NEW = 283,
    ASSIGN = 284,
    POINTIES = 285,
    LESS_EQUAL = 286,
    GREATER_EQUAL = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "tiger.y" /* yacc.c:1909  */

    double              d;
    const char*         str;

    /* all these will need to be declared in ast.hh */
    /* depending on our implementation, we might need all these,
       certain types might be described using the same union
       member. we can edit that as we go. */
    tiger::ASTNode::ASTptr     ast;
    tiger::ASTNode::value_t    val;

    tiger::DeclListASTNode*    decllist_ast;
    tiger::DeclarationASTNode*  decl_ast;

    tiger::FieldListASTNode*   fieldlist_ast;

    tiger::ExprSeqASTNode*     exprseq_ast;

    tiger::TypeASTNode*        type_ast;

    tiger::RecordFieldASTNode* recfield_ast;
    tiger::RecordTypeASTNode*  rectype_ast;

#line 117 "tiger.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (tiger::ASTNode::ASTptr *out);

#endif /* !YY_YY_TIGER_TAB_H_INCLUDED  */
