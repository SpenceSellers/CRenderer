#ifndef LOADMODEL_H
#define LOADMODEL_H

#include "objects.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
/*
  This loads a Model from a file.

  The file's name is determined by the name arugment.
  The full path to the model directory is added on for you.
  The model directory can be configured with a #define inside
  loadmodel.c.
 */
Model * load_model(char *name);

#endif

