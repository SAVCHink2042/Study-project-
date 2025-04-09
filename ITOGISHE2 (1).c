/*             УСЛОВИЕ:
В отличие от адмирала Квирка и его подразделения, Вы быстро сообразили что к чему и какую
тактику следует выбрать. Расчёт был верен и ловушка сработала. Андроид, как и планировалось,
обойдя по брахистохроне заминированное Тесло-дерево слева, задел невидимую правому лидару
растяжку. Заложенный под кустом микрозаряд детонировал и вывел из строя двигательную систему.
Вы с капитаном Ложко немедленно принялись за работу по внедрению в систему управления
роботом. Аккуратно отделив SAS контроллер, Ложко извлёк накопитель. В это время Вы развернули
переносную ЭВМ и принялись искать нужный переходник для подключения диска. Через 3 минуты
переходник NS305-AS был найден и диск был подключён.
– У нас не так много времени капитан! – с негодованием сказал Ложко.
– Ничего.. ещё успеем. Надо как можно скорее найти карту и загрузить в наш дрон. – бодро
ответили Вы.
– У меня всегда были проблемы с этими моделями андроидов, очень трудно запомнить, где
же он хранит эти карты.
– Давай по порядку: сначала грузим диск и ищём нужный раздел, далее в разделе находим
папку с картами, удаляем ненужный хлам, и то что останется проверим в ручном режиме.
– Хорошо!
Спустя 15 минут Ложко обнаружил нужную папку, но найти файл не смог. В папке и подпапках
находилось множество файлов с непонятными именами. На руках у Вас было старое имя файла с
картой map_5_*.avi, но в данной ситуации эта информация бесполезна. Вы помните, что в инструкции
говорилось о том, что карта должна быть сохранена в двух экземплярах, а следовательно это будет
пара одинаковых файлов. Ложко сообщил Вам, чтобы было бы хорошо в отдельную папку сложить
все подходящие файлы. Подходящим файлом является видео файл формата AVI, у которого есть
двойник в той же папке. В результирующую папку надо скопировать все такие файлы без двойников и
переименовать их в map_001.avi, map_002.avi, … Вы быстро прикинули схему алгоритма программы:

ищём все файлы (включая все подкаталоги) заданного каталога;

определяем тип файла, по первым байтам содержимого. Обрабатываем только AVI файлы.
Для этого потребуется обратиться к описанию формата AVI. Запрос в ТехноЯдро должен
помочь в этом;

для каждого найденного файла, ищем двойник в той же папке;

если двойник найден, то копируем исходный файл в папку result/.
Пока Ваш помощник Ложко настраивает дрон для отправки, Вы приступили к работе. Пары вечеров
должно хватить, подумали Вы, глядя на великолепное лазурное небо планеты. */

#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int copy(char tt[250],char d[30], int counter, char q[150]  ); /* имя файла,/ имя промежуточного или результирующего каталога/  номер файла для наименования*/
int copy(char tt[250],char d[30], int counter, char q[150] ){   // функция копирования файла в промежуточную или результирующую папку
    const char *p=tt;
    FILE *f, *g;
    char file_name[250], e[25], x ;
    f=fopen(p, "r");  
    if(!f) {
        printf(" исходник не открывается\n");
     }
     else {
        if(fscanf(f, "%c", &x)!=0) {
            rewind(f);
            if( counter>9 && counter<100) {
                sprintf(e, "map_0%d.avi", counter);
            }
            if( counter>99 ) {                            // ошибка в значении каунтера неправильно переданного в функцию copy
                sprintf(e, "map_%d.avi", counter);
            }
            if( counter>0 && counter< 10) {
                sprintf(e, "map_00%d.avi", counter);
            }
            strcpy( file_name, q);
            strcat( file_name, d);
            strcat( file_name, "/");
            strcat( file_name, e);
            g=fopen(file_name,  "a");
            if(!g) {
                printf("Файл для копирования в промежуточную папку не создан или не открывается\n");
                fclose(f);
                return 0;
            }
            while(fscanf(f, "%c", &x)==1) {
               fprintf(g, "%c", x);
            }
        fclose(g);
        }
    }
    fclose(f);
    return 1;   
} 
int result(char  a[10000], int number, char nm[250], char yy[20], char mm[20], char q[150]  );
int result(char  a[10000], int number, char nm[250], char yy[20], char mm[20], char q[150] ) {     
    FILE *g, *f;
    int counter;
    DIR *t;
    char ww[250], line[10010], bine[10010], cine[10010], x;
    struct dirent *dr;
    t=opendir(yy);
    dr=readdir(t); //11
    f=fopen(nm, "r");
    if(dr==NULL) {
        return 0;
    }
    else {
    while(dr!=NULL) {
        rewind(f);
        strcpy(ww, q);
        strcat(ww, yy);
        strcat(ww, "/");
        strcat(ww, dr->d_name);
        counter=1;
        if(fscanf(f, "%c", &x)!=0) {
           rewind(f);
           if(strcmp(ww, nm)!=0) {
               g=fopen(ww, "r");
               if(fgets(cine,10000, g)!=NULL) {
                  rewind(g);
                  if (strstr(a, ww)==NULL && strstr(a, nm)==NULL) {
                  while(fgets(line,10000, f)!=NULL && fgets(bine,10000, g)!=NULL) {
                      if( strcmp(line, bine)!=0) {
                          counter=0;
                          break;
                       }
                   }
                   if( counter==1) {
                      number++;
                      if(copy(nm, mm, number, q )) {
                          printf("копирование в резалт было выполнено\n");
                      }
                   break;
                   }
               }
           }
        }
     }
    dr=readdir(t);
    }
    }
    closedir(t);
    fclose(f);
    fclose(g);
    return number;
}

int sravnenie(  int counter, char q[150], char i[20], char mm[20]);
int sravnenie(  int counter, char q[150], char i[20], char mm[20]) {       // работа с промежуточной папкой и поиск одинаковых файлов
    int num=0, buf;
    DIR *t;
    char p[250],  a[10000];
    struct dirent *d;
    if(counter<1) {
        printf("Подходящие файлы в промежуточной папке отсутствуют\n");
        return 0;
    }
    t=opendir(i);
    d=readdir(t); 
    while(d!=NULL) {
       buf=num;
       strcpy(p, q);
       strcat(p, i);
       strcat(p, "/");
       strcat(p, d->d_name);
       num=result(a, num, p, i, mm, q);
       if(num> buf) {
           strcat(a, p);
           strcat(a, " ");
       }
       d=readdir(t);
     }
    closedir(t);
    return 1;
}

int tip_avi( const char * name);
int tip_avi( const char * name) {
    int i;
    FILE* f;
    char r[13], a[4]={'A', 'V','I', ' '};
    f=fopen(name, "r");
    if(!f ) {
        printf ("Файл не открывается\n");
        printf("%s", name);
        return 0;
    }
    if( fscanf(f, "%c", &r[0] )==0) {
       printf ("Файл пустой\n");
       fclose(f);
       return 0;
    }
    for( i=1; i< 13; i++) {
        fscanf(f, "%c", &r[i]);
    }
    if(a[0]==r[8] && a[1]==r[9] && a[2]==r[10] && a[3]==r[11] ) {
        fclose(f);
        return 1;
    } 
    fclose(f);
    return 0;
}

int wwk(  char u[500],  char q[150], char i[20], char dt[250], int counter);
int wwk(  char u[500],  char q[150], char i[20], char dt[250], int counter){         // закидывание нужных файлов в промежуточную папку
    DIR *t;
    char a[200], yy[2]=".", yyy[3]="..";
    struct dirent *d;
    struct stat  sb;
    t=opendir(u);
    d=readdir(t); 
    strcpy(a, dt);
    while(d!=NULL) {
       if(strcmp(d->d_name, yy)!=0 && strcmp(d->d_name, yyy)!=0) {
          strcpy(dt, a);
          strcat(dt, "/"); 
          strcat(dt, d->d_name); 
          stat(dt, &sb);
          if ((sb.st_mode & S_IFMT) == S_IFDIR) { 
              strcat(u, "/");
              strcat(u, d->d_name );
              printf("%s\n", u);
              counter+=wwk(u, q, i, dt, counter);                          //рекурсия для обхода подкаталогов
              }  
          else {
             if(tip_avi( dt)) {
                counter++;
                if(copy(dt, i, counter, q) ){
                   printf("копирование в промежуточную папку выполнено\n");
                }
                else {
                   printf("ошибка в tip_avi\n");
                }
             } 
         }
      }
      d=readdir(t);
   }
   closedir(t);
   return counter;    
}

void proverka(char q[150], char b[20]);
void proverka(char q[150], char b[20] ) {                                       // проверка на случай если в result попали пустые файлы, то их удаляем
    char a[250], line[10000];
    FILE *f;
    DIR *t;
    struct dirent *d;
    t=opendir(b);
    while((d=readdir(t))!=NULL) {
       strcpy(a, q);
       strcat(a, b);
       strcat(a, "/");
       strcat(a, d->d_name);
       f=fopen(a, "r"); 
       if(fgets(line, 10000, f)==NULL) {
          remove(a);
       }
    fclose(f);
    }
    closedir(t);
}
int main(void) {
    int counter=0;
    char a[500]="katalogi", q[150]="/home/student/Desktop/", b[20]="result", c[20]="next", d[250];
    strcpy(d, q);
    strcat(d, a);
    counter=wwk(a, q, c, d, counter);
    if(sravnenie( counter, q, c, b)) {
        printf("Программа выполнена");
    }
    proverka(q, b);
    return 1;
}
