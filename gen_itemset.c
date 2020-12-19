/*

	knapsack1.c で読み込むためのバイナリデータを作成する。

	./a.out <number> <seed> <filename>

	実行例:
	./a.out 10 0 itemset.dat

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // strtol, strtod でerror を補足したい

int load_int(const char *argvalue)
{
  long nl;
  char *e;
  errno = 0; // errno.h で定義されているグローバル変数を一旦初期化
  nl = strtol(argvalue,&e,10);
  if (errno == ERANGE){
    fprintf(stderr,"%s: %s\n",argvalue,strerror(errno));
    exit(1);
  }
  if (*e != '\0'){
    fprintf(stderr,"%s: an irregular character '%c' is detected.\n",argvalue,*e);
    exit(1);
  }
  return (int)nl;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s <number> <seed> <filename>\n", argv[0]);
		return 1;
	}

	int number = load_int(argv[1]);
	int seed = load_int(argv[2]);
	char *filename = argv[3];

	srand(seed);
	double value[number], weight[number];
	for (int i = 0 ; i < number ; i++) {
		value[i] = 0.1 * (rand() % 200);
		weight[i] = 0.1 * (rand() % 200 + 1);
		printf("v[%d] = %.1lf, w[%d] = %.1lf\n", i, value[i], i, weight[i]);
	}

	FILE *fp = fopen(filename, "wb");
	fwrite(&number, sizeof(int), 1, fp);
	fwrite(value, sizeof(double), number, fp);
	fwrite(weight, sizeof(double), number, fp);
	fclose(fp);

	return 0;
}