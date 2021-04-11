/* Länkad lista */

#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include "wrapper.h"



typedef int Data;

//Listan representeras av en nodpekare
typedef planet_type* List;

//Returnera en tom lista
List createEmptyList(void);

//Är listan tom?
//Returnerar 1 om listan är tom, annars 0
int isEmpty(List list);

//Lägg till nod först i listan
void addFirst(List *list,planet_type solar);

//lägg till nod sist i listan
void addLast(List *list,planet_type solar);

//Ta bort första noden i listan
void removeFirst(List *list);

//ta bort sista noden i listan
void removeLast(List *list);

//ta bort data ur listan (första förekomsten), returnera 0 om datat inte finns, annars 1
int removeElement(List *list,planet_type solar);

//Sök efter data i listan, returnera 1 om datat finns, annars 0.
int search(List list,planet_type solar);

//returnera hur många noder som finns i listan
int numberOfNodesInList(List list);

//töm listan /ta bort allt data (alla noder) ur listan
void clearList(List *list);

//Skriv ut listan
void printList(List list, FILE *textfile);






#endif