#include "list.h"
#include <stdlib.h>
#include <assert.h>


/*Det är helt tillåtet att lägga till egna hjälpfunktioner men de befintliga funktionerna får inte ändras*/

/*Hjälpfunktion till add
  Allokerar minne för en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (elC:\Users\lindg\Documents\C-Programming\DOA-Lab1\DOA-Lab1.cbpler NULL) returneras.*/

static List createListNode(const planet_type solar)
{

    List temp = (List)malloc(sizeof(planet_type));
    if(temp==NULL)
    {
        printf("ERROR");
        return NULL;
    }
    else
    {

        temp->next = NULL;
        strcpy(temp->name,solar.name);
        strcpy(temp->pid,solar.pid);
        temp->mass = solar.mass;
        temp->sx = solar.sx;
        temp->sy = solar.sy;
        temp->vx = solar.vx;
        temp->vy = solar.vy;
        temp->life = solar.life;


        return temp;
    }
    //Glöm inte att testa så att allokeringen lyckades innan du initierar noden
    //Ersätt med rätt returvärde
}

/*Returnera en tom lista - funktionen är färdig*/
List createEmptyList(void)
{
    return NULL;
}


/*Är listan tom?
  Returnerar 1 om den är tom, annars 0*/
int isEmpty(const List list)
{
    if(list==NULL)
    {
        return 1; //ersätt med rätt returvärde
    }
    else
    {
        return 0;
    }
}

/*Lägg till nod först i listan*/
/*Postcondition: Det nya datat ligger först i listan (testa med assert)*/
void addFirst(List *list, planet_type solar)
{
    List temp;
    temp = createListNode(solar);
    if(temp!=NULL)
    {
        temp->next = *list;
        *list = temp;
    }
    else

    {
        printf("ERROR");
    }
    //Anropa createListNode för att skapa den nya noden
    //Glöm inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsätter
    //Tänk på att listan kan vara tom när en ny nod läggs till
}

/*Lägg till nod sist i listan
  Tips, när du hittat rätt plats kan du använda funktionen addFirst for att lägga till*/
void addLast(List *list, planet_type solar)
{

    List temp1;
    List temp2;
    temp1 = *list;
    if(*list==NULL)
    {
        addFirst(list, solar); //om listan är tom ska addfirst användas istället
    }
    else
    {
        while(temp1->next != NULL)
        {
            temp1 = temp1->next;
        }
        temp2 = createListNode(solar);
        if(temp2!=NULL)
        {
            temp2->next = temp1->next;
            temp1->next = temp2;



        }
    }
}

/*Ta bort första noden i listan
  Precondition: listan är inte tom (testa med assert)
  Noden ska länkas ur och minnet frigöras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
    assert(*list!=NULL);
    assert(list!=NULL);
    List temp;
    temp = *list;
    *list = temp->next;
    free(temp);
    //Glöm inte att frigöra minnet för den nod som länkas ur listan.
    //Tänk på att listans huvud efter bortlänkningen måste peka på den nod som nu är först.
}

/*Ta bort sista noden i listan
  Precondition: listan är inte tom (testa med assert)t*/
void removeLast(List *list)
{

    assert(*list!=NULL);
    assert(list!=NULL);
    List temp1;
    List temp2;
    temp1 = *list;
    temp2 = *list;
    if(temp1->next!=NULL)
    {
        while(temp1->next!=NULL)
        {
            temp1 = temp1->next;
            if(temp1->next!=NULL)
            {
                temp2 = temp2->next;
            }
        }

        temp2->next = NULL;
        free(temp1);
    }
    else
    {
        *list = NULL;
        free(temp1);
    }
    //Glöm inte att frigöra minnet for den nod som länkas ur listan.
    //Tänk på att den nod som nu är sist inte pekar någonstans, dess pekare måste nollställas
}

//returnerar 0 ifall det inte gick att ta bort, och 1 om det gick, dubletter hanters genom att den tar det första fallet av talet som hittas



/*Ta bort data ur listan (första förekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, när du hittar rätt nod kan du använda en av de ovanstående funktionerna för att ta bort noden*/
int removeElement(List *list, planet_type solar)
{
    List temp;
    List temp1;
    int result;
    temp1 = *list;
    temp = *list;
    result = search(*list, solar);
    if(result==0)
        return 0;
    else
    {
        if(strcmp(temp->name,solar.name)==0)
        {
            result = 1;
        }
        else
        {
            while(strcmp(temp->name,solar.name)!=0)
            {
                temp = temp->next;
                if(strcmp(temp->name,solar.name)==0)
                {
                    result = 1;
                }
            }
            while(temp1->next!=temp)
            {
                temp1 = temp1->next;
            }
        }
        if(result==1)
        {

            if(temp->next == NULL)
            {
                removeLast(&(*list));
            }
            else if(temp == *list)
            {
                removeFirst(&(*list));
            }


            else
            {

                temp1->next = temp->next;
                free(temp);

            }
            return result;
        }
    }
}


int search(const List list, planet_type solar) //letar om ett data finns i listan.
{
    List temp;
    temp = list;
    if(temp==NULL) {}
    else
    {
        while(temp!=NULL)
        {
            if(strcmp(temp->name,solar.name)==0)
            {
                return 1;
            }
            temp = temp->next;
        }
    }

    return 0; //Ersätt med rätt returvärde
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
    if(list==NULL)
    {
        return 0;
    }
    else
        return 1 + numberOfNodesInList(list->next);
}

//Skriver ut listan

void printList(const List list, FILE *textfile)
{

    if(list==NULL){}
    else
    {
        fprintf(textfile, "Id:%s s=(%f,%f) v=(%f,%f)\n",list->pid,list->sx,list->sy,list->vx,list->vy);

        printList(list->next, textfile);
    }
}

