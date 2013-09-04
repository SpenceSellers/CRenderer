#include "loadmodel.h"

Model * read_line(FILE *file, Model *model){
    Line3D l ;
    fscanf(file, " %d %d %d %*s %d %d %d ",
	   &l.p1.x, &l.p1.y, &l.p1.z,
	   &l.p2.x, &l.p2.y, &l.p2.z
	);
    return Model_add(model, l);
}

Model * load_model(char *name){
    FILE *file;
    char fname_buffer[256];

    char *line;
    size_t line_len;
    ssize_t read;

    Model *model = NULL;
    
    strcpy(fname_buffer, "models/");
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
	} else {
	    printf("Unrecognized word: %s \n", action_word);
	}
	
    }
    return model;
}

