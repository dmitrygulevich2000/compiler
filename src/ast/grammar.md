```ebnf
<file> ::= <declaration>*

////////////////////////////////////////////////////////////////////////////////////
//!                             Declarations
////////////////////////////////////////////////////////////////////////////////////

<declaration> ::= <function-declaration>
                | <variable-declaration>

<function-declaration> ::= fun <identifier> IDENT(arg)* = <expression> ;

<variable-declaration> ::= var IDENT(name) IDENT(type)? = <expression> ;

////////////////////////////////////////////////////////////////////////////////////
//!                             Statements
////////////////////////////////////////////////////////////////////////////////////

<statement> ::= <expression>
              | <declaration> 
              | <expression-statement>
              | <assignment-statement>

<expression-statement> ::= <expression> ;

<assignment-statement> ::= <unary-expression> = <expression> ;

////////////////////////////////////////////////////////////////////////////////////
//!                             Expressions
////////////////////////////////////////////////////////////////////////////////////

<expression> ::= <equality-expression>

// https://en.cppreference.com/w/c/language/operator_precedence
// Precendence 7
<equality-expression> ::= <comparison-expression>
                        | <equality-expression> == <relational-expression>
                        | <equality-expression> != <relational-expression>

// Precendence 6
<comparison-expression> ::= <additivee-expression>
                          | <comparison-expression> < <additive-expression>
                          | <comparison-expression> <= <additive-expression>
                          | <comparison-expression> > <additive-expression>
                          | <comparison-expression> >= <additive-expression>

// Precendence 4
<additive-expression> ::= <multiplicative-expression>
                        | <additive-expression> + <multiplicative-expression>
                        | <additive-expression> - <multiplicative-expression>

// Precendence 3
<multiplicative-expression> ::= <unary-expression>
                              | <multiplicative-expression> * <unary-expression>
                              | <multiplicative-expression> / <unary-expression>
                              | <multiplicative-expression> % <unary-expression>

// Precendence 2
<unary-expression> ::= <postfix-expression>
                     | <keyword expression>
                     | - <unary-expression>
                     | ! <unary-expression>

// Precendence 1
<postfix-expression> ::= <primary-expression>
                       | <fn-call-expression>
                       | <field-access-expression>

<fn-call-expression> ::= <postfix-expression> ( {<expression>,}* )

<field-access-expression> ::= <postfix-expression> . IDENT(field)

// Primary
<primary-expression> ::= <literal-expression>
                       | <var-access-expression>

                       | ( <expression> )

<literal-expression> ::= NUMBER
                       | STRING
                       | CHAR
                       | TRUE
                       | FALSE

<var-access-expression> ::= IDENT(name)

// Keyword
<keyword expression> ::= <if-expression>
                       | <block-expression>
                       | <return-expression>

<if-expression> ::= if <variable-declaration>? <expression> then? <expression> {else <expression>}?

<block-expression> ::= { <statement>* <expression>? }

<return-expression> ::= return <expression> ;?

////////////////////////////////////////////////////////////////////////////////////
```
