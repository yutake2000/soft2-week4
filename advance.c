/*

  焼きなまし法 + 2-opt法

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h> // strtol のエラー判定用
#include <time.h>

// 町の構造体（今回は2次元座標）を定義
typedef struct
{
  int x;
  int y;
} City;

// 描画用
typedef struct
{
  int width;
  int height;
  char **dot;
} Map;

typedef struct {
  int *route;
  double dist;
} Answer;

// 整数最大値をとる関数
int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

// プロトタイプ宣言
// draw_line: 町の間を線で結ぶ
// draw_route: routeでの巡回順を元に移動経路を線で結ぶ
// plot_cities: 描画する
// distance: 2地点間の距離を計算
// solve(): TSPをといて距離を返す/ 引数route に巡回順を格納

void draw_line(Map map, City a, City b);
void draw_route(Map map, City *city, int n, const int *route);
void plot_cities(FILE* fp, Map map, City *city, int n, const int *route);
double distance(City a, City b);
double solve(const City *city, int n, int *route);
Map init_map(const int width, const int height);
void free_map_dot(Map m);
City *load_cities(const char* filename,int *n);

Map init_map(const int width, const int height)
{
  char **dot = (char**) malloc(width * sizeof(char*));
  char *tmp = (char*)malloc(width*height*sizeof(char));
  for (int i = 0 ; i < width ; i++)
    dot[i] = tmp + i * height;
  return (Map){.width = width, .height = height, .dot = dot};
}
void free_map_dot(Map m)
{
  free(m.dot[0]);
  free(m.dot);
}

City *load_cities(const char *filename, int *n)
{
  City *city;
  FILE *fp;
  if ((fp=fopen(filename,"rb")) == NULL){
    fprintf(stderr, "%s: cannot open file.\n",filename);
    exit(1);
  }
  fread(n,sizeof(int),1,fp);
  city = (City*)malloc(sizeof(City) * *n);
  for (int i = 0 ; i < *n ; i++){
    fread(&city[i].x, sizeof(int), 1, fp);
    fread(&city[i].y, sizeof(int), 1, fp);
    //printf("(x, y) = (%d, %d)\n", city[i].x, city[i].y);
  }
  fclose(fp);
  return city;
}
int main(int argc, char**argv)
{
  // const による定数定義
  const int width = 70;
  const int height = 40;
  const int max_cities = 100;

  Map map = init_map(width, height);
  
  FILE *fp = stdout; // とりあえず描画先は標準出力としておく
  if (argc != 2){
    fprintf(stderr, "Usage: %s <city file>\n", argv[0]);
    exit(1);
  }
  int n;
  City *city = load_cities(argv[1],&n);
  assert( n > 1 && n <= max_cities); // さすがに都市数100は厳しいので

  // 町の初期配置を表示
  plot_cities(fp, map, city, n, NULL);
  sleep(1);

  // 訪れる順序を記録する配列を設定
  int *route = (int*)calloc(n, sizeof(int));
  // 訪れた町を記録するフラグ
  //int *visited = (int*)calloc(n, sizeof(int));

  const double d = solve(city,n,route);
  plot_cities(fp, map, city, n, route);
  printf("total distance = %f\n", d);
  for (int i = 0 ; i < n ; i++){
    printf("%d -> ", route[i]);
  }
  printf("0\n");

  // 動的確保した環境ではfreeをする
  free(route);
  //free(visited);
  free(city);
  
  return 0;
}

// 繋がっている都市間に線を引く
void draw_line(Map map, City a, City b)
{
  const int n = max(abs(a.x - b.x), abs(a.y - b.y));
  for (int i = 1 ; i <= n ; i++){
    const int x = a.x + i * (b.x - a.x) / n;
    const int y = a.y + i * (b.y - a.y) / n;
    if (map.dot[x][y] == ' ') map.dot[x][y] = '*';
  }
}

void draw_route(Map map, City *city, int n, const int *route)
{
  if (route == NULL) return;

  for (int i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = route[(i+1)%n];// n は 0に戻る必要あり
    draw_line(map, city[c0], city[c1]);
  }
}

void plot_cities(FILE *fp, Map map, City *city, int n, const int *route)
{
  fprintf(fp, "----------\n");

  memset(map.dot[0], ' ', map.width * map.height); 

  // 町のみ番号付きでプロットする
  for (int i = 0; i < n; i++) {
    char buf[100];
    sprintf(buf, "C_%d", i);
    for (int j = 0; j < strlen(buf); j++) {
      const int x = city[i].x + j;
      const int y = city[i].y;
      map.dot[x][y] = buf[j];
    }
  }

  draw_route(map, city, n, route);

  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      const char c = map.dot[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);
}

double distance(City a, City b)
{
  const double dx = a.x - b.x;
  const double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

void gen_random_route(int n, int *route) {

  // 初期化
  for (int i = 0 ; i < n ; i++){
    route[i] = i;
  }

  // n回シャッフル
  for (int i=0; i<n; i++) {
    int j1 = rand() % (n-1) + 1;
    int j2 = rand() % (n-1) + 1;
    int temp = route[j1];
    route[j1] = route[j2];
    route[j2] = temp;
  }

}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

double dist(const City *city, int *route, int i, int j) {
  return distance(city[route[i]], city[route[j]]);
}

Answer calc(const City *city, int n) {
  int route[n];
  gen_random_route(n, route);

  double co = -1e-6; //最大化なら正、最小化なら負。絶対値が小さいほど悪化方向へ進みやすい。Tが大きいほど小さくできる。
  int T = 1e6;

  for (int t=0; t<T; t++) {

    int i = rand() % (n-1) + 1;
    int j = rand() % (n-1) + 1;

    // 2-opt法
    
    if (i > j) {
      swap(&i, &j);
    }
    if (j - i <= 2) continue; // 確実に交差していない

    double diff = 0;
    diff -= dist(city, route, i, (i + 1) % n);
    diff -= dist(city, route, (j - 1 + n) % n, j);
    diff += dist(city, route, i, (j - 1 + n) % n);
    diff += dist(city, route, (i + 1) % n, j);

    if ((rand() / (double)RAND_MAX) < exp(co * diff * t)) {
      // i番目とj番目の間をすべて逆向きにする
      while (1) {
        i = (i + 1) % n;
        j = (j - 1 + n) % n;
        swap(&route[i], &route[j]);
        if (0 <= j - i && j - i <= 1) break;
      }
    }

  }

  double sum_d = 0;
  for (int i = 0 ; i < n ; i++){
    const int c0 = route[i];
    const int c1 = route[(i+1)%n]; // nは0に戻る
    sum_d += distance(city[c0],city[c1]);
  }

  //printf("sum:%lf\n", sum_d);

  int *ans_route = (int*)calloc(n, sizeof(int));
  memcpy(ans_route, route, sizeof(int) * n);
  return (Answer){.dist = sum_d, .route = ans_route};
}

double solve(const City *city, int n, int *route)
{

  srand((unsigned)time(NULL));
  Answer ans = (Answer){.dist = 1e15};
  int times = 10;
  for (int i=0; i<times; i++) {
    Answer result = calc(city, n);
    //printf("d:%lf\n", result.dist);
    if (result.dist < ans.dist) {
      free(ans.route);
      ans = result;
    }
  }
  memcpy(route, ans.route, sizeof(int) * n);

  return ans.dist;
}
