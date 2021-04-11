#include "wrapper.h"
#include <time.h>
#include <math.h>
#include <zconf.h>
#include "list.h"

#define NAME "/mastermailbox" //servern skapar denna direkt, så vi kan prata med den från början
int InterCom = 1; //Så vi slipper varningar om oändliga loops



//-------------------Global------------------/

pthread_t green;

//--------------------------------------------//



void planetNameGen(char* name){ //Skapar ett namn för planeten
    char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 5; i++)
    {
        name[i] = alphabet[random() % 25];
    }
}

void planetIDGen(char* iD){ //Egen planetID generering, startar med ett backslash /
    char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 5; i++)
    {
        if(i==0)
            iD[i]='/';
        else
            iD[i] = alphabet[random() % 25];
    }
}

void populatePlanet(planet_type* solar) //Populerar en planet med namn, id, och alla startvärden som massa etc
{
    planetNameGen(solar->name);
    planetIDGen(solar->pid);
    solar->mass = random()% 100000+1000; //(rand()% 10+1)*pow(10,rand() % 4 + 23)
    solar->life = (int)random() % 100000+10000; //ger ett liv på mellan 1-10 cykler
    solar->sx = random() % 700 + 50; //placerar den endast inom ramarna för grafiken
    solar->sy = random() % 500 + 50;
    solar->vx = (random()%10)*0.001; //hastighet på 1-10, bör sänkas
    solar->vy = (random()%10)*0.001;


}

void systemOne(planet_type* system){
    planetNameGen(system->name);
    planetIDGen(system->pid);
    system->mass=100000000;
    system->life=100000;
    system->sx=200;
    system->sy=300;
    system->vx=0;
    system->vy=0;
}

void systemTwo(planet_type* system){
    planetNameGen(system->name);
    planetIDGen(system->pid);
    system->life=100000;
    system->mass=1000;
    system->sx=300;
    system->sy=300;
    system->vx=0;
    system->vy=0.0085;
}

void serverConnect(void (*funk)(planet_type*)){     //funktion som argument

    //Definitions start

    mqd_t mq,mp;
    int success, stop = 0;
    ssize_t bytes_read;
    List planet = createEmptyList();

    //Definitions end

    planet = calloc(sizeof(planet_type),1);

    funk(planet);     //Någon utav planetfunktionerna körs här

    success = MQconnect(&mq,NAME); //connect till global mailbox
    success = MQwrite(mq,(void*)planet); //skickar vår planet
    usleep(1000);



    while(stop==0) //kör whileloopen tills den lyckas koppla upp sig mot den nya mailboxen servern skapade för planeten
    {
        usleep(1000);
        success = MQconnect(&mp,planet->pid);   //anslut till den nya mailboxen
        if(success==1)
            stop = 1;


    }
    while(InterCom) //Kör den här tills planeten terminerats
    {

        bytes_read = MQread(mp,(void*)&planet);

        if((int)bytes_read == sizeof(planet_type))
        {
            printf("Planet with the id: %s terminated with the reason: %s\n",planet->pid,planet->name);
            pthread_exit(NULL);
        }
        usleep(1000); //sover en stund så den inte grillar while-loopen varje tick
    }


}       //funktionspekare som argument


void menu(void){

    char replay= 'y';
    while(replay=='y') {

        printf("----------Planet menu----------\n");
        int choice;


        printf("\nConnected to server\n\n");
        printf("1: Generate preset planets\n");
        printf("2: Generate random planets\n");
        printf("3: Disconnect from server\n");

        fscanf(stdin, "%d", &choice);

        switch (choice) {

            case 1:
                pthread_create(&green, NULL, (void *) serverConnect, systemOne);
                printf("How many smaller planets would you like to add?");
                int numGen=0;
                scanf("%d",&numGen);
                for (int i = 0; i <numGen ; ++i) {

                    pthread_create(&green, NULL, (void *) serverConnect, systemTwo);
                    usleep(1000000);
                }


                break;
            case 2:
                printf("How many planets would you like to generate?\n");
                int num=0;
                scanf("%d",&num);
                for (int i = 0; i <num ; i++) {
                    pthread_create(&green, NULL, (void *) serverConnect, populatePlanet);
                    usleep(100000);
                }


                break;
            case 3:
                printf("Okay!\n");
                replay='n';
                break;

            default:
                printf("Please etner something between 1 and 3\n");
        }
    }
}

int main()
{
    srandom((unsigned int)time(NULL));      //kör den här för våra genereringsfunktioner
    menu();
    pthread_join(green,NULL);

    return 0;
}
