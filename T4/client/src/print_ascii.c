#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(){
  setlocale(LC_ALL, "");  // Si esto no esta, no imprime
  printf("%lc\n", (wint_t)9818);  //wint_t es como extended char
  return 0;
}
