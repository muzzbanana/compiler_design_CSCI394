/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 5 "tiger.y" /* yacc.c:339  */

#include <iostream>
#include <string>
#include "ast.hh"
#include "semantic_checks.hh"
//#define YY_DECL extern "C" int yylex()

using namespace std;

extern "C" int yylex(void);
extern "C" int yylineno;
void yyerror(tiger::ASTNode::ASTptr *out, const char *);
tiger::ASTNode::ASTptr name(const char *str);

#line 81 "tiger.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "tiger.tab.h".  */
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
#line 1 "tiger.y" /* yacc.c:355  */

    using namespace tiger;

#line 115 "tiger.tab.c" /* yacc.c:355  */

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
#line 20 "tiger.y" /* yacc.c:355  */

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

#line 184 "tiger.tab.c" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */

#line 215 "tiger.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   313

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,     2,
      44,    45,    41,    39,    50,    40,    43,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    49,
      36,    35,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,    33,    47,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    95,    95,    99,   102,   104,   106,   108,   110,   112,
     114,   116,   118,   120,   122,   124,   126,   128,   130,   132,
     134,   136,   138,   140,   142,   144,   146,   148,   150,   152,
     154,   158,   160,   164,   166,   170,   172,   176,   179,   182,
     186,   189,   194,   197,   202,   204,   208,   210,   212,   216,
     219,   222,   226,   228,   230,   234,   238,   240,   242,   246,
     248,   252,   255,   260,   264,   266,   270,   272
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "STR", "NAME", "ARRAY", "IF",
  "THEN", "ELSE", "WHILE", "FOR", "TO", "DO", "LET", "IN", "END", "OF",
  "BREAK", "NIL", "FUNCTION", "VAR", "TYPE", "IMPORT", "PRIMITIVE",
  "CLASS", "EXTENDS", "METHOD", "NEW", "ASSIGN", "POINTIES", "LESS_EQUAL",
  "GREATER_EQUAL", "'|'", "'&'", "'='", "'<'", "'>'", "'['", "'+'", "'-'",
  "'*'", "'/'", "'.'", "'('", "')'", "'{'", "'}'", "']'", "';'", "','",
  "':'", "$accept", "program", "expr", "exprlist_opt", "exprseq_opt",
  "fieldlist_opt", "exprseq", "exprlist", "fieldlist", "lvalue",
  "lvalue_not_id", "decllist", "declaration", "typedecl", "type",
  "typefields_opt", "typefields", "typefield", "vardecl", "funcdecl", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   124,    38,    61,    60,    62,    91,    43,
      45,    42,    47,    46,    40,    41,   123,   125,    93,    59,
      44,    58
};
# endif

#define YYPACT_NINF -34

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-34)))

#define YYTABLE_NINF -34

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-34)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     140,   -34,   -34,   -31,   140,   140,    -2,    34,   -34,   -34,
     140,    63,    14,   245,   -23,   -19,   140,   140,    16,    -7,
     169,    22,    17,    47,    52,    64,   -34,   -34,   -34,   -34,
     -30,    12,   245,    18,    13,   -34,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,    66,
     140,   207,   245,    27,    25,    41,    31,    30,   140,   140,
     140,    39,     2,    54,   -34,   123,   -34,   -34,   -34,   140,
     134,   134,   134,   258,   271,   134,   134,   134,   -30,   -30,
     -34,   -34,   245,   -34,   226,    74,   -34,   140,   140,   -34,
      87,    81,   245,   156,    88,   140,    89,     3,    80,   245,
     -34,   140,   245,   245,    62,   140,   140,    48,    53,    50,
     -34,   245,    73,   -34,    92,    88,   -34,   -34,   245,   140,
     245,   194,    99,   -33,    88,   140,   105,    85,   245,   140,
     -34,   140,   130,   -34,   245,   -34,   -34,   245,   245,   101,
     140,   245
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     3,    44,     0,     0,     0,     0,    29,     5,
       0,     0,     0,     2,     6,    45,     0,    31,    35,     0,
       0,     0,     0,     0,     0,     0,    49,    52,    53,    54,
       7,     0,    37,     0,    34,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,    32,     0,     0,    36,     0,     0,
       0,     0,     0,     0,    51,     0,    50,    39,    22,     0,
      13,    16,    17,    19,    18,    12,    14,    15,     8,     9,
      10,    11,    20,    46,     0,    47,    21,     0,     0,    23,
       0,    25,    27,     0,    59,     0,     0,     0,     0,    38,
      48,     0,    41,    42,     0,     0,     0,     0,     0,    60,
      61,    64,     0,    56,     0,    59,    55,    30,    24,     0,
      26,     0,     0,     0,     0,     0,     0,     0,    43,     0,
      63,     0,     0,    62,    65,    58,    57,    28,    66,     0,
       0,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -34,   -34,     0,   -34,    83,   -34,   -34,   -34,   -34,   -34,
     -34,   -34,   113,   -34,   -34,    37,   -34,    29,   -34,   -34
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    32,    53,    33,    56,    34,    54,    57,    14,
      15,    25,    26,    27,   116,   108,   109,   110,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,    58,   131,    21,    19,    20,    48,    16,   113,   114,
      30,    46,    47,    17,    35,    18,    51,    52,   132,    50,
      49,    55,    61,    36,    37,    38,    39,    40,    41,    42,
      43,    95,    44,    45,    46,    47,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,   115,
      84,    60,    62,    96,    22,    23,    24,    63,    91,    92,
      93,    67,    69,    68,    31,    64,     1,     2,     3,    99,
       4,    83,    86,     5,     6,    87,    88,     7,    89,    65,
      90,     8,     9,    94,    22,    23,    24,   102,   103,    97,
     105,   101,   104,   107,   112,   111,   117,   119,   123,   122,
     124,   118,   125,    10,   130,   120,   121,    11,   -33,   126,
     135,    36,    37,    38,    39,    40,    41,    42,    43,   128,
      44,    45,    46,    47,    31,   134,     1,     2,     3,   137,
       4,   138,   136,     5,     6,   139,   140,     7,    66,   -33,
     141,     8,     9,     1,     2,     3,     0,     4,    98,     0,
       5,     6,   127,   133,     7,     0,     0,     0,     8,     9,
       0,     0,     0,    10,   -34,   -34,   -34,    11,   106,   -34,
     -34,   -34,     0,    44,    45,    46,    47,     0,     0,     0,
      10,     0,    59,     0,    11,     0,    36,    37,    38,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,    36,
      37,    38,    39,    40,    41,    42,    43,   129,    44,    45,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    37,    38,    39,    40,    41,
      42,    43,     0,    44,    45,    46,    47,    36,    37,    38,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
       0,     0,     0,     0,     0,    85,    36,    37,    38,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,     0,
       0,     0,     0,     0,   100,    36,    37,    38,    39,    40,
      41,    42,    43,     0,    44,    45,    46,    47,    36,    37,
      38,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,    36,    37,    38,     0,     0,    41,    42,    43,     0,
      44,    45,    46,    47
};

static const yytype_int16 yycheck[] =
{
       0,     8,    35,     5,     4,     5,    29,    38,     5,     6,
      10,    41,    42,    44,     0,    46,    16,    17,    51,    38,
      43,     5,     5,    30,    31,    32,    33,    34,    35,    36,
      37,    29,    39,    40,    41,    42,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    46,
      50,    29,     5,    51,    20,    21,    22,     5,    58,    59,
      60,    49,    49,    45,     1,     1,     3,     4,     5,    69,
       7,     5,    45,    10,    11,    50,    35,    14,    47,    15,
      50,    18,    19,    44,    20,    21,    22,    87,    88,    35,
       9,    17,     5,     5,     5,    95,    16,    35,    45,    51,
      50,   101,    29,    40,     5,   105,   106,    44,    45,    17,
       5,    30,    31,    32,    33,    34,    35,    36,    37,   119,
      39,    40,    41,    42,     1,   125,     3,     4,     5,   129,
       7,   131,    47,    10,    11,     5,    35,    14,    25,    16,
     140,    18,    19,     3,     4,     5,    -1,     7,    65,    -1,
      10,    11,   115,   124,    14,    -1,    -1,    -1,    18,    19,
      -1,    -1,    -1,    40,    30,    31,    32,    44,    12,    35,
      36,    37,    -1,    39,    40,    41,    42,    -1,    -1,    -1,
      40,    -1,    13,    -1,    44,    -1,    30,    31,    32,    33,
      34,    35,    36,    37,    -1,    39,    40,    41,    42,    30,
      31,    32,    33,    34,    35,    36,    37,    13,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    41,    42,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    48,    30,    31,    32,    33,
      34,    35,    36,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    30,    31,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    30,    31,
      32,    -1,    34,    35,    36,    37,    -1,    39,    40,    41,
      42,    30,    31,    32,    -1,    -1,    35,    36,    37,    -1,
      39,    40,    41,    42
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,    10,    11,    14,    18,    19,
      40,    44,    53,    54,    61,    62,    38,    44,    46,    54,
      54,     5,    20,    21,    22,    63,    64,    65,    70,    71,
      54,     1,    54,    56,    58,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    39,    40,    41,    42,    29,    43,
      38,    54,    54,    55,    59,     5,    57,    60,     8,    13,
      29,     5,     5,     5,     1,    15,    64,    49,    45,    49,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,     5,    54,    48,    45,    50,    35,    47,
      50,    54,    54,    54,    44,    29,    51,    35,    56,    54,
      48,    17,    54,    54,     5,     9,    12,     5,    67,    68,
      69,    54,     5,     5,     6,    46,    66,    16,    54,    35,
      54,    54,    51,    45,    50,    29,    17,    67,    54,    13,
       5,    35,    51,    69,    54,     5,    47,    54,    54,     5,
      35,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    62,    62,    63,
      63,    63,    64,    64,    64,    65,    66,    66,    66,    67,
      67,    68,    68,    69,    70,    70,    71,    71
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     3,     4,     6,     4,     6,     4,     8,     1,
       5,     0,     1,     0,     1,     0,     1,     1,     3,     2,
       1,     3,     3,     5,     1,     1,     3,     4,     4,     1,
       2,     2,     1,     1,     1,     4,     1,     3,     3,     0,
       1,     1,     3,     3,     4,     6,     7,     9
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (out, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, out); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, tiger::ASTNode::ASTptr *out)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (out);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, tiger::ASTNode::ASTptr *out)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, out);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, tiger::ASTNode::ASTptr *out)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , out);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, out); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, tiger::ASTNode::ASTptr *out)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (out);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (tiger::ASTNode::ASTptr *out)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 95 "tiger.y" /* yacc.c:1646  */
    {
       *out = (yyvsp[0].ast);
  }
#line 1519 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 99 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new StrASTNode((yyvsp[0].str));
        free((char*)(yyvsp[0].str)); // free the duplicated string
  }
#line 1528 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 102 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new NumASTNode((yyvsp[0].d));
  }
#line 1536 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 104 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new NilASTNode();
  }
#line 1544 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 106 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = (yyvsp[0].ast);
  }
#line 1552 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 108 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new UnaryMinusASTNode("-", (yyvsp[0].ast));
  }
#line 1560 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 110 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new PlusASTNode("+", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1568 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 112 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new MinusASTNode("-", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1576 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 114 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new MultASTNode("*", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1584 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 116 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new DivASTNode("/", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1592 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 118 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new EqualASTNode("=", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1600 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 120 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new NotEqualASTNode("<>", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1608 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 122 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new LessASTNode("<", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1616 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 124 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new GreaterASTNode(">", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1624 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 126 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new LessEqualASTNode("<=", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1632 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 128 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new GreaterEqualASTNode(">=", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1640 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 130 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new LogicalAndASTNode("&", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1648 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 132 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new LogicalOrASTNode("|", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1656 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 134 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new AssignASTNode(":=", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1664 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 136 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new FuncCallASTNode("(", "(", "))", name((yyvsp[-3].str)), (yyvsp[-1].exprseq_ast), false);
  }
#line 1672 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 138 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = (yyvsp[-1].exprseq_ast);
  }
#line 1680 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 140 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new TypeInstASTNode("", "{", "}", name((yyvsp[-3].str)), (yyvsp[-1].ast));
  }
#line 1688 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 142 "tiger.y" /* yacc.c:1646  */
    {     /* array */
        (yyval.ast) = new ArrayASTNode("", " [", "] of ", name((yyvsp[-5].str)), (yyvsp[-3].ast), (yyvsp[0].ast), false);
  }
#line 1696 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 144 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new ConditionalASTNode("if", "then", "else", (yyvsp[-2].ast), (yyvsp[0].ast), new NilASTNode());
  }
#line 1704 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 146 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new ConditionalASTNode("if", "then", "else", (yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1712 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 148 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new WhileLoopASTNode("while", "do", (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1720 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 150 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new ForLoopASTNode("for", ":=", "to", "do", name((yyvsp[-6].str)), (yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
  }
#line 1728 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 152 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new BreakASTNode();
  }
#line 1736 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 154 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new LetASTNode("let", "in", "end", (yyvsp[-3].decllist_ast), (yyvsp[-1].exprseq_ast));
  }
#line 1744 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 158 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = new ExprSeqASTNode(", ");
  }
#line 1752 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 160 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = (yyvsp[0].exprseq_ast);
  }
#line 1760 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 164 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = new ExprSeqASTNode("; ");
  }
#line 1768 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 166 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = (yyvsp[0].exprseq_ast);
  }
#line 1776 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 170 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new FieldListASTNode(", ");
  }
#line 1784 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 172 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = (yyvsp[0].fieldlist_ast);
  }
#line 1792 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 176 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = new ExprSeqASTNode("; ");
        (yyval.exprseq_ast)->add_node((yyvsp[0].ast));
  }
#line 1801 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 179 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = (yyvsp[-2].exprseq_ast);
        (yyval.exprseq_ast)->add_node((yyvsp[0].ast));
  }
#line 1810 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 182 "tiger.y" /* yacc.c:1646  */
    {
        yyerrok;
  }
#line 1818 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 186 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = new ExprSeqASTNode(", ");
        (yyval.exprseq_ast)->add_node((yyvsp[0].ast));
  }
#line 1827 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 189 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.exprseq_ast) = (yyvsp[-2].exprseq_ast);
        (yyval.exprseq_ast)->add_node((yyvsp[0].ast));
  }
#line 1836 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 194 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.fieldlist_ast) = new FieldListASTNode(", ");
        (yyval.fieldlist_ast)->add_node(new FieldMemberASTNode("=", name((yyvsp[-2].str)), (yyvsp[0].ast), false));
  }
#line 1845 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 197 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.fieldlist_ast) = (yyvsp[-4].fieldlist_ast);
        (yyval.fieldlist_ast)->add_node(new FieldMemberASTNode("=", name((yyvsp[-2].str)), (yyvsp[0].ast), false));
  }
#line 1854 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 202 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = name((yyvsp[0].str));
  }
#line 1862 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 204 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = (yyvsp[0].ast);
  }
#line 1870 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 208 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new DotASTNode("(", ".", ")", (yyvsp[-2].ast), name((yyvsp[0].str)), false);
  }
#line 1878 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 210 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new IndexASTNode("", "[", "]", name((yyvsp[-3].str)), (yyvsp[-1].ast), false);
  }
#line 1886 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 212 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new IndexASTNode("", "[", "]", (yyvsp[-3].ast), (yyvsp[-1].ast), false);
  }
#line 1894 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 216 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.decllist_ast) = new DeclListASTNode(" ");
        (yyval.decllist_ast)->add_node((yyvsp[0].decl_ast));
  }
#line 1903 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 219 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.decllist_ast) = (yyvsp[-1].decllist_ast);
        (yyval.decllist_ast)->add_node((yyvsp[0].decl_ast));
  }
#line 1912 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 222 "tiger.y" /* yacc.c:1646  */
    {
        yyerrok;
  }
#line 1920 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 226 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.decl_ast) = new DeclarationASTNode("", (yyvsp[0].ast));
  }
#line 1928 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 228 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.decl_ast) = new DeclarationASTNode("", (yyvsp[0].ast));
  }
#line 1936 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 230 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.decl_ast) = new DeclarationASTNode("", (yyvsp[0].ast));
  }
#line 1944 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 234 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new TypeDeclASTNode("type", "=", name((yyvsp[-2].str)), (yyvsp[0].type_ast));
  }
#line 1952 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 238 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.type_ast) = new TypeASTNode("", name((yyvsp[0].str)));
  }
#line 1960 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 240 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.type_ast) = new TypeASTNode("{ ", " }", (yyvsp[-1].rectype_ast));
  }
#line 1968 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 242 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.type_ast) = new TypeASTNode("", new ArrayTypeASTNode("array of ", name((yyvsp[0].str))));
  }
#line 1976 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 246 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.rectype_ast) = new RecordTypeASTNode(", ");
  }
#line 1984 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 248 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.rectype_ast) = (yyvsp[0].rectype_ast);
  }
#line 1992 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 252 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.rectype_ast) = new RecordTypeASTNode(", ");
        (yyval.rectype_ast)->add_node((yyvsp[0].recfield_ast));
  }
#line 2001 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 255 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.rectype_ast) = (yyvsp[-2].rectype_ast);
        (yyval.rectype_ast)->add_node((yyvsp[0].recfield_ast));
  }
#line 2010 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 260 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.recfield_ast) = new RecordFieldASTNode(":", name((yyvsp[-2].str)), name((yyvsp[0].str)), false);
  }
#line 2018 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 264 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new UntypedVarDeclASTNode("var", ":=", name((yyvsp[-2].str)), (yyvsp[0].ast));
  }
#line 2026 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 266 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new TypedVarDeclASTNode("var", ":", ":=", name((yyvsp[-4].str)), name((yyvsp[-2].str)), (yyvsp[0].ast));
  }
#line 2034 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 270 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new UnTypedFuncDeclASTNode("(function ", "(", ") = ", ")", name((yyvsp[-5].str)), (yyvsp[-3].rectype_ast), (yyvsp[0].ast), false);
  }
#line 2042 "tiger.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 272 "tiger.y" /* yacc.c:1646  */
    {
        (yyval.ast) = new TypedFuncDeclASTNode("(function ", "(", "):", "=", ")", name((yyvsp[-7].str)), (yyvsp[-5].rectype_ast), name((yyvsp[-2].str)), (yyvsp[0].ast), false);
  }
#line 2050 "tiger.tab.c" /* yacc.c:1646  */
    break;


#line 2054 "tiger.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (out, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (out, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, out);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, out);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (out, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, out);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, out);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 276 "tiger.y" /* yacc.c:1906  */


void yyerror(tiger::ASTNode::ASTptr *out, const char *error) {
    cerr << "ERROR: line " << yylineno << " column " << yylloc.first_column << "-" << yylloc.last_column << endl;
    cerr << error << " " << yylloc.first_line << endl;
}

void error_reporting() {
  cerr << "ERROR: line " << yylineno << " column " << yylloc.first_column << "-" << yylloc.last_column << endl;
}

/* Create a new NameASTNode from a duplicated const char* without causing a memory leak.
 * This is necessary because flex calls strdup() in order to make sure the pointer to the
 * string doesn't change, but this results in a memory leak if the newly duplicated string
 * is not freed properly. */
tiger::ASTNode::ASTptr name(const char *str) {
    tiger::ASTNode::ASTptr new_node = new NameASTNode(str);
    free((char*)str); // free the duplicated string after it's safely stored in a std::string
    return new_node;
}
