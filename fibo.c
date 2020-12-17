#include <stdio.h>

/*
行列
	a b
	c d
*/
typedef struct {
	int a, b, c, d;
} Matrix;

Matrix mul(Matrix A, Matrix B) {
	int a = A.a * B.a + A.b * B.c;
	int b = A.a * B.b + A.b * B.d;
	int c = A.c * B.a + A.d * B.c;
	int d = A.c * B.b + A.d * B.d;
	return (Matrix){a, b, c, d};
}

Matrix power(Matrix A, int n) {

	if (n == 0) {
		// 単位行列
		return (Matrix){1, 0, 0, 1};
	}

	if (n % 2 == 0) {
		// 偶数のときはA^(n/2) * A^(n/2)
		Matrix B = power(A, n/2);
		return mul(B, B);
	} else {
		// 奇数のときはA^(n-1) * A
		Matrix B = power(A, n-1);
		return mul(A, B);
	}

}

void print_matrix(Matrix A) {
	printf("%d %d\n%d %d", A.a, A.b, A.c, A.d);
}

int fibo(int n) {
	if (n == 0)
		return 1;

	Matrix A = (Matrix){1, 1, 1, 0};

	Matrix B = power(A, n-1);
	return B.a + B.b; // B.a * fibo(0) + B.b * fibo(1)
}

int main() {

	int N = 40;
	for (int i=0; i<=N; i++) {
		printf("fibo(%2d) = %d\n", i, fibo(i));
	}

	return 0;
}