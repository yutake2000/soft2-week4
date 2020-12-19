#include <stdio.h>
#include <stdlib.h>

int main(void){
  float f[4];

  // 以下に適切な値を設定する
  f[0] = 1.143139122e+27f; // Hell
  f[1] = 6.659299403e+28f; // o,Wo
  f[2] = 1.099819698e-32f; // rld\n
  f[3] = 0; // \0
  
  // "Hello,World\n" と出力してほしい
  printf("%s",(char*)f);
  return 0;
}
