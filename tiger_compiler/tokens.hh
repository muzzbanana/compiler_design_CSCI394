enum _Tokens {ARRAY = 257,
IF,
THEN,
ELSE,       // 260
WHILE,
FOR,
TO,
DO,
LET,
IN,
END,
OF,
BREAK,
NIL,        // 270
FUNCTION,
VAR,
TYPE,
IMPORT,
PRIMITIVE,
CLASS,
EXTENDS,
METHOD,
NEW,
COMMA,      // 280
COLON,
SEMICOLON,
OPEN_PAREN,
CLOSE_PAREN,
OPEN_BRACKET,
CLOSE_BRACKET,
OPEN_CURLY,
CLOSE_CURLY,
STOP,
PLUS,       // 290
MINUS,
MULT,
DIV,
EQUAL,
POINTIES,
LESS,
LESS_EQUAL,
GREATER,
GREATER_EQUAL,
AND,        // 300
OR,
ASSIGN,
END_OF_LINE,
COMMENT,
/* 'int' and 'string' aren't in the specification --
 * are they just supposed to be identifiers? that
 * seems kind of weird */
STRING,
STRING_LITERAL,
INTEGER,
INTEGER_LITERAL,
SPACE,
ERORR_ESCAPE, // 310
ERROR_COMMENT,
IDENTIFIER};
