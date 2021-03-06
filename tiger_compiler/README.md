IR Tree Generation
==================

Sorry for the very late submission.
The only thing remaining is record types.

### Frames (frame.cc, frame.hh)

The frame keeps track of the stack as a double ended queue of integers, as well as the frame pointer, stack pointer, first temporary's address for each stack, and maps from the names of each variable of each type (locals, arguments, and temporaries), to their offset from that frame's frame pointer. 

pushframe() takes a vector mapping arguments passed names to their variables and a vector mapping locals names to their variables. popframe() pops the frame off the stack. 

The stack is a public variable so it can be directly manipulatable by other functions if necessary. Everything else is public because at that point might as well. The stack isn't strictly necessary, but it's useful for debugging purposes to have a representation of what we're keeping track of.

However, temporaries can be added, looked up, and popped to directly and immediately grab their fp offsets.


### Translation (ast_ir.cc, irtree.cc, irtee.hh)

The fucntion `conver_ast` which is defined in ast_ir.cc takes the produced AST and converts each AST node to and IR node. Every ASTNode has its own `convert_to_ir` method.

Additionally, each node has a method `get_var_names` used to get names to add to the frame.



Tiger Semantics
===============

Note: Mutual recursion is not working. We will fix by next assignment.

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
   * __Syntactic Errors__: syntactic errors are supposrted by Bison's `%define parse.error verbose`
     and our yyerror function which makes use of yylloc(defined in .y).
     Here is a sample error:  
     ```
      ERROR: line 1 column 5-7
             syntax error, unexpected FOR, expecting NAME 1
     ```
   * __Semantic Errors__: semantic errors are handled using the built in `'@'` functionality. @<token>.<x>,
                          where x can ask for first or last line or column and returns an integer. We pass
                          this integer to our AST classes and use that value to print the line number. Will
                          support column values in the near future.
     Here is a couple sample errors:
      ```
        ERROR: line 5
               in expression '("asdfghjkl" + 1)'
               Attempting binary operation on between 1 or more non-integer values
               (the types are ‘string’ and ‘int’)
   
        ERROR: line 1
               true and false condition expressions in 'if' statement must have the same type
               (types are ‘int’ and ‘string’)
       ```
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
   * Same variable name in different scopes (Should pass)
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
   * Repition of variables in function arguments or fields in records
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
   * Use of undeclared record fields
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
   * Function call on non-function type
   * Mismatch of array type and array elements
   * Function call with wrong return type or argument(s) type
   * Array indexing with non-integer
   * Instantiation of undeclared variables

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
