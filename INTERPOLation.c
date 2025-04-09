/*Траектория Хокинг торпеды не поддавалась точному вычислению, датчики зафиксировали её
координаты в определённые моменты времени. Бортовой компьютерный предсказатель ПГА-112
выдал предполагаемые положения торпеды в будущем. Однако, предсказатель не смог
рассчитать где будет торпеда в момент времени перехода на Ц-скорость. Осознав бесполезность
работы с предсказателем, Вы решили самостоятельно запрограммировать расчёт положения
торпеды в момент времени х. Вы объединили все данные с датчиков и из предсказателя в файлах
по координатам х, у, z. Каждый файл содержал n пар значений t1, х1, t2, х2, ..., {п, хп, разделённых
пробелами, а в начале файла лежало само число n. Выбрав первый файл - тоpпедо_х.txt для
экспериментов, вы начали программировать кусочно-линейную интерполяцию.
Ваш помощник Ложко переживал сильнее Вашего, и поэтому выражал постоянные сомнения в
алгоритме. Для проверки кода Вы решили генерировать тестовый файл с данными какой-нибудь
заранее известной функции, к примеру:

log(х)+sin(х)
Далее запускать расчёт и сравнивать полученный результат с истинным значением функции в
какой-либо координате, которую Вы будете задавать из аргументов командной строки.
Количество точек и шаг для генерации тестовых значений также будет задаваться с аргументов. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int get_size(const char* name);
int get_size(const char* name) {
    int x, n;
    FILE* f = fopen(name, "r");
    if (!f) {
        printf(" Файл не открывается");
        return -1; 
    }
    if(fscanf(f, "%d", &x)!=1) {
        printf("Файл пуст");
        return -1;
        }
    else {
        n=x;
    }
    fclose(f);
    return n;
}

double* filling_the_array_y(int n, const char* name);
double* filling_the_array_y(int n, const char* name) {
    int  i;
    double *y, x, z;
    FILE *f;
    f = fopen(name, "r");
    y = (double*)malloc(n * sizeof(double));
    if(!y) {
        printf(" Память для массива a не выделена");
        fclose(f);
        return 0;
    }
    fscanf(f, "%lf", &x);
    for ( i = 0; i < n; i++) {
        fscanf(f, "%lf%lf", &z, &x);
        y[i] = z; 
    }
    fclose(f);
    return y;
}

double* filling_the_array_x(int n, const char* name);
double* filling_the_array_x(int n, const char* name) {
    double z, y, * x;
    int i;
    FILE *f;
    f = fopen(name, "r");
    x = (double*)malloc(n* sizeof(double));
    if(!x) {
        printf(" Память для массива x не выделена");
        fclose(f);
        return 0;
    }
    fscanf(f, "%lf", &z);
    for (i = 0; i < n; i++) {
        fscanf(f, "%lf%lf", &y, &z);
        x[i] = z;
    }
    fclose(f);
    return x;
}



void interpol(double *a,  double *b , double *c, int n);
void interpol(double *a,  double *b , double *c, int n) {
    int i, t=0;
    double p, f;
    while( t<c[1] ) {
        if(t*c[2]+c[0] > a[n-1] || t*c[2]+c[0] < a[0]) {
            break;
        }
            for(i=0; i<n-1; i++) {
                if((c[0]+t*c[2])<a[i+1] && (c[0]+t*c[2])>a[i] ) {
                    p=(b[i]-b[i+1])/(a[i]-a[i+1])*(c[0]+t*c[2])+(b[i+1]*a[i]-b[i]*a[i+1])/(a[i]-a[i+1]);
                    f=sin(c[0]+t*c[2])+log(c[0]+t*c[2]);
                    printf(" №точки %d, Значение функции:%lf, Значение через интероляциb:%lf \n",  t+1, f, p );
                    break;
             }
         }
       t++;
    }
}

int main(int argc, char* argv[]) {     /*  ИНСТРУКЦИЯ: argv[1]- начальная точка    argc[2]-количество точек  argv[3]- шаг*/
    double *x, *y, *z;  
    int n, i;
    if(argc!=4) {
        printf("Недостаточно аргументов");
        return 1;
    }
    z=(double*)malloc(3* sizeof(double));
    for(i=0; i<3; i++) {
        z[i]= strtod( argv[i+1], &*argv );
    }
    n=get_size("torpedo_x.txt");
    x=filling_the_array_x(n,"torpedo_x.txt" );
    y=filling_the_array_y(n,"torpedo_x.txt" );
    interpol(x, y, z, n);    
    free(x);
    free(y);
    free(z);
    return 1;
}
