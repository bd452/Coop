#ifndef CCLASS_H
#define CCLASS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define STRINGIFY2( x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define PASTE2( a, b) a##b
#define PASTE( a, b) PASTE2( a, b)

#define makeFunc(aname, aimp) (classFunction) {.name = #aname, .implementation = aimp}
#define $(classMember, functionName) class_do(&classMember, #functionName, 0, NULL)
#define $$(classMember, functionName, argsNumber, args) class_do(&classMember, #functionName, argsNumber, args)
//#define ∞(parent)  = makeClass(parent)
#define inheritsFrom(parent) = makeClass(parent)
#define newClass(name, parent) Class name = makeClass(parent)
#define func(classMember, name, imp) classFunction classMember ## _ ## name = makeFunction(#name, ^(void* args) imp); class_addFunction(&classMember, &classMember ## _ ## name);
#define msgSend(classMember, functionName) class_do(&classMember, #functionName)

typedef struct classFunction{
    char *name;
    void *implementation;
} classFunction;

typedef struct classProperty{
    char *name;
    void *value;
} classProperty;


typedef struct Class {
	char *name;
	struct Class *inherits;
	struct classFunction* functions;
	int functionsSize;
	struct classProperty* properties;
	int propertiesSize;

} Class;

typedef void* (^blockType)(void*);

// void class_addFunction(Class *class, classFunction *function);
// classFunction functionMake(char* name, void* implementation) ;

void class_addFunction(Class *class, classFunction *function);
classFunction makeFunction(char* name, void* implementation) ;
Class makeClass(Class *parent);

void* class_do(Class *class, char* functionName, int number, void *passingArgs);


#endif