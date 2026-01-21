# lilTomlParser

Kind of. Doesn't support tables in the slightest, yet.


## todo
  * [ ] Lexing/parsing rewrite
    * [x] Lexer
      * [ ] Fix comments: shouldn't skip WS inside them
      * [x] test
    * [ ] Parser
      * [ ] parsing loop _(started)_
        * [ ] return type strat(s)?
      * [ ] test
  * [x] Separate lexer from parser
  * [ ] Make current custom types an optional default implementation
    * [x] Genericise parser return
  * [-] CTest
    * [x] Lexer _(min)_
    * [ ] Parser _(started)_
    * [ ] Utils
    * [ ] `Config` handling
  * [ ] Allow non-static build
  * [ ] Allow optionally including [isocline](https://github.com/daanx/isocline) for prettyfication

## Issues

1. `Parser::run()` (or its internal loop) broken: not populating sections/kvp vecs
