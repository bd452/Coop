#ifndef CCLASS_H
#define CCLASS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef struct classFunction{
    char *name;
    void *implementation;
} classFunction;

typedef struct classProperty{
    char *name;
    void *value;
} classProperty;


typedef struct Class {
	struct Class *inherits;
	struct classFunction* functions;
	int functionsSize;
	struct classProperty* properties;
	int propertiesSize;

} Class;

typedef void* (^blockType)(void*);

void class_addFunction(Class *class, classFunction *function);
classFunction functionMake(char* name, void* implementation) ;

void class_addFunction(Class *class, classFunction *function);
classFunction functionMake(char* name, void* implementation) ;
Class makeClass(Class *parent, classFunction *functions, int numberOfFunctions, classProperty* properties, int numberOfProperties);

void* class_do(Class *class, char* functionName);


#endif