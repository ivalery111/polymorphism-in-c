#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*fptrSet)(void *, uint32_t);
typedef uint32_t (*fptrGet)(void *);
typedef void (*fptrDisplay)();

typedef struct virt_funcs_s {
  /* Funtctions */
  fptrSet fptrSetX;
  fptrGet fptrGetX;
  fptrSet fptrSetY;
  fptrGet fptrGetY;

  fptrDisplay fptrDisplay;

} vFunctions;

typedef struct shape_s {
  vFunctions functions;

  /* Base Variables */
  uint32_t x;
  uint32_t y;

} Shape;
void shapeSetX(Shape *shape, uint32_t x) { shape->x = x; }
void shapeSetY(Shape *shape, uint32_t y) { shape->y = y; }
uint32_t shapeGetX(Shape *shape) { return shape->x; }
uint32_t shapeGetY(Shape *shape) { return shape->y; }
void shapeDisplay(Shape *shape) { printf("I'm Shape\n"); }

Shape *createShapeInstance() {
  Shape *shape = calloc(1, sizeof(Shape));
  assert(shape != NULL);

  shape->functions.fptrSetX = shapeSetX;
  shape->functions.fptrSetY = shapeSetY;
  shape->functions.fptrGetX = shapeGetX;
  shape->functions.fptrGetY = shapeGetY;
  shape->functions.fptrDisplay = shapeDisplay;

  shape->x = 10;
  shape->y = 15;

  return shape;
}

typedef struct rectangle_s {
  Shape base;

  uint32_t width;
  uint32_t height;

} Rectangle;
void rectangleSetX(Rectangle *rectangle, uint32_t x) { rectangle->base.x = x; }
void rectangleSetY(Rectangle *rectangle, uint32_t y) { rectangle->base.y = y; }
uint32_t rectangleGetX(Rectangle *rectangle) { return rectangle->base.x; }
uint32_t rectangleGetY(Rectangle *rectangle) { return rectangle->base.y; }
void rectangleDisplay(Rectangle *rectangle) { printf("I'm Rectangle\n"); }

Rectangle *createRectangleInstance() {
  Rectangle *rectangle = calloc(1, sizeof(Rectangle));
  assert(rectangle != NULL);

  rectangle->base.functions.fptrSetX = rectangleSetX;
  rectangle->base.functions.fptrSetY = rectangleSetY;
  rectangle->base.functions.fptrGetX = rectangleGetX;
  rectangle->base.functions.fptrGetY = rectangleGetY;
  rectangle->base.functions.fptrDisplay = rectangleDisplay;

  rectangle->base.x = 100;
  rectangle->base.y = 150;
  rectangle->width = 300;
  rectangle->height = 350;

  return rectangle;
}

int main(void) {
  Shape *shape = createShapeInstance();

  shape->functions.fptrSetX(shape, 90);
  shape->functions.fptrSetY(shape, 95);
  assert(shape->functions.fptrGetX(shape) == 90);
  assert(shape->functions.fptrGetY(shape) == 95);
  shape->functions.fptrDisplay();

  /* ----------------------- */

  Rectangle *rectangle = createRectangleInstance();
  assert(rectangle->base.functions.fptrGetX(rectangle) == 100);
  assert(rectangle->base.functions.fptrGetY(rectangle) == 150);

  rectangle->base.functions.fptrDisplay();

  /* ----------------------- */

  enum { SHAPES_SIZE = 2 };

  Shape *shapes[SHAPES_SIZE];

  shapes[0] = createShapeInstance();
  shapes[0]->functions.fptrSetX(shapes[0], 555);
  shapes[1] = (Shape *)createRectangleInstance();
  shapes[1]->functions.fptrSetX(shapes[1], 777);

  assert(shapes[0]->functions.fptrGetX(shapes[0]) == 555);
  assert(shapes[1]->functions.fptrGetX(shapes[1]) == 777);

  printf("Shapes:\n");
  for (size_t i = 0; i < SHAPES_SIZE; i++) {
    shapes[i]->functions.fptrDisplay();
    printf("x = %u\n", shapes[i]->functions.fptrGetX(shapes[i]));
  }

  return 0;
}
