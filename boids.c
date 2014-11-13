/* Author: Sigurd Thomassen <sigurd14@gmail.com> */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "SDL_image.h"
#include "list.h"

typedef enum {false, true} bool_t;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 768;

static void limit_velocity(vector_t *vec)
{
    double dvec = 0.0, vlim = 10.0;
    vector_t *absvec;

    absvec = vabs(vec);

    dvec = sqrt(pow(vec->x, 2) + pow(vec->y, 2));

    printf("%f\n", dvec);

    if (dvec > vlim) {
        divide(vec, absvec);
        multiplication(vec, vlim);
    }

    vector_destroy(absvec);
}

void logSDLError(char *error) {
    printf("%s error: %s\n", error, SDL_GetError());
}

SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren) {

    SDL_Texture *texture = IMG_LoadTexture(ren, file);
    if (texture == NULL) {
        logSDLError("LoadTexture");
    }

    return texture;
}

void renderScaledTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {

    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderScaledTexture(tex, ren, x, y, w, h);
}

void renderSpriteTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip) {

    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderSpriteTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip) {

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    if (clip != NULL) {
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderSpriteTexture(tex, ren, dst, clip);
}

/*typedef struct boid boid_t;*/
/*struct boid {*/
    /*vector_t *velocity;*/
    /*vector_t *position;*/
    /*int x;*/
    /*int y;*/
/*};*/

boid_t *create_boid() {
    boid_t *boid = (boid_t *)malloc(sizeof(boid_t));
    static int i = 0;

    srand(++i);
    //printf("%d\n", rand());

    boid->velocity = (vector_t *) vector_create(0, 0);
    boid->position = (vector_t *) vector_create(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
    boid->x = (boid->position)->x;
    boid->y = (boid->position)->y;

    return boid;
}

void destroy_boid(boid_t *boid) {
    free(boid->velocity);
    free(boid->position);

    free(boid);
}

list_t *initBoidList() {
    list_t *list = list_create();
    boid_t *boid;

    int numboids = 20;

    int i;

    for (i = 0 ; i < numboids ; i++) {
        boid = create_boid();
        list_addlast(list, boid);
    }

    return list;
}

void drawBoids(list_t *list, SDL_Texture *image, SDL_Renderer *renderer) {
    boid_t *tmp;
    list_iter_t *iter;
    iter = list_createiter(list);
    while (list_hasnext(iter)) {
        tmp = (boid_t *)list_next(iter);
        list_passnext(iter);
        renderTexture(image, renderer, tmp->x, tmp->y);	
}
}

vector_t *rule1(boid_t *boidj, list_t *boidlist) {
    vector_t *pc = (vector_t *)vector_create(0, 0);
    boid_t *boid;
    double count = 0;
    int distance = 90;

    printf("Inside rule 1\n");
    list_iter_t *iter = list_createiter(boidlist);
    while (list_hasnext(iter)) {
        boid = list_next(iter);
        printf("retreived boid from list\n");
        list_passnext(iter);
        printf("passed list further\n");
        printf("Iter is: %s\n", iter);
        if (boid != boidj) {
            if ((magnitude(boid->position) - magnitude(boidj->position)) < distance) {
                count++;
                subtraction(boid->position, boidj->position);
                printf("dones subtraction\n");
                addision(pc, boid->position);
                printf("done addition\n");
            }
        }
    }
    division(pc, count);
    division(pc, 100);
    return pc;
}

vector_t *rule2(boid_t *boidj, list_t *boidlist) {
    vector_t *c = (vector_t *)vector_create(0, 0);
    boid_t *boid;
    int boiddistance = 24;

    list_iter_t *iter = list_createiter(boidlist);
    while (list_hasnext(iter)) {
        boid = list_next(iter);
        list_passnext(iter);
        if (boid != boidj) {
            if ((magnitude(boid->position) - magnitude(boidj->position)) < boiddistance) {
                subtraction(boid->position, boidj->position);
                subtraction(c, boid->position);
            }
        }
    }

    return c;
}

vector_t *rule3(boid_t *boidj, list_t *boidlist) {
    vector_t *pv = (vector_t *)vector_create(0, 0);
    boid_t *boid;
    double count;
    int distance = 90;

    list_iter_t *iter = list_createiter(boidlist);
    while (list_hasnext(iter)) {
        boid = list_next(iter);
        list_passnext(iter);
        if (boid != boidj) {
            if ((magnitude(boid->position) - magnitude(boidj->position)) < distance) {
                count++;
                addision(pv, boid->velocity);
            }
        }
    }
    division(pv, count);
    subtraction(pv, boidj->velocity);
    division(pv, 2);
    return pv;
}

vector_t *rule4(boid_t *boidj, list_t *boidlist) {
    vector_t *vector = (vector_t *)vector_create((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));
    subtraction(vector, boidj->position);
    division(vector, 100);
    return vector;
}

void move(list_t *boidlist) {
    boid_t *boid;
    vector_t *v1, *v2, *v3, *v4;

    
    list_iter_t *iter = list_createiter(boidlist);
    printf("I created the iter\n");
    while (list_hasnext(iter)) {
        boid = list_next(iter);
        printf("I got the boid from iter\n");
        printf("Boid contents:\n");
        printf("Boid->Velocity: (%d, %d)\n", boid->velocity->x, boid->velocity->y);
        v1 = rule1(boid, boidlist);
        printf("Done rule1\n");
        multiplication(v1, 1.9);
        v2 = rule2(boid, boidlist);
        printf("Done rule2\n");
        multiplication(v2, 0.4);
        v3 = rule3(boid, boidlist);
        printf("Done rule3\n");
        multiplication(v3, 1.9);
        v4 = rule4(boid, boidlist);
        printf("Done rule4\n");
        multiplication(v4, 0.007);

        multiplication(boid->velocity, 5.5);
        addision(boid->velocity, v1);
        normalized(boid->velocity);
        addision(boid->velocity, v2);
        normalized(boid->velocity);
        addision(boid->velocity, v3);
        normalized(boid->velocity);
        addision(boid->velocity, v4);
        normalized(boid->velocity);

        multiplication(boid->velocity, 4);

        addision(boid->position, boid->velocity);
        static int counter;
        printf("Im here for the %d. time\n", counter);

        list_set(iter, boid);
        list_passnext(iter);
        counter++;


        /*if ((0 < boid->position->x && boid->position->x < SCREEN_WIDTH) && (0 < boid->position->y && boid->position->y < SCREEN_HEIGHT)) {
          inside = true
          }*/
    }
}

int main(int argc, char **argv)
{

    //Initiating SDL IMG
    if ((IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG != IMG_INIT_PNG)) {
        logSDLError("IMG_Init");
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError("SDL_INIT");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Lesson 5", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        logSDLError("CreateWindow");
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        logSDLError("CreateRenderer");
        return 3;
    }

    SDL_Texture *image1 = loadTexture("bird1.png", renderer);

    if (image1 == NULL) {
        return 4;
    }

    list_t *list = initBoidList();

    SDL_Event e;

    bool_t quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }
        move(list);

        SDL_RenderClear(renderer);
        drawBoids(list, image1, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(image1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0
        ;}
