Tiger Semantics
===============

We chose to implement our semantic checks using the **"functional style"**.
This means we maintain a permanent environment and create temporary ones when
needed.

### Types (type.hh, type.cc)
Types are defined using a class `Type`. This serves as the base for the for
   - BaseType which is used for ints, strings, nil, error, things that not yet implemented
     (in case we didn't complete the assignment), and as a hack a type to represent when
     we didn't find a type.
   - ArrayType for arrays
   - RecordType for records
   - FunctionType for functions
   

### Symbol Tables (semantic.hh, semantic.cc)
Symbol Tables are created using a class `SymbolTable`.
It uses a map to represent the symbol tables. The map is composed of pairs of
type string and type tiger_type which is a self defined variable that indicates
the type of a tiger variable.

This class has 2 simple functionalities beyond consturction and destruction.

    * insert : adds a string, type-tiger pair to the symbol table
    * lookup : takes a string and returns the type of the string


### Scopes (scope.hh, scope.cc)
Scopes are handled through a class `Scope`.
This is uses a vector of sybmol tables. We then mimic a stack using the vector.
The class has three simple functionalities:

    * push_scope   : this adds a new scope
    * pop_scope    : this removes the newest scope from the vector
    * preexisting  : this checks if a variable exists in its larger scope
    * search       : this takes a string and terates through the vector in reverse
                     order to check if variable has already been declared, if found,
                     it returns its type, if not returns error type
    * insert       : this takes a string and its corresponding type and adds the binding
                     to its parent scopes
                          
## Error Handling
Not working propely. Columns and lines do not update correctly. Works well for semantic errors
but not syntactic. Will fix soon.

## Testing

   * Type mismatch in expression
   ```
      "asdfghjkl" + 1
   ```
   * Type mismatch in function return type
   ```
     let
       function f () : int = "hi"
     in
       f()
     end 
   ```
   * Type mismatch in variable assignment and decleration
   ```
      let
        var a : int := 4
      in
        a := "hi"
      end
   ```
   * Type mismatch in if-then-else and while statements
   ```
      if 1 then 10 else "else"
   ```
   * Same variable name in different scopes
   ```
      let
         var x := 1
         function f(int: i) =
            let var x := 2
            in x + 1 end
       in
         f(4)
       end
   ```
   * Recursive and mutually recursive functions
     ``` 
      let
         function f (a : string) : string = f(15)
       in
         f("abcdefg")
       end
     ```
     ```
      let
         function f (i : int) = if i > 0 then g(i - 1) else 0
         function g (i : string) = if i > 0 then f(i - 1) else 1
      in
         f(5)
      end
     ```
   * Higher-order and nested functions
   * Undefined identifiers
   * Repeatedly defined identifiers
     ```
      let
         var portland_bridge_name := "Morrison Bridge"
         var portland_bridge_name := "Tillikum Crossing"
         var portland_bridge_name := "Burnside Bridge"
       in
         portland_bridge_name
       end
      ```
   * Misuse of reserved words (Syntactic Error)
   * Incorrect nesting (Syntactic Error)
   * Repition of variables in function arguments
     ```
      let 
         function f(i:int, j:int, i:string) = 1
       in
         f(1,2,"hello")
       end
      ```
   * Declerations with invalid types
     ```
      let
         type arrtype = array of dog
         var arr := arrtype [10] of 0
         var a : int := 0
       in
         a = arr[11]
       end
      ```
   * Bad record types
     ```
      let
         type fruit = { color : string, size : int }
         var banana : fruit := fruit { color="yellow", size=10 }
       in
         /* oh no this property doesn't exist */
         banana.tastiness := 4
       end
      ```
   * Indexing out of range of array
     ```
      let 
         type arrtype = array of int
         var arr : arrtype := int[10] of 0
       in
         a = arr[11]
       end
      ```

Tiger Parser
============

* The header file, ast.hh, contains 11 derived classes:
    - NilASTNode: this is to handle if and while statements that have false conditions
                  and therefore have no return value.
                  The parse will now print return `"nil"`.
    - NumASTNode: this returns a number.
    - StrASTNode: this returns a string.
    - NameASTNode: this returns a name token as a string.
    - UnaryASTNode: single child. This serves as a basis for unary operations. In our 
                    case, this is only used for unaryminus (negation).
    - NoEvalUnaryASTNode: single child. This is for types that need a single child in
                          the tree but are not evaluate-able. This is used in the case 
                          declerations and typing of variables.
    - BinaryASTNode: two chidlren. Serves as the basis for evaluate-able binary ops.
    - NoEvalBinaryASTNode: two children. For types that need two children but are not
                           evaluate-able. This is used for assignments, while loops, etc.
    - TertiaryASTNode: three children. Used for if-then-else, typed variable declerations,
                       arrays, etc.
    - QuatranaryASTNode: four children. This is used for for loops and typed function
                         declerations.
    - VectorASTNode: this is a special class used for types that require lists such as
                     declerations in let statements (decllist), members in field types
                     (fieldlist). The class makes use of a vector and has a method add_node
                     that adds elements to the end of the vector.
                         
 * toStr() now uses std::stringstram rather than std::to_string. Tokens are added to a buffer
   and then converted to a string when we reach end of tokens.
 
 * `%define parse.error verbose` for error handling, temporarily. Declared in .y file.
 * `%parse-param {tiger::ASTNode::ASTptr *out}` which is declared in .y file allows us to pass
   are parameter to yyparse. This was done so we could access the ASTNode classes and call 
   methods as we saw fit.
 * Use of `strdup()` in tiger.l was causing memory leaks because it we need to free the space allocated
   by `strdup()`. A simple way this was to create a function `name()` in tiger.y that creates a new
   ASTNode::ASTPtr and frees it.
 * **Changes were made to the lexer.**
    - the removal of ERROR tokens. We now use yyerror to report errors. We report the line of 
      the lexical error using yylineno.
    - use of literals rather than tokens in enum for binary ops. eg. LESS_EQUAL is now just '<='.
    - use of yyval.d and yyval.str for numbers and strings respectively.
   

Tiger lexer prior to parser
===========================

* Comments are handled by using *start conditions* in Lex. When a `/*` symbol
  is encountered, it switches into `<COMMENT>` mode, where it eats all characters
  until it encounters a matching `*/` sequence. To account for nested comments,
  it keeps track of a `comment_depth` variable which is incremented for each
  additional `/*` encountered in `<COMMENT>` mode, and decremented for each `*/`.
  When a `*/` is encountered that causes `comment_depth` to reach 0, the lexer
  exits `<COMMENT>` mode and goes back to `<INITIAL>` mode.

* If the lexer reaches the end of file (`<<EOF>>` token) while still in `<COMMENT>`
  mode, an error is printed out to `stderr` and an `ERROR_UNTERM_COMMENT` token is
  returned before the end-of-file.

* If a `*/` token is encountered while the lexer is in `<INITIAL>` mode -- that is,
  if the `*/` has no matching `/*` -- an `ERROR_COMMENT` token is returned.

* Strings are handled as a single token with the regular expression:
  `\"([^"\n\r]|\\.)*\"` -- that is:
    - a double quote
    - followed by zero or more repetitions of:
        * some character that isn't a quote (or a newline)
        * or a backslash followed by a single character
    - followed by another double quote.

* Unterminated strings are handled by a second regex, matched after the first string
  regex, which is identical but missing the last quote. This will only be matched on
  strings that are missing a second quote mark, and an `ERROR_UNTERM_STRING` token
  will be returned.
