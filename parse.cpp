/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
*/


#include "parse.h"

using namespace std;

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}







//Prog ::= PROGRAM IDENT; DeclBlock ProgBody

bool Prog(istream& in, int& line){
   bool status;
    LexItem progVal;
    
    progVal = Parser::GetNextToken(in, line);
  

    if(progVal != PROGRAM){
        ParseError(line, "Missing PROGRAM");
        return false;
    }

    LexItem identVal = Parser::GetNextToken(in,line);

   
    if(identVal != IDENT){
        ParseError(line, "Missing Program Name");
        return false;
    }

    LexItem semiVal = Parser::GetNextToken(in, line);
   
    if(semiVal != SEMICOL){
        ParseError(line, "Missing SemiColon");
        return false;
    }

    status = DeclBlock(in, line);

    if(!status){
        ParseError(line, "Incorrect Declaration Section.");
        return false;
    }

    LexItem beginVal = Parser::GetNextToken(in, line);

   

    if(beginVal != BEGIN){
       ParseError(line, "Non-recognizable Program Block");
      ParseError(line, "Incorrect Program Section");
      return false;
   }

    status = ProgBody(in, line);

    if(!status){
        ParseError(line, "Incorrect Program Body");
        return false;
    }

   LexItem endVal = Parser::GetNextToken(in, line);
  

   
    
   if(endVal != DONE){
       ParseError(line, "Non-recognizable Program Block");
       ParseError(line, "Incorrect Program Section");
       return false;
   }

    

    return status;
}





//DeclBlock ::= VAR {DeclStmt;}


bool DeclBlock(istream& in, int& line){
    bool status = false;
    
    LexItem varVal = Parser::GetNextToken(in, line);

   

    if(varVal != VAR){
        ParseError(line, "Non-recognizable Declaration Block.");
        return false;
    }
    
    status = DeclStmt(in,line);
    
    if(!status){
        ParseError(line, "Syntactic error in Declaration Block.");
        return false;    
    }
       
  
    return true;
}






//DeclStmt ::= Ident {, Ident} : (Integer | Real | String)



bool DeclStmt(istream& in, int& line){
    bool status = false;
    LexItem tokVal = Parser::GetNextToken(in, line);
  

    if (defVar.find(tokVal.GetLexeme()) != defVar.end()) {
        cout << "Var Exists!" << endl;
        ParseError(line, "Var cant be redeclared");
        return false;
    }
    else{
        defVar.insert({tokVal.GetLexeme(), true});
    }

    LexItem commaVal = Parser::GetNextToken(in, line);

  

    if(commaVal == COMMA){
        status = DeclStmt(in, line);
       
    }
    else if(commaVal.GetToken() == IDENT){
        ParseError(line, "Unrecognized Input Pattern");
        return false;
    }
   
    else{

        if(commaVal != COLON){
            ParseError(line, "Missing Colon");
            return false;
        }

        LexItem t = Parser::GetNextToken(in, line);

       

        if(t.GetLexeme() != "REAL" && t.GetLexeme() != "INTEGER" && t.GetLexeme() != "STRING"){
            ParseError(line, "Incorrect Declaration Type.");
            return false;
        }
         LexItem semiVal = Parser::GetNextToken(in, line);
       
        if(semiVal != SEMICOL){
            ParseError(line, "Missing semicolon.");
            return false;
        }
        LexItem identVal = Parser::GetNextToken(in, line);
        
        if(identVal == IDENT){
        Parser::PushBackToken(identVal);
        status = DeclStmt(in,line);}
        else{
            Parser::PushBackToken(identVal);
            return true;
        }
        

        return status;
    }

    return status;
}









//ProgBody ::= BEGIN {Stmt;} END
bool ProgBody(istream& in, int& line){
     bool status = false;
    

    status = Stmt(in,line);
    
    if(!status){
        ParseError(line, "Syntactic error in Program Body.");
        return false;    
    }
    
    LexItem semiVal = Parser::GetNextToken(in, line);
        if(semiVal != SEMICOL){
            ParseError(line-1, "Missing semicolon in Statement");
            return false;
        }
        LexItem endVal = Parser::GetNextToken(in, line);
        
        if(endVal != END){
            
        Parser::PushBackToken(endVal);
        status = ProgBody(in,line);
        }
    
        else{
            return true;
        }

   
     
    
    return status;        
    
}
        




//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
    //cout<<"stmt start "<<t<<endl;
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		//cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
            //cout<<"back frm test 5"<<t<<endl;
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;
		
	case FOR:
		status = ForStmt(in, line);
		
		break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return ex;
}

//IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]

bool IfStmt(istream& in, int& line) {
	bool exVal=false ;
	LexItem t;

	if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	exVal = LogicExpr(in, line);
	if( !exVal ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != RPAREN ) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != THEN ) {

		ParseError(line, "Missing THEN");
		return false;
	}

    exVal = Stmt(in, line);
    if( !exVal ) {
		ParseError(line, "Missing  statement after Then");
		return false;
	}
    
    if((t=Parser::GetNextToken(in, line)) != ELSE ) {
         exVal = Stmt(in,line);
		
		return true;
	}

    
    exVal = Stmt(in,line);
   
	return exVal;
}



//ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt

bool ForStmt(istream& in, int& line) {
    bool status = false;
   
    LexItem identVal = Parser::GetNextToken(in,line);
       
    
        if(identVal != IDENT){
            ParseError(line, "Missing assignment operator");
            return false;
        }
     LexItem asopVal = Parser::GetNextToken(in,line);

        if(asopVal != ASSOP){
            ParseError(line, "Missing assignment operator");
            return false;
        }
    LexItem iconstVal = Parser::GetNextToken(in,line);
    if(iconstVal != ICONST){
        ParseError(line, "Missing Iconst operator");
            return false;
        
    }
     LexItem nextVal = Parser::GetNextToken(in,line);
    if(nextVal!= TO && nextVal!= DOWNTO){
        ParseError(line, "Misiing to or downto.");
        return false;
    }
    LexItem secondiconst = Parser::GetNextToken(in,line);
    if(secondiconst!= ICONST){
        ParseError(line, "Misiing Iconst.");
        return false;
    }
    LexItem doVal = Parser::GetNextToken(in,line);
   
    if(doVal!= DO){
        ParseError(line, "Misiing do.");
        return false;
    }
    status = Stmt(in,line);
    
    
    return status;
    
}


//AssignStmt ::= Var := Expr
bool AssignStmt(istream& in, int& line) {

    bool varSt = false, status = false;
    LexItem t;

    varSt = Var( in, line);


    if (varSt){
        t = Parser::GetNextToken(in, line);
            

        if (t == ASSOP){ 
            status = Expr(in, line);
            if(!status) {
                ParseError(line, "Missing Expression in Assignment Statment");
                return status;
            }
            else{
                return status;
            }

        }
        else if(t.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            return false;
        }
        else {
            ParseError(line, "Missing Assignment Operator =");
            return false;
        }
    }
    else {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    return status;
         
}



//ExprList ::= Expr {, Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr ::= Term {(+|-) Term}

bool Expr(istream& in, int& line){

   LexItem val ;

    bool status = Term(in, line);


    if( !status ) {
        return false;
    }

    val = Parser::GetNextToken(in, line);
    if(val.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << val.GetLexeme() << ")" << endl;
        return false;
    }
    while ( val.GetToken()== PLUS || val.GetToken() == MINUS )
    {
        status = Term(in, line);
        if( !status )
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        val = Parser::GetNextToken(in, line);
        if(val.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << val.GetLexeme() << ")" << endl;
            return false;
        }


    }
 
    Parser::PushBackToken(val);
    return true;


}


//Term ::= SFactor {( * | / ) SFactor}

bool Term(istream& in, int& line) {

    bool status = SFactor(in, line);

    if( !status ) {
        return false;
    }

    LexItem val = Parser::GetNextToken(in, line);
    
    if(val.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << val.GetLexeme() << ")" << endl;
        return false;
    }
    
    
    while ( val.GetToken() == MULT || val.GetToken() == DIV )
    {
        status = SFactor(in, line);

        if( !status ) {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        val	= Parser::GetNextToken(in, line);
        if(val.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << val.GetLexeme() << ")" << endl;
            return false;
        }

    }
   
    Parser::PushBackToken(val);
    return true;
}

//SFactor ::= [(+ | -)] Factor

bool SFactor(istream& in, int& line) {
    LexItem val = Parser::GetNextToken(in, line);
   
    bool status;
    int signVal = 0;
    if(val.GetToken() == MINUS )
    {
        signVal = -1;
    }
    else if(val.GetToken() == PLUS)
    {
        signVal = 1;
    }
    else
        Parser::PushBackToken(val);

    status = Factor(in, line, signVal);
    return status;

}


//LogicExpr ::= Expr (= | > | <) Expr

bool LogicExpr(istream& in, int& line){

    bool status = Expr(in, line);
    LexItem val;

    if( !status ) {
        return false;
    }

    val = Parser::GetNextToken(in, line);
    if(val.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << val.GetLexeme() << ")" << endl;
        return false;
    }
    if ( val.GetToken() == GTHAN  || val.GetToken() == EQUAL || val.GetToken() == LTHAN)
    {
        status = Expr(in, line);
        if( !status )
        {
            ParseError(line, "Missing expression after relational operator");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(val);
    return true;


}


//Var ::= IDENT

bool Var(istream& in, int& line)
{
    string identVal;

    LexItem val = Parser::GetNextToken(in, line);

    if (val.GetToken() == IDENT){
        identVal = val.GetLexeme();
        if (!(defVar.find(identVal)->second))
        {
            ParseError(line, "Undeclared Variable");
            return false;

        }
        return true;
    }
    else if(val.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << val.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
}


//Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)

bool Factor(istream& in, int& line, int signVal){
    LexItem val = Parser::GetNextToken(in, line);
 

    if( val.GetToken() == IDENT ) {
        
        string lexemeVal = val.GetLexeme();
        if (!(defVar.find(lexemeVal)->second))
        {
            ParseError(line, "Undefined Variable");
            return false;
        }
        return true;
    }
    else if( val.GetToken() == ICONST ) {

        return true;
    }
    else if( val.GetToken() == SCONST ) {

        return true;
    }
    else if( val.GetToken() == RCONST ) {

        return true;
    }
    else if( val.GetToken() == LPAREN ) {
        bool status = Expr(in, line);
        if( !status ) {
            ParseError(line, "Missing expression after (");
            return false;
        }
        if( Parser::GetNextToken(in, line) == RPAREN )
            return status;

        ParseError(line, "Missing ) after expression");
        return false;
    }
    else if(val.GetToken() == ERR){
        
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << val.GetLexeme() << ")" << endl;
        return false;
    }

    ParseError(line, "Unrecognized input");
    return 0;

}

