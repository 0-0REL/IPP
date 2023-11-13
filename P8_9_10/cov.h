#include <string.h>
char bit[8][1];
char *invertir(char cadena[]) {
  int longitud = strlen(cadena);
  char temporal;
  for (int izquierda = 0, derecha = longitud - 1; izquierda < (longitud / 2);
       izquierda++, derecha--) {
    temporal = cadena[izquierda];
    cadena[izquierda] = cadena[derecha];
    cadena[derecha] = temporal;
  }
  return cadena;
}
char* convb(int numero){
    int x = 0;
        do{
            if(numero%2 == 1){
            numero--;
            numero/=2;
            strcpy(bit[x], "1");
            }
            else {
            numero/=2;
            strcpy(bit[x], "0");
            }
            x++;
        } while(x<=7);
        invertir(bit[0]);
        return bit[0];
}