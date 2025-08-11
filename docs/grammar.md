ASDL - abstract syntax Description Language

EBNF - extended backus-naur form

``` ebnf

program = 


```
jbnf
{} -> repeats 1 or more times  
[] -> optional  
<> -> nonterminal definition  
::= -> expands to  
? -> occurs exactly 0 or once, placed before value  
() -> scope class  
'' -> single character   
"" -> string  
={} -> list of values separated by a comma or newline or both  
| -> or other value  
# -> comment  
ASCII -> any ascii character  
lhs-rhs -> within range of lhs to rhs, where lhs and rhs can be any character  


Program ::= fndef

fndecl ::= "fn" ident '(' params ')' [ '->' datatype ] ';'   
fndef ::= "fn" ident '(' params ')' [ '->' datatype ] '{' {<stmt>} '}'   
    params ::= <param> | <param> ',' <params> | "nil"  
    param ::= datatype ident ['=' <literal>]  

stmt ::= <retstmt> |
retstmt ::= "return" <expr> ';'

expr ::= <literal> | <Arithexpr> | <groupexpr> | <bitexpr> | <logexpr>
groupexpr ::= '(' <expr> ')'
Arithexpr ::= <expr> <ArithOps> <Expr>
bitexpr ::= <expr> <bitOps> <expr>
logexpr ::= <expr> <logOps> <expr>


literal ::= <intlit> | <fltlit> | <chrlit> | <strlit>
intlit ::= {0-9}
fltlit ::= {0-9 | ?'.' | (?e {0-9 | ?'.'})}
chrlit ::= \'?ASCII\'
strlit ::= \"{ASCII}\"

ident ::= (a-zA-Z_){a-zA-Z0-9_}

ArithOps ::= '+' | '-' | '*' | '/' | '%'
bitOps ::= "&&" | "||" | "^^" | "!!"
logOps ::= '&' | '|' | '^' | '!' | '<' | '>' | "<=" | ">=" | "!=" | "=="

keywords = {
    fn,
    return,
    break,
    continue,
    do,
    while,
    for,
    if,
    else,
    goto,
    switch,
    case,
    default,
    auto,
    import,
    use,
    typedef,
    struct,
    enum,
    union,
    const,
    static,
    register,
    volatile,
}

datatypes = {  
    i8, int8, sbyte,  
    i16, int16, short,  
    i32, int32, int,  
    i64, int64, long, isize,  
    u0, uint0, nil,  
    u1, uint1, bool,  
    u8, uint8, byte, char,   
    u16, uint16, ushort,  
    u32, uint32, uint,   
    u64, uint64, ulong, usize,  
    f32, float32, float,  
    f64, float64, double,  
    f128, float128, ldouble,  
    str, 
    ptr, 
}

symbols = {  # unused but here for your knowledge  
    + 
    - 
    * 
    / 
    % 
    = 
    += 
    -= 
    *= 
    /= 
    %= 
    ++ 
    -- 
    & 
    | 
    ^ 
    ! 
    && 
    || 
    ^^ 
    !! 
    == 
    != 
    < 
    > 
    <= 
    >= 
    << 
    >> 
    @ 
    # 
    ; 
    , 
    : 
    . 
    -> 
    \ 
    (
    )
    {
    }
    [
    ]  
}

predefined macros = {
    NIL = ((nil*) 0), 

}

predefined typedefs = {
    nil    
}


precedence = {
    ()
    exp
    * / % 
    + -
}