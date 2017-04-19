/****************** error.cxx *************************/
/* enthält eine Reihe von Fehlertexten, die über ihre
Nummer verwendet werden können und jederzeit erweitert
werden können. 

Zur Ausgabe der Fehlertexte wird die Funktion 

	error ( int nr) 

verwendet, der die Fehlernummer zu übergeben ist.
Die Funktion error führt ein exit durch. 

Alternativ kann die Funktion 

	errortext (char *) 

verwendet werden, die den angegebenen Text ausgibt


	Stand: Sommersemester 2017 

*****************************************************/

#ifndef GLOBAL_H

#include "global.h"

#endif


/***************** Fehlertexte **********************/


char *errtxt[] =
        {
/*0*/    (char *) "Undefined Syntaxerror ",
/*1*/    (char *) "'='erwartet anstelle von ':=' ",
/*2*/    (char *) "Integer-Konstante nach = erwartet ",
/*3*/    (char *) " = nach Idenitikator erwartet ",
/*4*/    (char *) "Nach const,var,procedure Idenifikator erwartet",
/*5*/    (char *) " ';' oder ',' fehlt ",
/*6*/    (char *) "Statement oder Deklaration erwartet ",
/*7*/    (char *) "Falsches Symbol nach Statement im Block",
/*8*/    (char *) "Wiederholung erwartet",
/*9*/    (char *) "Falsche Benutzung eines Symbols in Statement",
/*10*/   (char *) "Identifikator nicht deklariert",
/*11*/   (char *) "Keine Zuweisungen an Konstante oder Prozedurnamen zulässig",
/*12*/   (char *) "Wertzuweisungsoperator ist ':=' ",
/*13*/   (char *) "Nach call ist Identifikator erwartet ",
/*14*/   (char *) "Konstante oder Variable in call nicht erlaubt",
/*15*/   (char *) "then erwartet",
/*16*/   (char *) "end oder ';' erwartet ",
/*17*/   (char *) "do erwartet ",
/*18*/   (char *) "Falsches Symbol nach Statement",
/*19*/   (char *) "Vergleichsoperator erwartet",
/*20*/   (char *) "Prozedurname in Ausdruck nicht erlaubt",
/*21*/   (char *) " ')' fehlt ",
/*22*/   (char *) " Symbol nach Faktor nicht erlaubt",
/*23*/   (char *) "Ausdruck darf nicht mit diesem Symbol beginnen",
/*24*/   (char *) "Zahl zu groß ",
/*25*/   (char *) "Programm zu groß",
/*26*/   (char *) "Prozedurschachtelung zu tief",
/*27*/   (char *) "kein Faktor: Name oder Konstante oder ( E) ",
/*28*/   (char *) " Symboltabelle voll",
/*29*/   (char *) "Lexemfeld voll",
/*30*/   (char *) "Statement erwartet",
/*31*/   (char *) "Korrektes Programmende fehlt",
/*32*/   (char *) " unzulässiges Eingabezeichen (Scanner)",
/*33*/   (char *) "Nach PROGRAM noch Symbole in Eingabedatei",
/*34*/   (char *) "Identifikator doppelt deklariert",
/*35*/   (char *) "Doppelpunkt erwartet",
/*36*/   (char *) "Unzulässiger Typ",
/*37*/   (char *) "Falsche Eintragsart in Symboltabelle",
/*38*/   (char *) "Keine korrektes reelles Literal",
/*39*/   (char *) "real oder int erwartet",
/*40*/   (char *) " ; fehlt"

        };


/***************** Fehlerfunktion **********************/



void error(int nr)
/* Funktion gibt den der Nummer nr entsprechenden Fehlertext aus */
{
    ferr << "Zeile " << lineno << ": " << errtxt[nr];

    exit(1);
}


void errortext(char *text)
/* Funktion gibt den angegebenen  Fehlertext aus */
{
    ferr << "Zeile" << lineno << text;

    exit(1);
}


void warningtext(char *text)
/* Funktion gibt den angegebenen  Warnungstext aus */
{
    ferr << "Zeile" << lineno << text;

}

