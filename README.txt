	Dupa ce gigel a tinut mortis ca studentii de anul I sa realizeze
jocul denumit simbolic tetribit, acesta nu a luat in considerare ca 
se va plictisi repede de un joc care are doar 1 0 asa ca a trecut la 
nivelul urmator TETRIPIC. 
	Pentru realizarea temei am avut de lucrat cu fisiere de tip BMP
un capitol nou pentru noi dar un capitol care merita invatat. 
	Sa incepem:

	Pentru reazliarea jocului am folosit alocare dinamica structuri 
specifice fisierelor de tip BMP dar si diverse functii din care 
reamintim:	 
		1.bmp_color ** malloc_tetris(int latime, int lungime) functia intorce o matrice printr-un double pointer (fiecare pozitie din pointer pointeaza catre cate o linie a matricei).
		
		2.Functia free_tetris elibereaza memoria
		
		3.Functia scrie_tetris  genereaza o noua harta de dimensiuni date ,scrie linie cu linie pixelii imaginii, in ordine inversa , pune informatiile in headere
		
		4. Functia Piesa Scrie o piesa pe harta de tipul dat (eg: O Z S etc ...)

		5. Task1_piesa genereaza pentru task1 nerotite

		6. Rotatie_90 Roteste toata harta

		7. Task2_piesa genereaza fisierele pentru task2 roteste harta si o afiseaza de 3 ori.


In main primeste in linia de comanda numarul taskului verifica si apeleaza taskurile pentru fiecare piesa in parte .