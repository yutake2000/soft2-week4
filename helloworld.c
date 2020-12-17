#include <stdio.h>
#include <stdlib.h>

int main(void){
  float f[4];

  // 以下に適切な値を設定する
  f[0] = 1.14313905e+27f;
  f[1] = 6.659299403e+28f;
  f[2] = 1.099819698e-32f;
  f[3] = 0;
  
  // "Hello,World\n" と出力してほしい
  printf("%s",(char*)f);
  return 0;
}
