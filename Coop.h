#ifndef COOP_H
#define COOP_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define STRINGIFY2( x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define PASTE2( a, b) a##b
#define PASTE( a, b) PASTE2( a, b)

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define makeFunc(aname, aimp) (classFunction) {.name = #aname, .implementation = aimp}
#define $(classMember, functionName) class_do((void *)&classMember, #functionName, 0, NULL)
#define $$(classMember, functionName, argsNumber, args) class_do(&classMember, #functionName, argsNumber, args)
//#define ∞(parent)  = makeClass(parent)
#define inheritsFrom(parent) = makeClass(parent)
#define newClass(name, parent) Class name = makeClass(parent)
#define classFunc(classMember, name, imp) classFunction classMember ## _ ## name = makeClassFunction(#name, ^(Class* self, void* args) imp); class_addClassFunction(&classMember, &classMember ## _ ## name);
#define instanceFunc(classMember, name, imp) instanceFunction classMember ## _ ## name = makeInstanceFunction(#name, ^(obj self, void* args) imp); class_addInstanceFunction(&classMember, &classMember ## _ ## name);
#define msgSend(classMember, functionName) class_do(&classMember, #functionName)

#define logPrint(args,...) printf( COLOR_CYAN "§ - "  args COLOR_RESET "\n", ## __VA_ARGS__)
#define errorLog(args,...) printf( COLOR_MAGENTA "ERROR (%s,%s,%d): " args COLOR_RESET "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)




typedef struct classFunction{
    char *name;
    void *implementation;
} classFunction;

typedef struct instanceFunction{
    char *name;
    void *implementation;
} instanceFunction;

typedef struct classProperty{
    char *name;
    void *value;
} classProperty;

typedef void* imp;


//Object type
//1 == Class
//2 == Instance

typedef struct Class {
	int structType;
	char *name;
	struct Class *inherits;
	struct classFunction* functions;
	int functionsSize;
	struct instanceFunction* instanceFunctions;
	int instanceFunctionsSize;
	struct classProperty* properties;
	int propertiesSize;
	
} Class;

typedef struct CoopObject {
	int structType;
	Class frame;

} object;

typedef object *obj;

// typedef struct ClassInstance {
// 	char *name;
// 	struct Class *type;
// }

typedef void* (^blockType)(void*, void*);

// void class_addFunction(Class *class, classFunction *function);
// classFunction functionMake(char* name, void* implementation) ;

void class_addClassFunction(Class *class, classFunction *function);
void class_addInstanceFunction(Class *class, instanceFunction *function);
classFunction makeClassFunction(char* name, void* implementation) ;
instanceFunction makeInstanceFunction(char* name, void* implementation) ;
Class makeClass(Class *parent);
obj makeClassInstance(Class *parent);

imp class_do(Class *class, char* functionName, int number, void *passingArgs);
imp instance_do(obj instance, char* functionName, int number, void *passingArgs);
imp generic_do(void* object, char* functionName, int number, void *passingArgs);
int getType(void *object);

#endif