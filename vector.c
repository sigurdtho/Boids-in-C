/* Author: Sigurd Thomassen <sigurd14@gmail.com> */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

/*
 * Creates a vector.
 */
vector_t *vector_create(double x, double y) {
	vector_t *vector = malloc(sizeof(vector_t));
	if (vector == NULL) {
		goto error;
	}

	vector->x = x;
	vector->y = y;

	return vector;
error:
	return NULL;
}

void print_vector(vector_t *a, char *c)
{
	printf("%s, x: %d, y: %d\n", c, (int)a->x, (int)a->y);
}

/*
 * Destroys a given vector.
 */
void vector_destroy(vector_t *vector) {
	free(vector);
}

/*
 * Vector addision.
 */
void addision(vector_t *a, vector_t *b) {
	a->x += b->x;
	a->y += b->y;
	print_vector(a, "a");
}

/*
 * Vector subtraction.
 */
void subtraction(vector_t *a, vector_t *b) {
	a->x -= b->x;
	a->y -= b->y;
}

void divide(vector_t *a, vector_t *b) {
	a->x /= b->x;
	a->y /= b->y;
}

/*
 * Divides the vector with a float.
 */
void division(vector_t *a, double b) {
	if (b == 0) {
		a->x = a->x;
		a->y = a->y;	 
	}
	else {
		a->x /= b;
		a->y /= b;
	}
}

/*
 * Multiplies the vector with a float.
 */
void multiplication(vector_t *a, double b) {
	a->x *= b;
	a->y *= b;
}

/*
 * Returns a new absolute value vector.
 */
vector_t *vabs(vector_t *vector) {
	vector_t *vector2 = vector_create(vector->x, vector->y);
	vector2->x = abs(vector2->x);
	vector2->y = abs(vector2->y);

	return vector2;
}

/*
 * Returns the magnitude of the given vector.
 */
double magnitude(vector_t *vector) {
	double magnitude;
	magnitude = sqrt(pow(vector->x, 2) + pow(vector->y,2));

	return magnitude;
}

/*
 * Returns a new normalized vector with magnitude 1 but the same direction.
 */
void normalized(vector_t *vector) {
	
	vector->x /= magnitude(vector);
	vector->y /= magnitude(vector);
}

/*
 * Returns a copy of the given vector.
 */
vector_t *copy(vector_t *vector) {
	vector_t *vector2 = vector_create(vector->x, vector->y);

	return vector2;
}