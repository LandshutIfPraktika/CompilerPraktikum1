
/****************** lexmain.cxx *********************/

/* Hauptprogramm zum Test der lexikalischen Analyse 

		Stand: Sommersemester 2017
		

		


		
************************************************/


#ifndef GLOBAL_H 
#include "global.h"
#endif 






int main()
{
	int lookahead = -1; 

	/* Initialisierungen der lexikalischen Analyse durchführen */
	initialize(); 

	/* 1. Symbol lesen  */ 
		
	lookahead = nextsymbol();
  
	/* Symbol ausgeben */ 
	fout << "		Tokentyp=    " <<lookahead; 
  
 /* Symbole aus Eingabedatei lesen bis Dateiende */ 
  while (lookahead != DONE)
	{ 	
		lookahead = nextsymbol();
		fout << "		Tokentyp=    " <<lookahead; 

 	}

	exit(0); 	/* Erfolg */
}


  

 



