# lilTomlParser

Kind of. Doesn't support tables in the slightest, yet.


## todo
  * [ ] Lexing/parsing rewrite
    * [x] Lexer
      * [ ] Fix comments: shouldn't skip WS inside them
      * [x] test
    * [ ] Parser
      * [x] populate `m_input`
      * [ ] parsing loop _(started)_
        * [ ] return type strat(s)?
      * [ ] test
  * [ ] Separate lexer from parser
  * [ ] Make current custom types an optional default implementation
    * [ ] Genericise parser return somehow
        - _maybe tuple, maybe pair?_
  * [-] CTest
    * [x] Lexer _(min)_
    * [ ] Parser _(started)_
    * [ ] Utils
    * [ ] `Config` handling
  * [ ] Allow non-static build
  * [ ] Allow optionally including [isocline](https://github.com/daanx/isocline) for prettyfication
