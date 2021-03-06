#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <math.h>
#define SQ(x) (x*x)
void calc_energy(struct rgb_img* im, struct rgb_img** grad) {
int height = im->height, width = im->width, red, blue, green;
int rx, gx, bx, ry, gy, by;
create_img(grad, height, width);
for (int i = 0; i < height; i++) {
for (int j = 0; j < width; j++) {
// x gradients
int x_grad, y_grad;
if (j == 0 && j == width - 1) {
// 1 pixel wide
rx = ry = get_pixel(im, i, j, 0);
gx = gy = get_pixel(im, i, j, 1);
bx = by = get_pixel(im, i, j, 2);
}
else if (j == 0) {
rx = get_pixel(im, i, j + 1, 0) - get_pixel(im, i, width - 1, 0);
gx = get_pixel(im, i, j + 1, 1) - get_pixel(im, i, width - 1, 1);
bx = get_pixel(im, i, j + 1, 2) - get_pixel(im, i, width - 1, 2);
}
else if (j == width - 1) {
rx = get_pixel(im, i, 0, 0) - get_pixel(im, i, j - 1, 0);
gx = get_pixel(im, i, 0, 1) - get_pixel(im, i, j - 1, 1);
bx = get_pixel(im, i, 0, 2) - get_pixel(im, i, j - 1, 2);
}
else {
rx = get_pixel(im, i, j + 1, 0) - get_pixel(im, i, j - 1, 0);
gx = get_pixel(im, i, j + 1, 1) - get_pixel(im, i, j - 1, 1);
bx = get_pixel(im, i, j + 1, 2) - get_pixel(im, i, j - 1, 2);
}
// y gradient
if (i == 0 && i == width - 1) {
// 1 pixel wide
ry = rx;
gy = gx;
by = bx;
}
else if (i == 0) {
ry = get_pixel(im, i + 1, j, 0) - get_pixel(im, height - 1, j, 0);
gy = get_pixel(im, i + 1, j, 1) - get_pixel(im, height - 1, j, 1);
by = get_pixel(im, i + 1, j, 2) - get_pixel(im, height - 1, j, 2);
}
else if (i == height - 1) {
ry = get_pixel(im, 0, j, 0) - get_pixel(im, i - 1, j, 0);
gy = get_pixel(im, 0, j, 1) - get_pixel(im, i - 1, j, 1);
by = get_pixel(im, 0, j, 2) - get_pixel(im, i - 1, j, 2);
}
else {
ry = get_pixel(im, i + 1, j, 0) - get_pixel(im, i - 1, j, 0);
gy = get_pixel(im, i + 1, j, 1) - get_pixel(im, i - 1, j, 1);
by = get_pixel(im, i + 1, j, 2) - get_pixel(im, i - 1, j, 2);
}

x_grad = pow(rx, 2) + pow(gx, 2) + pow(bx, 2);
y_grad = pow(ry, 2) + pow(gy, 2) + pow(by, 2);

uint8_t energy = (uint8_t)(pow((double)(x_grad + y_grad), 0.5) / 10.0);
set_pixel(*grad, i, j, (int)energy, (int)energy, (int)energy);
}
}
}
/*
void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    *grad = (struct rbg_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = im->height;
    (*grad)->width = im->width;
    (*grad)->raster = (uint8_t *)malloc(3 * im->height * im->width);

    for (int j = 1; j < im->height - 1; j++){ //no corner pieces
        for (int i = 1; i < im->width - 1; i++){
            int rleft = get_pixel(im, j, i-1, 0);
            int gleft = get_pixel(im, j, i-1, 1);
            int bleft = get_pixel(im, j, i-1, 2);
            
            int rright = get_pixel(im, j, i+1, 0);
            int gright = get_pixel(im, j, i+1, 1);
            int bright = get_pixel(im, j, i+1, 2);
            
            int rup = get_pixel(im, j+1, i, 0);
            int gup = get_pixel(im, j+1, i, 1);
            int bup = get_pixel(im, j+1, i, 2);

            int rdown = get_pixel(im, j-1, i, 0);
            int gdown = get_pixel(im, j-1, i, 1);
            int bdown = get_pixel(im, j-1, i, 2);

            int delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
            int delta_y_sq = SQ((rup - rdown))  + SQ((gup - gdown)) + SQ((bup - bdown));
            uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);
            set_pixel(*grad, j, i, energy, energy, energy);
        }
    }
    //still need: first row, last row, first col, last col, four corners
    //first row: y = 0
    for (int x = 1; x < im->width - 1; x++){
        int rleft = get_pixel(im, 0, x-1, 0);
        int gleft = get_pixel(im, 0, x-1, 1);
        int bleft = get_pixel(im, 0, x-1, 2);

        int rright = get_pixel(im, 0, x+1, 0);
        int gright = get_pixel(im, 0, x+1, 1);
        int bright = get_pixel(im, 0, x+1, 2);

        int rup = get_pixel(im, 1, x, 0);
        int gup = get_pixel(im, 1, x, 1);
        int bup = get_pixel(im, 1, x, 2);

        int rdown = get_pixel(im, im->height - 1, x, 0);
        int gdown = get_pixel(im, im->height - 1, x, 1);
        int bdown = get_pixel(im, im->height - 1, x, 2);

        int delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
        int delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
        uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

        set_pixel(*grad, 0, x, energy, energy, energy);
    }

    //last row: y = im->height -1
    for (int x = 1; x < im->width - 1; x++){
        int rleft = get_pixel(im, im->height-1, x-1, 0);
        int gleft = get_pixel(im, im->height-1, x-1, 1);
        int bleft = get_pixel(im, im->height-1, x-1, 2);

        int rright = get_pixel(im, im->height-1, x+1, 0);
        int gright = get_pixel(im, im->height-1, x+1, 1);
        int bright = get_pixel(im, im->height-1, x+1, 2);

        int rup = get_pixel(im, im->height-2, x, 0);
        int gup = get_pixel(im, im->height-2, x, 1);
        int bup = get_pixel(im, im->height-2, x, 2);

        int rdown = get_pixel(im, 0, x, 0);
        int gdown = get_pixel(im, 0, x, 1);
        int bdown = get_pixel(im, 0, x, 2);

        int delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
        int delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
        uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

        set_pixel(*grad, im->height - 1, x, energy, energy, energy);
    }

    //first column: x = 0
    for (int y = 1; y < im->height - 1; y++){
        int rleft = get_pixel(im, y, im->width-1, 0);
        int gleft = get_pixel(im, y, im->width-1, 1);
        int bleft = get_pixel(im, y, im->width-1, 2);

        int rright = get_pixel(im, y, 1, 0);
        int gright = get_pixel(im, y, 1, 1);
        int bright = get_pixel(im, y, 1, 2);

        int rup = get_pixel(im, y+1, 0, 0);
        int gup = get_pixel(im, y+1, 0, 1);
        int bup = get_pixel(im, y+1, 0, 2);

        int rdown = get_pixel(im, y-1, 0, 0);
        int gdown = get_pixel(im, y-1, 0, 1);
        int bdown = get_pixel(im, y-1, 0, 2);

        int delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
        int delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
        uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

        set_pixel(*grad, y, 0, energy, energy, energy);
    }

    //last column: x = im->width - 1
    for (int y = 1; y < im->height - 1; y++){
        int rleft = get_pixel(im, y, im->width-2, 0);
        int gleft = get_pixel(im, y, im->width-2, 1);
        int bleft = get_pixel(im, y, im->width-2, 2);

        int rright = get_pixel(im, y, 0, 0);
        int gright = get_pixel(im, y, 0, 1);
        int bright = get_pixel(im, y, 0, 2);

        int rup = get_pixel(im, y+1, im->width-1, 0);
        int gup = get_pixel(im, y+1, im->width-1, 1);
        int bup = get_pixel(im, y+1, im->width-1, 2);

        int rdown = get_pixel(im, y-1, im->width-1, 0);
        int gdown = get_pixel(im, y-1, im->width-1, 1);
        int bdown = get_pixel(im, y-1, im->width-1, 2);

        int delta_x_sq = SQ((rleft - rright))  + SQ((gleft - gright)) + SQ((bleft - bright));
        int delta_y_sq = SQ((rup - rdown))  + SQ((gup - gdown)) + SQ((bup - bdown));
        uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

        set_pixel(*grad, y, im->width-1, energy, energy, energy);
    }

    //4 corners: (0,0), (im->height-1,0), (0,im->width-1), (height-1, width-1)
    //(0,0)
    int rleft = get_pixel(im, 0, im->width-1, 0);
    int gleft = get_pixel(im, 0, im->width-1, 1);
    int bleft = get_pixel(im, 0, im->width-1, 2);

    int rright = get_pixel(im, 0, 1, 0);
    int gright = get_pixel(im, 0, 1, 1);
    int bright = get_pixel(im, 0, 1, 2);

    int rup = get_pixel(im, im->height-1, 0, 0);
    int gup = get_pixel(im, im->height-1, 0, 1);
    int bup = get_pixel(im, im->height-1, 0, 2);

    int rdown = get_pixel(im, 1, 0, 0);
    int gdown = get_pixel(im, 1, 0, 1);
    int bdown = get_pixel(im, 1, 0, 2);

    int delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
    int delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
    uint8_t energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

    set_pixel(*grad, 0, 0, energy, energy, energy);

    //(im->height-1,0)
    rleft = get_pixel(im, im->height-1, im->width-1, 0);
    gleft = get_pixel(im, im->height-1, im->width-1, 1);
    bleft = get_pixel(im, im->height-1, im->width-1, 2);

    rright = get_pixel(im, im->height-1, 1, 0);
    gright = get_pixel(im, im->height-1, 1, 1);
    bright = get_pixel(im, im->height-1, 1, 2);

    rup = get_pixel(im, im->height-2, 0, 0);
    gup = get_pixel(im, im->height-2, 0, 1);
    bup = get_pixel(im, im->height-2, 0, 2);

    rdown = get_pixel(im, 0, 0, 0);
    gdown = get_pixel(im, 0, 0, 1);
    bdown = get_pixel(im, 0, 0, 2);

    delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
    delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
    energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

    set_pixel(*grad, im->height-1, 0, energy, energy, energy);

    //(0,im->width-1)
    rleft = get_pixel(im, 0, im->width-2, 0);
    gleft = get_pixel(im, 0, im->width-2, 1);
    bleft = get_pixel(im, 0, im->width-2, 2);

    rright = get_pixel(im, 0, 0, 0);
    gright = get_pixel(im, 0, 0, 1);
    bright = get_pixel(im, 0, 0, 2);

    rup = get_pixel(im, im->height-1, im->width-1, 0);
    gup = get_pixel(im, im->height-1, im->width-1, 1);
    bup = get_pixel(im, im->height-1, im->width-1, 2);

    rdown = get_pixel(im, 1, im->width-1, 0);
    gdown = get_pixel(im, 1, im->width-1, 1);
    bdown = get_pixel(im, 1, im->width-1, 2);

    delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
    delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
    energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

    set_pixel(*grad, 0, im->width-1, energy, energy, energy);

    //(im->height-1, im->width-1)
    rleft = get_pixel(im, im->height-1, im->width-2, 0);
    gleft = get_pixel(im, im->height-1, im->width-2, 1);
    bleft = get_pixel(im, im->height-1, im->width-2, 2);

    rright = get_pixel(im, im->height-1, 0, 0);
    gright = get_pixel(im, im->height-1, 0, 1);
    bright = get_pixel(im, im->height-1, 0, 2);

    rup = get_pixel(im, im->height-2, im->width-1, 0);
    gup = get_pixel(im, im->height-2, im->width-1, 1);
    bup = get_pixel(im, im->height-2, im->width-1, 2);

    rdown = get_pixel(im, 0, im->width-1, 0);
    gdown = get_pixel(im, 0, im->width-1, 1);
    bdown = get_pixel(im, 0, im->width-1, 2);

    delta_x_sq = SQ((rleft - rright)) + SQ((gleft - gright)) + SQ((bleft - bright));
    delta_y_sq = SQ((rup - rdown)) + SQ((gup - gdown)) + SQ((bup - bdown));
    energy = (uint8_t)(sqrt(delta_x_sq + delta_y_sq) / 10);

    set_pixel(*grad, im->height-1, im->width-1, energy, energy, energy);
}
*/
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MIN3(a,b,c) (MIN(MIN(a,b), MIN(b,c)))

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    *best_arr = (double *)malloc(sizeof(double) * (grad->width+1) * (grad->height+1));
    for (int i = 0; i < grad->width; i++){
        (*best_arr)[i] = get_pixel(grad, 0, i, 0);
        //printf("%f ", (*best_arr)[i]);
    }
    //printf("\n");
    for (int y = 1; y < grad->height; y++){
        for (int x = 0; x < grad->width; x++){
            if (x != 0 && x != grad->width - 1){
                double a = (*best_arr)[(y-1)*(grad->width) + x-1];
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                double c = (*best_arr)[(y-1)*(grad->width) + x+1];
                (*best_arr)[y*grad->width + x] = MIN3((a), (b), (c)) + get_pixel(grad, y, x, 0);
            }
            else if (x == 0){
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                double c = (*best_arr)[(y-1)*(grad->width) + x+1];
                (*best_arr)[y*grad->width + x] = MIN((b), (c)) + get_pixel(grad, y, x, 0);
            }
            else if (x == grad->width - 1){
                double a = (*best_arr)[(y-1)*(grad->width) + x-1];
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                (*best_arr)[y*grad->width + x] = MIN((a), (b)) + get_pixel(grad, y, x, 0);
            }

            //printf("%f ", (*best_arr)[y*grad->width+x]);
        }
        //printf("\n");
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int *)malloc(sizeof(int) * height);
    double cur_min = best[height * width-1];
    int cur_min_ind = width-1;
    for (int x = 0; x < width; x++){
        if (best[(height-1)*width + x] < cur_min) {
            cur_min = best[(height-1)*width + x];
            cur_min_ind = x;
        }
    }
    (*path)[height-1] = cur_min_ind;
    for (int y = height - 2; y >= 0; y--){
        if ((*path)[y+1] != 0 && (*path)[y+1] != width-1){
            if (MIN3(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]-1])
                {(*path)[y] = (*path)[y+1]-1;}
            else if (MIN3(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]])
                {(*path)[y] = (*path)[y+1];}
            else (*path)[y] = (*path)[y+1]+1;
        }
        else if ((*path)[y+1] == 0) {
            if (MIN(best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]])
                {(*path)[y] = (*path)[y+1];}
            else (*path)[y] = (*path)[y+1]+1;
        }
        else if ((*path)[y+1] == width-1) {
            if (MIN(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]]) == best[y*width+(*path)[y+1]-1])
                {(*path)[y] = (*path)[y+1]-1;}
            else (*path)[y] = (*path)[y+1];
        }
    }
    //print test
    /*
    for (int i = 0; i < height; i++){
        printf("%d ", (*path)[i]);
    }
    */
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    *dest = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*dest)->height = src->height;
    (*dest)->width = src->width - 1;
    (*dest)->raster = (uint8_t *)malloc(3 * (*dest)->height * (*dest)->width);

    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            if (x < path[y]) {
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x, r, g, b);
            }
            else if (x > path[y]) {
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x-1, r, g, b);
            }
        }
    }
}