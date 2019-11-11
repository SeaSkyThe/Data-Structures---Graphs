#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
static int SESSION_TRACKER;
int flag = 1;

char* print_time(){
    int size = 0;
    time_t t;
    char *buffer;

    t=time(NULL);  //hora atual do calendario

    char *timestr = asctime( localtime(&t) );
    timestr[strlen(timestr) - 1] = 0;    //removendo \n

    size = strlen(timestr)+ 1 + 2;
    buffer = (char*)malloc(size);

    memset(buffer, 0x0, size);
    snprintf(buffer, size, "[%s]", timestr);

    return buffer;
}

void log_print(char* filename, char *local, char *fmt,...){
    va_list         list;
    char            *p, *r;
    int             e;
    FILE *fp;
    fp = fopen (filename,"a+");

    if(fp == NULL){
        fp = fopen(filename, "w");
    }

    if(SESSION_TRACKER == 0){
        fprintf(fp,"\n\n%s ",print_time());
        fprintf(fp,"[%s]",filename);
        fprintf(fp,"[Local: %s] \n",local);
    }



    va_start( list, fmt );

    for (p = fmt ; *p ; p++){

        if ( *p != '%' ){//se for string simples
            fputc(*p,fp);
        }
        else{
            switch (*++p){
                /* string */
                case 's':
                {
                    r = va_arg(list, char *);
                    fprintf(fp,"%s", r);
                    continue;
                }

                /* integer */
                case 'd':
                {
                    e = va_arg( list, int );
                    fprintf(fp,"%d", e);
                    continue;
                }

                default:
                    fputc(*p, fp);
                }
        }

    }
    va_end(list);
    //fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}
