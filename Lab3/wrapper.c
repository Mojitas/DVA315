#include "wrapper.h"

#define MAX_SIZE 1024

//Big note! If you think the arguments makes no sense, you are allowed to change them, as long as the basic functionality is kept
//In case you run in to blocking issues with reading from the queue, the option O_NONBLOCK is a hot tip

int MQcreate (mqd_t* mq, char* name)
{

    struct mq_attr attr; //hanterar attribut på mailkön
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    *mq = mq_open(name,O_CREAT|O_RDWR|O_NONBLOCK,0666,&attr); //skapar en kö med skriv och läsrättigheter

    if(*mq!=(mqd_t)-1)
    {
        return 1;
    }
    else
    {
        return *mq;
    }
    /* Creates a mailslot with the specified name. Uses mq as reference pointer, so that you can reach the handle from anywhere */
    /* Should be able to handle a messages of any size */
    /* Should return 1 on success and 0 on fail*/

}
int MQconnect (mqd_t* mq, char* name)
{

    *mq = mq_open(name,O_RDWR|O_NONBLOCK); //Öppnar kön för read/write

    if(*mq!=(mqd_t)-1)
    {
        return 1;
    }
    else
    {
        //printf("MQConnect failed for pid: %s: error: %d, %s\n",name,errno,strerror(errno));
        return 0;
    }
    /* Connects to an existing mailslot for writing Uses mq as reference pointer, so that you can reach the handle from anywhere*/
    /* Should return 1 on success and 0 on fail*/
}

ssize_t  MQread (mqd_t mq, void** buffer)
{

    ssize_t bytes_read;
    bytes_read = mq_receive(mq,*buffer,1024,NULL); //Läser in ett meddelande från kön in i buffer

    if(bytes_read == -1)
    {
        //printf("MQRead failed: error: %d, %s\n",errno,strerror(errno)); //printar ut hel errormeddelandet från mq_receive
    }
    else
    {

        return bytes_read;

    }
    /* Read a msg from a mailslot, return nr Uses mq as reference pointer, so that you can reach the handle from anywhere */
    /* should return number of bytes read*/
}

int MQwrite (mqd_t mq, void* sendBuffer)
{
    int success;


    success = mq_send(mq,sendBuffer, sizeof(planet_type) ,0); //skickar ett meddelande som lagrats i sendbuffer

    if(success == 0)
    {
        return 1;
    }
    else
    {
        //printf("MQWrite failed: error: %d, %s\n",errno,strerror(errno));
        return 0;
    }
    /* Write a msg to a mailslot, return nr Uses mq as reference pointer, so that you can reach the handle from anywhere*/
    /* should return number of bytes read*/
}

int MQclose(mqd_t* mq, char* name)
{
    int success;
    success = mq_close(*mq); //stänger kön
    mq_unlink(name);
    if(success == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    /* close a mailslot, returning whatever the service call returns Uses mq as reference pointer, so that you can
    reach the handle from anywhere*/
    /* Should return 1 on success and 0 on fail*/
}





