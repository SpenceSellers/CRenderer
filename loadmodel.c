#include "loadmodel.h"

#define MODEL_DIR "models/"

Model * read_line(FILE *file, Model *model){
    Line3D l ;
    fscanf(file, " %d %d %d %*s %d %d %d ",
	   &l.p1.x, &l.p1.y, &l.p1.z,
	   &l.p2.x, &l.p2.y, &l.p2.z
	);
    return Model_add(model, l);
}

Model * read_extrude(FILE *file, Model *model){
    Point3D d;
    fscanf(file, " %d %d %d ",
	   &d.x, &d.y, &d.z
	);
    return Model_extrude(model, d, 1);
}

Model * read_to(FILE *file, Model *model){
    Point3D last = model->line.p2;
    Point3D p;
    fscanf(file, " %d %d %d ", &p.x, &p.y, &p.z);
    Line3D l;
    l.p1 = last;
    l.p2 = p;
    model = Model_add(model, l);
    return model;
}

Model * read_scale(FILE *file, Model *model){
    double scale;
    fscanf(file, " %lf ", &scale);
    Model_scale(model, scale);
    return model;
}
Model * load_model(char *name){
    FILE *file;
    char fname_buffer[256];

    char *line;
    size_t line_len;
    ssize_t read;

    Model *model = NULL;
    
    strcpy(fname_buffer, MODEL_DIR);
    strcat(fname_buffer, name);
    
    file = fopen(fname_buffer, "r");
    if (file == NULL){
	printf("Failed to open %s \n", fname_buffer);
	return NULL;
    } else {
	printf("Successfully opened file %s \n", fname_buffer);
    }
    
    while (!feof(file)){
	char action_word[256];
	
	// Read the "action word"
	fscanf(file, "%s", action_word);
	
	if (strcmp("Line", action_word) == 0){
	    model = read_line(file, model);
	} else if (strcmp("Extrude", action_word) == 0){
	    model = read_extrude(file, model);
	} else if (strcmp("To", action_word) == 0){
	    model = read_to(file, model);
	} else if (strcmp("Scale", action_word) == 0){
	    model = read_scale(file, model);
	} else {
	    printf("Unrecognized word: %s \n", action_word);
	}
	
    }
    return model;
}

