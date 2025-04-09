/*Заменить всюду в тексте один набор символов на другой  */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
	
int main(void) {
    int  len1, len2, lenp, i, lent;
    char line[10000+1],  *p, s[29]="vary very far,", q[29]="tomorrow,";
    FILE * f, *out;
    f = fopen("ert.txt", "r");
    if(f==NULL) {
       return -1;
       } 
    out = fopen("out.txt", "w+");
    if(out==NULL) {  
       return -1;
       }
    len1=strlen(q);
    len2=strlen(s);
    while( fgets(line, 10000, f) != NULL ) {
       lent=strlen(line);
      
       p = strstr(line, q);
       if(p == NULL) {
          for ( i = 0; i < lent; i++) {
             fprintf(out, "%c", line[i] );
             }
          } 
       else { 
          lenp=strlen(p);
          if( len1>=len2) {
                for( i = 0; i<len2; i++) {
                   p[i]=s[i];
                 }
                for( i = len2; i < lenp-len1+len2 ; i++) { 
                    p[i]=p[i+len1-len2];
                 }
                    line[lent-len1+len2]='\0';
                for ( i = 0; i < lent-len1+len2; i++) {
                    fprintf(out, "%c", line[i] );
                    }
                }
             else { 
                for( i = lent + len2; i > 0; i--) {
                   p[i+len2-len1]=p[i];
                   }
                for ( i = 0; i<len2; i++  )  {
                   p[i]=s[i];
                   }
                for ( i = 0; i < lent+len2-len1; i++) {
                fprintf(out, "%c", line[i] );
               }
           }
       }
   }
   fclose(f);
   printf("Nadeys programma srabotala pravilno\n");
   return 1;
}
