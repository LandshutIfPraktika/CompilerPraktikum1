
/**************** parser.cxx  Sommersemester 2017******************/



#ifndef GLOBAL_H

#include "global.h"

#endif


int lookahead;                    /* lookahead enthält nächsten EIngabetoken */


int exp();

int nextsymbol();

void lookfunc();

/******************  factor  **********************************************/
/* analysiert wird der korrekte Aufbau eines Faktors 

			

Schnittstelle: 

	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							



*/



int factor() {
    int kind;
    st_entry *found;        // Zeiger auf Eintrag in ST
    int factor_typ;

    if (tracesw)
        trace << "\n Zeile:" << lineno << "	Faktor";


    switch (lookahead)    // je nach nächstem Eingabesymbol in lookahead
    {
        case KLAUF:    /* Symbol '(' folgt --> (EXPRESSION) erwartet*/

            lookahead = nextsymbol();
            exp();
            if (lookahead == KLZU)
                // korrekt ; nächstes Symbol lesen --> Ende
                lookahead = nextsymbol();
            else
                error(27); // kein Faktor
            break;


        case INTNUM:
            /* Int-Zahl (INTNUMBER) gefunden --> okay */
            lookahead = nextsymbol();

            break;


        case REALNUM:        /* Real-Zahl (REALNUMBER) gefunden --> okay */
            lookahead = nextsymbol();

            break;


        case ID:    /* Identifikator (ID) gefunden  */
            /* Suche Identifikator in Symboltabelle ;
                angewandtes Auftreten -->
                Deklaration muss vorhanden sein
                und also Eintrag in ST */

            found = lookup(idname);


            if (found == NULL)
                /* nicht gefunden --> Fehler: Id nicht deklariert*/
                error(10);

            else    // Id in ST gefunden ; Art prüfen

            {
                kind = found->token;    // Art des ST-Eintrags

                switch (kind) {
                    case KONST:    // Konstantenname --> okay

                        break;

                    case INTIDENT:// einfache Variable, Typ int --> okay

                        break;

                    case REALIDENT:// einfache Variable, Typ real --> okay

                        break;


                    case PROC:    // Name einer Prozedur in
                        // Factor nicht erlaubt
                        error(20); // --> exit
                        // break;

                } // endswitch (kind)

                // nächstes Symbol lesen

                lookahead = nextsymbol();
            }    // endif


            break;

        default:    // kein korrekter Faktor
            error(27);
    }    // endswitch (lookahead)

    return (0);
}    // end factor









/******************  term ***************************************************/
/* analysiert wird der korrekte Aufbau eines Terms nach folgender Syntax:
			
			TERM	::=		FACTOR  { '*' FACTOR |  '/' FACTOR }*
			
Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Typ des Terms ist Funktionswert

*/


int term() {
    int ret;

    if (tracesw)
        trace << "\n Zeile:" << lineno << "Term:";


    ret = factor();
    // korrekter Factor

    while (lookahead == MULT || lookahead == DIV)
        // solange * oder / folgt, muss Factor kommen

    {// nächstes Symbol lesen
        lookahead = nextsymbol();
        ret = factor();


    }
    return (0);
}    // end term




/******************  exp ***************************************************/
/* analysiert wird der korrekte Aufbau eines Ausdrucks nach folgender Syntax:
			
			EXPRESSION	::=		TERM { '+' TERM |  '-' TERM}*
			
Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Funktionswert ist Typ des Ausdrucks
*/

int exp() {
    int typ_left, typ_right;
    if (tracesw)
        trace << "\n Zeile:" << lineno << "Ausdruck";

    typ_left = term();
    // korrekter Term

    while (lookahead == PLUS || lookahead == MINUS)
        // solange + oder - folgt, muss Term kommen

    {// nächstes Symbol lesen
        lookahead = nextsymbol();
        // Term prüfen
        typ_right = term();
        // nach korrektem Ende wurde nächstes Symbol gelesen

    }
    return (0);
}    // end exp




/******************  condition ***************************************************/
/* analysiert wird der korrekte Aufbau einer Bedingung nach folgender Syntax:
			
			CONDITION	::=		EXPRESSION  RELOP  EXPRESSION
			
Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/

int condition() {
    int typ_left, typ_right;


    if (tracesw)
        trace << "\n Zeile:" << lineno << "Condition";


    typ_left = exp();
    // korrekter Ausdruck
    // relationaler Operator muss folgen

    switch (lookahead) {
        case EQ:
        case NE:
        case LT:
        case LE:
        case GT:
        case GE:// nächstes Symbol lesen
            lookahead = nextsymbol();
            // Ausdruck muss folgen
            typ_right = exp();

            break;

        default: // kein relationaler Operator
            error(19);
    }
    if (typ_left != typ_right)
        errortext((char *) "Typen der Operanden nicht kompatibel");

    return (typ_left);
}  // end condition 




/****************** statement ***************************************************/
/* analysiert wird der korrekte Aufbau eines Statements nach folgender Syntax:
			
			STATEMENT 	::=		IDENT  ':=' EXPRESSION  
							|	call IDENT
							|	begin STATEMENT { ';' STATEMENT }* end
							|	if CONDITION then STATEMENT [else STATEMENT ]
							|	while CONDITION do STATEMENT



Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/



void statement() {
    st_entry *found;        // Zeiger auf ST-Eintrag
    int typ_left, typ_right;

    if (tracesw) trace << "\n Zeile:" << lineno << "Statement";


    // Überprüfung des aktuellen lex. Symbols

    // we have a procedure call
    if (lookahead == CALL) {
        if (tracesw) trace << "\n Zeile: " << lineno << " Procedure Call";

        lookfunc();
        found = lookup(idname);
        if (lookahead != ID || found == NULL) {
            error(13);
        }

        if (found->token != PROC) {
            error(14);
        }

        lookfunc();
        // valid proc call
    } else
        // we have couple of statements
    if (lookahead == BEGIN) {
        if (tracesw) trace << "\n Zeile: " << lineno << " Begin-End Block";

        lookfunc();

        statement();

        while (lookahead == SEMICOLON) {
            lookfunc();
            statement();
        }

        if (lookahead != END) {
            error(16);
        }

        lookfunc();
    } else
        // we have a condition
    if (lookahead == IF) {
        if (tracesw) trace << "\n Zeile: " << lineno << " IF-THAN";

        lookfunc();
        condition();

        if (lookahead != THEN) {
            error(15);
        }

        lookfunc();
        statement();

        if (lookahead == ELSE) {
            if (tracesw) trace << "\n Zeile: " << lineno << " ELSE";

            lookfunc();
            statement();
        }

    } else
        // we have a loop
    if (lookahead == WHILE) {
        if (tracesw) trace << "\n Zeile: " << lineno << " while loop";

        lookfunc();
        condition();

        if (lookahead != DO) {
            error(17);
        }
        lookfunc();
        statement();

    } else {
        // we have an assignment
        if (tracesw) trace << "\n Zeile: " << lineno << " assignment";

        if (lookahead != ID) {
            error(6);
        }

        found = lookup(idname);

        if (found == NULL) {
            error(10);
        }
        if (found->token == PROC || found->token == CONST) {
            error(11);
        }

        //todo check type of identifier
        lookfunc();
        if (lookahead != ASS) {
            error(12);
        }
        lookfunc();
        typ_right = exp();
    }

    return;    // end statement
}



/****************** procdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Prozedurdeklaration 
nach folgender Syntax:
			
			PROCDECL 	::=		{procedure IDENT ';' BLOCK ';' }*



Schnittstelle: 
	bei Aufruf :			erkannt wurde das Schlüsselwort procedure 
							nächstes Eingabesymbol befindet sich in lookahead 
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/



void procdecl() {
    st_entry *neu;          // Zeiger auf ST-Eintrag

    symtable *neusym;        // Zeiger auf Symboltabelle


    if (tracesw)
        trace << "\n Zeile:" << lineno << "Procdeklaration:";

    if (lookahead != ID) {
        error(4);
    }


    if (lookup_in_actsym(idname) != NULL) {
        error(34);
    }

    neu = insert(PROC);
    neusym = neu->subsym;


    lookfunc();
    if (lookahead != SEMICOLON) {
        error(40);
    }
    lookfunc();

    block(neusym);


    if (lookahead != SEMICOLON) {
        error(40);
    }
    lookfunc();


    if (lookahead == PROCEDURE) {
        lookfunc();
        procdecl();
    }

    return;   // end procdecl
}



/****************** vardecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablendeklaration 
nach folgender Syntax:
			
			VARDECL 	::=		  var IDENT ' : ' TYP  { ',' IDENT ' : ' TYP} *  ';' 			
			  
				
Schnittstelle: 
	bei Aufruf :			erkannt wurde das Schlüsselwort var 
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/



void vardecl() {
    st_entry *neu, *found;
    int oneMore = 1;

    if (tracesw)
        trace << "\n Zeile:" << lineno << "Variablendeklaration:";


    // nach var muss Identifikator folgen
    while (oneMore) {
        if (lookahead != ID) {
            error(4);
        }
        lookfunc();

        if (lookahead != COLON) {
            error(35);
        }
        lookfunc();

        if (lookahead == INT) {
            if (lookup_in_actsym(idname) != NULL) {
                error(34);
            }
            insert(INTIDENT);
        } else if (lookahead == REAL) {
            if (lookup_in_actsym(idname) != NULL) {
                error(34);
            }
            insert(REALIDENT);
        } else {
            error(41);
        }

        lookfunc();

        if (lookahead != KOMMA) {
            oneMore = 0;
        } else {
            lookfunc();
        }
    }


    if (lookahead != SEMICOLON) {
        //expected ;
        error(5);
    }

    lookfunc();

    return;    // ende vardecl

}








/****************** constdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablendeklaration 
nach folgender Syntax:
			
			CONSTDECL 	::=	 const IDENT '=' NUMBER {',' IDENT '=' NUMBER } * ';' 
			
			  
				
Schnittstelle: 
	bei Aufruf :			erkannt wurde das Schlüsselwort const 
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/




void constdecl() {
    st_entry *neu, *found;
    int oneMore = 1;

    if (tracesw)
        trace << "\n Zeile:" << lineno << "Konstantendeklaration:";

    // auf const muss IDENT folgen
    while (oneMore) {

        if (lookahead != ID) {
            // expected ident
            error(4);
        }
        lookfunc();

        if (lookahead != EQ) {
            // expected =
            if (lookahead == ASS) {
                error(1);
            }
            error(3);
        }
        lookfunc();

        if (lookahead != INTNUM) {
            // expected intnum
            error(2);
        }

        if (lookup_in_actsym(idname) != NULL) {
            error(34);
        }
        insert(KONST);
        lookfunc();

        if (lookahead != KOMMA) {
            oneMore = 0;
        } else {
            lookfunc();
        }
    }

    if (lookahead != SEMICOLON) {
        //expected ;
        error(5);
    }

    lookfunc();
    return;        // end constdecl
}




/****************** block ***************************************************/

/* analysiert wird der korrekte Aufbau eines Blockes nach folgender Syntax:

			
		BLOCK		::= 	[ CONSTDECL ]
							[ VARDECL ]
							  PROCDECL 
							STATEMENT 



Der Parameter neusym ist ein Zeiger auf die Symboltabelle des zu 
analysierenden Blockes 
===> 
		äussersten (globalen)  Block:		firstsym 
		bei Prozeduren:		Zeiger auf neu angelegte ST für Prozedur 
		
Zu Beginn muss der globale Zeiger actsym auf die neue ST gesetzt werden
Vor Verlassen muss actsym wieder auf den vorigen Wert gesetzt werden

				
Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead


*/




void block(symtable *neusym)

/* symtable * neusym :	Zeiger auf neue ST */


{
    symtable *lastsym;


    if (tracesw) {
        trace << "\n Zeile:" << lineno << "Block";
    }
    // actsym auf neue ST setzen
    lastsym = actsym;
    actsym = neusym;

    if (lookahead == CONST) {
        lookfunc();
        constdecl();
    }

    if (lookahead == VAR) {
        lookfunc();
        vardecl();
    }

    if (lookahead == PROCEDURE) {
        lookfunc();
        procdecl();
    }

    statement();

    // bei Blockende : Symboltabelle zurücksetzen
    // actsym = Zeiger auf vorherige ST

    actsym = lastsym;
    return;        // end block
}





/****************** program    ***************************************************/
/* analysiert wird der korrekte Aufbau eines Programmes nach folgender Syntax:
			

			PROGRAM		::=		BLOCK	'.' 
				
			

				
Schnittstelle: 
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/



void program() {

    if (tracesw)
        trace << "\n Zeile:" << lineno << "Programm";


    // globale Symboltabelle  anlegen (firstsym
    firstsym = create_newsym();

    // erstes Symbol lesen
    lookahead = nextsymbol();

    // Block muss folgen
    block(firstsym);

    //  nach Block muss '$' folgen
    if (lookahead == PROGEND)
        // nächstes Symbol lesen
        lookahead = nextsymbol();

    else
        // korrektes Programmende fehlt
        error(31);

    // Dateiende erreicht ?
    if (lookahead != DONE)
        error(33); // noch Symbole in Eingabedatei nach RPOGRAM

}    // end program

void lookfunc() {
    lookahead = nextsymbol();
}