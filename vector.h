/* Author: Sigurd Thomassen <sigurd14@gmail.com> */

struct vector {
    double x;
    double y;
};
typedef struct vector vector_t;

void print_vector(vector_t *a, char *c);

/*
 * Creates and returns a vector.
 */
vector_t *vector_create(double x, double y);

/*
 * Destroyes a given vector.
 */
void vector_destroy(vector_t *vector);

/*
 * Vector addision.
 */
void addision(vector_t *a, vector_t *b);

/*
 * Vector subtraction.
 */
void subtraction(vector_t *a, vector_t *b);

void divide(vector_t *a, vector_t *b);

/*
 * Divides the vector with a float.
 */
void division(vector_t *a, double b);

/*
 * Multiplies the vector with a float.
 */
void multiplication(vector_t *a, double b);

/*
 * Returns a new absolute value vector.
 */
vector_t *vabs(vector_t *vector);

/*
 * Returns the magnitude of the given vector.
 */
double magnitude(vector_t *vector);

/*
 * Returns a new normalized vector with magnitude 1 but the same direction.
 */
void normalized(vector_t *vector);

/*
 * Returns a copy of the given vector.
 */
vector_t *copy(vector_t *vector);
