Tiger lexer
===========

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
