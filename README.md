# -Building-a-Parser-for-the-PLSL

In this programming assignment, you will be building a parser for a simple programming language. The syntax definitions of the small programming language, 
called Pascal-Like Simple Language (PLSL), are given below using EBNF notations. The PLSL syntax has some features similar to the well-known Pascal Language.
Your implementation of a parser to the language is based on the following grammar rules specified in EBNF notations.


1. Prog ::= PROGRAM IDENT; DeclBlock ProgBody
2. DeclBlock ::= VAR {DeclStmt;}
3. DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
4. ProgBody ::= BEGIN {Stmt;} END
5. Stmt ::= AssigStmt | IfStmt | WriteLnStmt | ForStmt
6. WriteLnStmt ::= WRITELN (ExprList)
7. IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
8. ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
9. AssignStmt ::= Var := Expr
10. ExprList ::= Expr {, Expr}
11. Expr ::= Term {(+|-) Term}
12. Term ::= SFactor {( * | / ) SFactor}
13. SFactor ::= [(+ | -)] Factor
14. LogicExpr ::= Expr (= | > | <) Expr
15. Var ::= IDENT
16. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)

The following points describe the programming language. Note that not all of these points will be addressed in this assignment. 
However, they are listed in order to give you an understanding of the language semantics and what to be considered for implementing an interpreter for the language in
Programming Assignment 3. These points are:


1. The language has three types: INTEGER, REAL and STRING.

2. The precedence rules of the PSLP language are as shown in the table

3. The PLUS, MINUS, MULT, and DIV operators are left associative.

4. All variables have to be declared in declaration statements in the declaration block

5. An IfStmt evaluates a logical expression (LogicExpr) as a condition. If the logical expression value is true,
 then the Stmt block is executed, otherwise it is not. An else part for an IfSmt is optional.

6. A WriteLnStmt evaluates the list of expressions (ExprList), and prints their values in order from left to right followed by a newline.

7. The ASSOP operator (:=) in the AssignStmt assigns a value to a variable. It evaluates the Expr on the right-hand side and saves its value in 
a memory location associated with the left-hand side variable (Var). A left-hand side variable of a numeric type can be assigned a value of either one 
of the numeric types (i.e., INTEGER, REAL) of the language. For example, an integer variable can be assigned a real value, and a real variable can be assigned
an integer value. In either case, conversion of the value to the type of the variable must be applied. However, A left-hand side STRING variable can only
be assigned a STRING value.

8. The binary operations for addition, subtraction, multiplication, and division are performed upon two numeric operands (i.e., INTEGER, REAL) of the
 same or different types. If the operands are of the same type, the type of the result is the same type as the operator’s operands. Otherwise, the type
 of the result is REAL.
 
9. The EQUAL LTHAN and GTHAN relational operators operate upon two compatible operands. The evaluation of a logical expression, based on EQUAL, 
LTHAN or GTHAN operation, produces either a true or false value that controls whether the statement(s) of the selection IfStmt is executed or not.

10. It is an error to use a variable in an expression before it has been assigned.

12. The unary sign operators (+ or -) are applied upon unary numeric operands (i.e., INTEGER, REAL).
