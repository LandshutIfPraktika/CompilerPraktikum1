/************************ lexan.cxx  Sommersemester 2017 ********************/
/*************** Scanner *******************************/


#ifndef GLOBAL_H

#include "global.h"

#endif


#define NORW   13        /* Zahl der reservierten Worte */

#define RWSYMLEN  15        /* Max. Länge reservierter Symbole */


int lineno;                    /* Zeilennummer */

int num;                    /* Wert einer int-Konstanten  */
double realnum;            /* Wert einer real-Konstanten*/

char idname[BSIZE];      /* Name eines Bezeichners ;
							wird vom Parser weiterverwendet */


char actchar;                /* gelesenes Zeichen */




/*  Um Bezeichner von reservierten Symbolene unterscheiden zu können,
wird eine Tabelle reservierte Worte verwendet (restable). 

Die Tabelle enthält Einträge für jedes Schlüsselwort, bestehend aus 
dem Schlüsselwort selbst und dem zugehörigen Tokentyp (Codierung vgl.global.h):

Bei Erkennen eines möglichen Bezeichners wird zuerst die Tabelle
der reservierten Symbole durchsucht (lookforres); 
wird ein Schlüsselwort gefunden, liefert lookforres den dem Schlüsselwort 
zugeordneten Tokentyp; sonst 0. Bei Ergebnis 0 liegt dann tatsächlich ein
Bezeichner vor. 
*/




/* Struktur eines Eintrags in der Tabelle reservierter Symbole */


struct ressw {
    char ressymbol[RWSYMLEN];            /* Symbol */
    int token;                            /* zugehöriger Tokentyp */
};


/* Tabelle reservierter Worte */


struct ressw
        restable[] =
        {{"const", CONST},
         "var", VAR,
         "procedure", PROCEDURE,
         "call", CALL,
         "begin", BEGIN,
         "end", END,
         "if", IF,
         "then", THEN,
         "else", ELSE,
         "while", WHILE,
         "do", DO,
         "int", INT,
         "real", REAL
        };


/* Suchen nach einem reservierten Symbol */

int lookforres(char *s)

/* Sucht in Tabelle reservierter Worte nach s und liefert zugehörigen Token
   falls gefunden,
   sonst 0 
*/

{
    struct ressw *ptr;

    for (ptr = restable; ptr < &restable[NORW]; ptr++)

        if (strcmp(ptr->ressymbol, s) == 0)
            /* Symbol gefunden */
            return (ptr->token);

    return (0);                /* Symbol nicht gefunden */
}


int issepperator(const char a) {
    switch (a) {
        case ' ':
        case '\t':
        case ',':
        case '=':
        case '<':
        case '>':
        case ':':
        case '+':
        case '-':
        case '*':
        case '/':
        case ';':
            return 1;
        default:
            return 0;
    }
}



/******************* Initialisieren des Scanners **********************/

/* wird aufgerufen von  initialize () aus init.cxx ; 
nimmt Vorbesetzungen der Variablen num, realnum,  idname und lineno vor; 


liest das erste Zeichen aus der Eingabe */



void initlexan() {
    num = NONE;
    realnum = -0.0;
    idname[0] = '\0';
    lineno = 1;
    fin.get(actchar);            /* Erstes Zeichen der Eingabe lesen */

}






/*******************************************************************************/
/********** Funktion nextsymbol ************************************************/
/*******************************************************************************/


/**** zentrale Funktion zum Lesen des nächsten lexikalischen Symbols ***************/
/*****  identifiziert nächstes lexikalisches Symbol der Eingabe **********/

/***** WICHTIG!!!!!   
		Bei Aufruf von nextsymbol muss  sich das nächste Eingabezeichen bereits in actchar befinden

*******/


/**    liefert Codierung des nächsten Symbols (token): 

		- Konstante:				token == INTNUM und Wert der Konstanten in Variable num
									token == REALNUM und Wert in realnum
		- Bezeichner:				token == ID  und Zeiger auf Name in idname 
		- Schlüsselwort:			token == Tokentyp des reservierten Symbols nach 
									Suche in restable 
		- Operatoren,Sonderzeichen :entsprechende Token 

**/




int nextsymbol() {
    int token;
    char lexbuf[BSIZE];        /* lokaler Puffer für Eingabezeichen */





    while (!fin.eof())            /* Eingabe-Dateiende nicht erreicht */
    {

        if (actchar == ' ' || actchar == '\t')
            /*Blank und Tab in Ausgabedatei kopieren und überlesen */
        {
            fout.put(actchar);
            fin.get(actchar);

        } else if (actchar == '\n' || actchar == '\r')
            /* Newline in Ausgabedatei kopieren, überlesen/entfernen, Zeilennummer erhöhen */
        {
            fout.put(actchar);
            fin.get(actchar);
            lineno++;
        } else

            /***** actchar ist Ziffer --> Literal erkennen  *****/
        if (isdigit(actchar)) {

            char zahl[BSIZE];        /* Puffer für Ziffern */
            int b = 0;                /* Zeichenzahl*/


            // as long as we are reading digits
            while (isdigit(actchar)) {
                fout.put(actchar);
                zahl[b] = actchar;
                b++;
                fin.get(actchar);
            }


            // real num
            if (actchar == '.') {
                fout.put(actchar);
                zahl[b] = actchar;
                b++;
                fin.get(actchar);

                // if no digit is following => error
                if (!isdigit(actchar)) {
                    error(38);
                }
                //second part of the real
                while (isdigit(actchar)) {
                    fout.put(actchar);
                    zahl[b] = actchar;
                    b++;
                    fin.get(actchar);
                }


                zahl[b] = '\0';
                realnum = atof(zahl);
                return REALNUM;
            }

            zahl[b] = '\0';
            num = atoi(zahl);
            return INTNUM;
        }   /***** actchar ist Buchstabe -->  Identifikatoren und Schlüsselwörter erkennen ****/
            /***  in lexbuf zwischenspeichern***********/
        else if (isalpha(actchar)) {

            char ident[BSIZE];
            int b = 0;                /* Zeichenzahl */
            int res = 0;

            /* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
                solange Buchstaben oder Ziffern folgen --> Identifikator */
            while (isalnum(actchar)) {
                fout.put(actchar);
                ident[b] = actchar;
                b++;
                fin.get(actchar);
            }

            ident[b] = '\0';

            if ((res = lookforres(ident))) {
                return res;
            }

            strcpy(idname, ident);
            return (ID);
        }
            /***** Sonderzeichen oder Operatoren erkennen ***************/
        else {
            fout.put(actchar);                /* Zeichen in Ausgabedatei kopieren */

            switch (actchar) {
                case '=':
                    fin.get(actchar);
                    return (EQ);

                case ':':
                    fin.get(actchar);
                    if (actchar == '=') {
                        fout.put(actchar);
                        fin.get(actchar);
                        return (ASS);
                    }
                    return (COLON);

                case ',':
                    fin.get(actchar);
                    return (KOMMA);
                case '<':
                    fin.get(actchar);
                    if (actchar == '=') {
                        fout.put(actchar);
                        fin.get(actchar);
                        return (LE);
                    }
                    return (LT);
                case '>':
                    fin.get(actchar);
                    if (actchar == '=') {
                        fout.put(actchar);
                        fin.get(actchar);
                        return (GE);
                    }
                    return (GT);
                case '+':
                    fin.get(actchar);
                    return (PLUS);
                case '-':
                    fin.get(actchar);
                    return (MINUS);
                case '*':
                    fin.get(actchar);
                    return (MULT);
                case '/':
                    fin.get(actchar);
                    return (DIV);
                case ';':
                    fin.get(actchar);
                    return (SEMICOLON);

                case '$':
                    fin.get(actchar);
                    return (PROGEND);

                case '(':
                    fin.get(actchar);
                    return (KLAUF);

                case ')':
                    fin.get(actchar);
                    return (KLZU);

                default:
                    error(32);


            } /* end-switch */
        } /* end-else */


    }/* end while */


    return (DONE);    /* EIngabe -Ende erreicht */

}


