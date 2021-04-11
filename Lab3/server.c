#include <cairo.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "list.h"
//#include "wrapper.h"
#include <math.h>


#define NAME "/mastermailbox"

int interCom=1;    //int som kan stänga av planetSetUp
pthread_t i_am_thread;
double G = 6.67259E-11;
int dt = 50;
List head;
pthread_mutex_t mutex;



static void do_drawing(cairo_t *);
GtkWidget *window;
GtkWidget *darea;

void math(List head, List planetData) {

    double axtot=0, aytot=0;
    double r, diffx=0,diffy=0, a1,ax=0,ay=0;
    List temp;

    axtot = 0;      
    aytot = 0;
    temp = head;
    while(temp->next != NULL)
    {
        if(strcmp(temp->name,planetData->name)!=0)     //returnerar 0 om det är samma planet
        {
            //Beräkningar går här
            diffx = temp->sx - planetData->sx;
            diffy = temp->sy - planetData->sy;
            r = sqrt(pow(diffx,2)+pow(diffy,2)); //ekvation 2
            a1 = G*((temp->mass)/pow(r,2)); //ekvation 1
            ax = a1*diffx/r; //ekvation 3
            ay = a1*diffy/r; //ekvation 4
            axtot = axtot + ax;
            aytot = aytot + ay;
            temp = temp->next;
        } else
            temp=temp->next;
        
    }

    pthread_mutex_lock(&mutex);

    planetData->vx = planetData->vx + axtot*dt; //ekvation 5
    planetData->vy = planetData->vy + aytot*dt; //ekvation 5
    planetData->sx = planetData->sx + (planetData->vx)*dt; //Ekvation 6
    planetData->sy = planetData->sy + (planetData->vy)*dt; //Ekvation 6
    planetData->life = planetData->life -1;
    
    pthread_mutex_unlock(&mutex);
    usleep(10000);


}

void planet(void* plan){

    mqd_t mq;
    int success;
    List temp=(List)plan;       //typkonverterar

    success = MQcreate(&mq,temp->pid);

    pthread_mutex_lock(&mutex);

    addLast(&head,*temp);
    List findPlanet = head;

    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    while(findPlanet->next!=NULL) //här stegar pekaren igenom hela listan tills slutet
    {

        if(strcmp(findPlanet->name,temp->name)==0) //denna if-sats jämför två strängar, och går in om båda matchar
        {
            findPlanet = findPlanet->next;
            break;
        }
        else
            findPlanet = findPlanet->next;
    }
pthread_mutex_unlock(&mutex);
    


    while(interCom){

        while (findPlanet->life > 0 && findPlanet->sx < 800 && findPlanet->sy < 600 && findPlanet->sx > 0 && findPlanet->sy > 0){     //liv kvar och hålla sig innanför fönstret
            math(head,findPlanet);

        }

        if(findPlanet->life == 0)     //Behöver inget villkor för den här biten för den ska ändå bara inträffa efter att planeten försvunnit/dött
        {
            strcpy(findPlanet->name,"Planet died, life is 0");
        }
        else
        {
            strcpy(findPlanet->name,"Planet gone from system");
        }

        success = MQwrite(mq,(void*)findPlanet);

        pthread_mutex_lock(&mutex);
        mq_unlink(findPlanet->pid);            //unlinka mailboxen innan planeten tas bort 
        removeElement(&head,*findPlanet);     //ta bort död planet
        pthread_mutex_unlock(&mutex);
        
        pthread_exit(NULL);

    }
}


void planetSetUp(void* arg)               //Funktion som upprättar kontakt med mastermailbox
{
    mqd_t mq = *(mqd_t*)arg;
    List message = (List)calloc(sizeof(planet_type),1);
    ssize_t bytes_read;
    while(interCom){

        bytes_read = MQread(mq, (void*) &message);
        if(bytes_read==sizeof(planet_type)) {

            pthread_create(&i_am_thread,NULL,(void*)planet,(void*)message); //nytråd för varje planet

        }

        usleep(100000);
    }
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, //Draw event for cairo, will be triggered each time a draw event is executed
                              gpointer user_data)
{
    do_drawing(cr); //Launch the actual draw method
    return FALSE; //Return something
}

static void do_drawing(cairo_t *cr) //Do the drawing against the cairo surface area cr
{

    cairo_set_source_rgb(cr, 0, 0, 0); //planet color
    cairo_select_font_face(cr, "Purisa",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);

    cairo_move_to(cr, 20, 30);

    //------------------------------------------Insert planet drawings below-------------------------------------------
    
    List temp = head;


    while(temp != NULL)
    {
        cairo_arc(cr,temp->sx,temp->sy,sqrt(sqrt(sqrt(temp->mass)))*2,0,2*3.1415);
        cairo_fill(cr);
        temp = temp->next;
    }

    //------------------------------------------Insert planet drawings Above-------------------------------------------

}
GtkTickCallback on_frame_tick(GtkWidget * widget, GdkFrameClock * frame_clock, gpointer user_data) //Tick handler to update the frame
{
    gdk_frame_clock_begin_updating (frame_clock); //Update the frame clock
    gtk_widget_queue_draw(darea); //Queue a draw event
    gdk_frame_clock_end_updating (frame_clock); //Stop updating frame clock
}

int main(int argc, char *argv[])
{
    //----------------------------------------Variable declarations should be placed below---------------------------------
    mqd_t mq;
    int success;


    //----------------------------------------Variable declarations should be placed Above---------------------------------

    //GUI stuff, don't touch unless you know what you are doing, or if you talked to me
    gtk_init(&argc, &argv); //Initialize GTK environment
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //Create a new window which will serve as your top layer
    darea = gtk_drawing_area_new(); //Create draw area, which will be used under top layer window
    gtk_container_add(GTK_CONTAINER(window), darea); //add draw area to top layer window
    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL); //Connect callback function for the draw event of darea
    g_signal_connect(window, "destroy", //Destroy event, not implemented yet, altough not needed
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); //Set position of window
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); //Set size of window
    gtk_window_set_title(GTK_WINDOW(window), "Plantetlab"); //Title
    gtk_widget_show_all(window); //Show window
    gtk_widget_add_tick_callback(darea,(GtkTickCallback)on_frame_tick, NULL,(GDestroyNotify)1); //Add timer callback functionality for darea  //ifall saker skiter sig, ta bort GtkTickCallback ur argument 2, ifall mer skiter sig, ta bort GDestroyNotify
    //GUI stuff, don't touch unless you know what you are doing, or if you talked to me


    //-------------------------------Insert code for pthreads below------------------------------------------------

    pthread_mutex_init(&mutex,NULL);
    mq_unlink(NAME);
    success = MQcreate(&mq, NAME);
    head = createEmptyList();

    pthread_create(&i_am_thread, NULL, (void*)planetSetUp, (void*)&mq);    //vänta på clienter
    //-------------------------------Insert code for pthreads above------------------------------------------------

    gtk_main();//Call gtk_main which handles basic GUI functionality
    pthread_join(i_am_thread,NULL);

    return 0;
}