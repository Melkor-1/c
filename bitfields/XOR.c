#include <stdio.h>
#include <stdlib.h>

void flip(int *x, int *y); 
int max(int x, int y); 

int main(void) 
{
	int x = 999;
	int y = -302;
	
	printf("Initial values: x = %d, y = %d\n", x, y);

	/* XOR returns 1 only if both bits are not the same 
	*  XOR-ing a bit with itself results in 0
	*  XOR-ing a bit with the same bit, returns 0
	*  XOR-ing the returned bit with the same bit, returns the original bit
	*  If we take any set of bits, and XOR them with any other set of bits, and
	*  get a perhaps intelligible set of bits, XOR-ing this with the same other set
	*  of bits, result in the original set of bits we started with.
	*/

	/* To swap two numbers.
	*  Let's take two numbers a and b, storing them in x and y. Then: 
	*/
	x = x ^ y;	 /* x == a XOR b */
	y = y ^ x;	 /* y == b XOR (a XOR b) ---> a */
	x = x ^ y;   /* x == (a XOR b) XOR a ---> b */

	printf("x: %d\ny: %d\n", x, y);

	int greatest = max(x, y);
	printf("greatest: %d\n", greatest);
	
	flip(&x, &y);
	printf("After flipping ON bits to OFF and vice versa:\n"
		   "x: %d, y: %d\n", x, y);

	return EXIT_SUCCESS;
}

/* Synopsis: TO find the max number using XOR */
int max(int x, int y) 
{
	return x ^ ((x ^ y) & -(x < y));
}

/* Synopsis: Flips all bits of a particular number */
void flip(int *x, int *y) 
{
	*x = *x ^ 0xFF;
	*y = *y ^ 0xFF;
}
