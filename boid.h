#include "vector.h"

typedef struct boid boid_t;
struct boid {
    vector_t *velocity;
    vector_t *position;
    int x;
    int y;
};

void destroy_boid(boid_t *boid);
