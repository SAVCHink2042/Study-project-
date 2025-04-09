
/*
Всё было готово к запуску, когда вдруг погас свети замигала красная лампочка. Вы моментально
связали это событие с запуском Вашей программы в сервере управления охраной, видимо Ложко
забыл отключить резервный источник питания или защитная система предполагалатакое вторжение.
Вы быстро сообразили, как подключиться к передающему контроллеру. Далее Вы загрузилитуда
головучервя и включили режим отладки, тем самым заморозили отправку сигналатревогии
получили доступ кконсоли. Обойдя пару десятков папок с файлами, Вы нашли необходимые файлы с
‘логамитревоги. Сигнал состоял из последовательности целых чисел. В инструкции на странице 501
Вы нашли схему декодирования сигнала, однако понять это было непросто. После нехитрых
‘манипуляций скриптами, Вы упростили задачу до следующей:
* имя файла имеетвид: tracking_signals_aa.txt, tracking_signals_ab.txt, tracking_signals_ac.txt;
* дляопределениятребуемого файла надо внём определить: уникаленли минимальный
элемент;
* исправленный сигнал надо вывести в файл tracking_signal.txt, для исправления сигнала надо:
переставить первые 4 элемента в конец файла, апоследние 3 элемента в начало.

Беглым взглядом посмотрев на список процессов, Вы увидели, что сторожевой демон активени
перезагрузит контроллер, что приведёт к отключению Вашего режима отладки, через90 минут. Это
приведётк отправке сигналатревоги в центр. Времениу мало - за дело!.*/

 #include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
 
int getSize(const char* name);
int getSize(const char* name) {
    int x, n = 0;
    FILE* f = fopen(name, "r");
    if (!f) {
        printf(" Файл не открывается");
        return -1; 
    }
    if(fscanf(f, "%d", &x)!=1) {
        printf("Файл пуст");
        return -1;
        }
    rewind(f);
    while (fscanf(f, "%d", &x) == 1)
        n++;
    fclose(f);
    return n;
}
 
int* readFile(const char* name, int size);
int* readFile(const char* name, int size) {
    int j;
    FILE* f;
    
    int *array;
    f = fopen(name, "r");
    if (f) {
        array = (int*)malloc(size * sizeof(int*));
        if(!array) {
            printf(" Память для массива не выделена");
            return 0;
          }
        for ( j = 0; j < size; j++) {
            fscanf(f, "%d", &(array[j]));
        }
        fclose(f);
    }
        return array;
}


int getmin( const char* name);
int getmin( const char* name) {
    int min, j, size;
     int *array;
    FILE* f;
    size=getSize(name);
    f = fopen(name, "r");
      if (f) {
        array = (int*)malloc(size * sizeof(int*));
        if(!array) {
            printf(" Память для массива не выделена");
            return 0;
          }
   
        for ( j = 0; j < size; j++) {
            fscanf(f, "%d", &(array[j]));
        }
        fclose(f);
    }
        min=array[0];
          for ( j = 0; j < size; j++) {
            if( array[j]<min) {
               min=array[j];
        }
      }
        free(array);
        return min;
}
bool isRightFile(const char* name, int mina, int minb, int minc);
bool isRightFile(const char* name, int mina, int minb, int minc) {
    int k;
     k = getmin( name);
      if (k!=minb && k!=minc) {
        return true;
    }
      if (k!=minc && k!=mina) {
        return true;
    }
      if (k!=minb && k!=mina) {
        return true;
    }
    return false;
}
        
void printFile(int* array, int size );
void printFile(int* array, int size) {
    int i;
    FILE* OUT = fopen("tracking_signal.txt", "w+");
     if (!OUT) {
        printf(" Файл OUT не открывается");
      
    }
    for (i = size-4; i < size ; i++)         {
             fprintf(OUT, "%d " , array[i]);
        }
     for (i = 3; i < size-4 ; i++) {
         fprintf(OUT, "%d " , array[i]);
        }
     for (i = 0; i < 3 ; i++)         {
         fprintf(OUT, "%d " , array[i]);
         }
    free(array);
    fclose(OUT);
}
bool isExecutedRequestOK(const char* name, int mina, int minb, int minc);
bool isExecutedRequestOK(const char* name, int mina, int minb, int minc) {
   int size;
   int *array;
   size = getSize(name);
   array = readFile(name, size);
      if (isRightFile(name, mina, minb, minc)) {
        printFile(array, size);
        return true;
    }
     free(array);
     return false;
}
int main(void) {
    int mina, minb, minc;
       mina=getmin("tracking_signals_aa.txt");
    minb=getmin("tracking_signals_ab.txt");
    minc=getmin("tracking_signals_ac.txt");
    if (isExecutedRequestOK("tracking_signals_aa.txt",  mina, minb, minc)) {
        printf("The program has been executed correctly. All right!\n");
          return 0;
    }
    if (isExecutedRequestOK("tracking_signals_ab.txt", mina, minb, minc)) {
        printf("The program has been executed correctly. All right!\n");
        return 0;
    }
    if (isExecutedRequestOK("tracking_signals_ac.txt",  mina, minb, minc)) {
        printf("The program has been executed correctly. All right!\n");
        return 0;
    }
    printf("ERROR: No valid file");
    return -1;
}
