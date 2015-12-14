#include "Coop.h"

void initializeStuff();
void initBaseClasses();



int main()
{
	initBaseClasses();
	initializeStuff();
	
	


	return 0;
}

Class BDObject;

void initBaseClasses() {

	printf( COLOR_MAGENTA "\n\n\n========= INITIALIZING RUNTIME =========\n\n\n" COLOR_RESET "\n");
	BDObject inheritsFrom(NULL);
	BDObject.name = "BDObject";

	classFunc(BDObject, new, {
		return makeClassInstance(&BDObject);
	});

	classFunc(BDObject, newSubclass, {
		return makeClass(&BDObject);
	});

	instanceFunction BDObject_printlol = makeInstanceFunction("printlol", ^(obj self, void* args) {
		logPrint("lol");
		//instance_do(self, "printlol", 0, NULL);
	});
	class_addInstanceFunction(&BDObject, &BDObject_printlol);

	// instanceFunc(BDObject, printlol, {
	// 	printf("lol");
	// });
		printf( COLOR_MAGENTA "\n\n\n========= RUNTIME INITIALIZED =========\n\n\n" COLOR_RESET "\n");
}

void initializeStuff() {
	//Class myClass inheritsFrom(&BDObject);

	Class myClass = *(Class *)$(BDObject, newSubclass);
	myClass.name = "myClass";

	obj myClassInstance = $(BDObject, new);



	classFunc(myClass, checkStuffFunction, {
		logPrint("checking to see if this thing works\n");
		return 420;
	});

	classFunc(myClass, add4, {
		int *array = (int *)args;
		int added = array[0] + array [1];
		logPrint("The numbers are %d and %d\n", array[0], array[1]);
		return added;
	});

	generic_do(myClassInstance, "printlol", 0, NULL);
	//$(myClassInstance, printlol);
	int stuff[2] = { 4, 5 };
	int returnedAddedValue = (int)$$(myClass, add4, 2, stuff);
	int returnedValue = (int)$(myClass, checkStuffFunction);
	printf("%d\n", returnedAddedValue);


}

void class_addClassFunction(Class *class, classFunction *function) {
	printf( COLOR_YELLOW " - adding %s to %s\n" COLOR_RESET, function->name, class->name);
	int oldSize  = class->functionsSize;
	struct classFunction *newClassFunctions = realloc(class->functions, sizeof(classFunction) * (oldSize + 1));
	if (newClassFunctions) {
	class->functions = newClassFunctions;
	class->functions[oldSize] = *function;
	//class->functions = newArray;
	class->functionsSize = oldSize + 1;
} else {
	errorLog("couldn't add %s to %s", function->name, class->name);
}

	
}
void class_addInstanceFunction(Class* class, instanceFunction *function) {

	printf( COLOR_YELLOW " - adding %s to %s\n" COLOR_RESET, function->name, class->name);
	int oldSize = class->instanceFunctionsSize;
	struct instanceFunction *newInstanceFunctions = realloc(class->instanceFunctions, sizeof(instanceFunction) * (oldSize + 1));
	if (newInstanceFunctions) {
		class->instanceFunctions = newInstanceFunctions;
		class->instanceFunctions[oldSize] = *function;
		class->instanceFunctionsSize = oldSize + 1;

	}  
	else {

	}
	
}


imp class_do(Class *class, char* functionName, int number, void *passingArgs) {


	printf( COLOR_BLUE "doing function %s on Class %s" COLOR_RESET "\n", functionName, class->name);
	classFunction* functions = class->functions;
	int numberOfFunctions = class->functionsSize;
	printf(" - there are %d functions\n", numberOfFunctions);
	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf(" -- checking function %d: %s\n", i+1, functions[i].name);
		if (functions[i].name == functionName)
		{
			printf(" --- it's function %d\n", i+1);
			void *(^asBlock)(void*, void*) = (blockType) (functions[i]).implementation;
			return asBlock(class, passingArgs);
		}
	}
	return NULL;
}

imp instance_do(obj instance, char* functionName, int number, void *passingArgs) {

	Class *class = &instance->frame;
	printf( COLOR_BLUE "doing function %s on instance of class %s" COLOR_RESET "\n", functionName, class->name);
	
	instanceFunction* functions = class->instanceFunctions;
	int numberOfFunctions = class->instanceFunctionsSize;
	printf(" - there are %d functions\n", numberOfFunctions);
	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf(" -- checking function %d: %s\n", i+1, functions[i].name);
		if (functions[i].name == functionName)
		{
			printf(" --- it's function %d\n", i+1);
			void *(^asBlock)(void*, void*) = (blockType) (functions[i]).implementation;
			return asBlock(instance, passingArgs);
		}
	}
	return NULL;
}

imp generic_do(void* myObject, char* functionName, int number, void *passingArgs) {
	logPrint("object type is %d", ((int*)myObject)[0]);
	if (((int*)myObject)[0] == 1) {
		return class_do(myObject, functionName, number, passingArgs);
	} else if (((int*)myObject)[0] == 2){
		return instance_do(myObject, functionName, number, passingArgs);
	} else {
		errorLog("error doing function %s", functionName);
		return NULL;
	}
}



classFunction makeClassFunction(char* name, imp implementation) {
	printf( COLOR_GREEN " - making class function %s\n" COLOR_RESET, name);
	classFunction newFunction;
	newFunction.name = name;
	newFunction.implementation = implementation;
	return newFunction;
}

instanceFunction makeInstanceFunction(char* name, imp implementation) {
	printf( COLOR_GREEN " - making instance function %s\n" COLOR_RESET, name);
	instanceFunction newFunction;
	newFunction.name = name;
	newFunction.implementation = implementation;
	return newFunction;
}

Class makeClass(Class *parent) {
	printf( COLOR_RED "making subclass of %s" COLOR_RESET "\n", (parent ? parent->name : "nothing"));
	return (Class) {
		.inherits = parent,
		.functions = malloc(sizeof(classFunction)),
		.instanceFunctions = malloc(sizeof(instanceFunction)),
		.functionsSize = 0,
		.instanceFunctionsSize = 0,
		.properties = malloc(sizeof(classProperty)),
		.propertiesSize = 0,
		.structType = 1

	};
}

obj makeClassInstance(Class *parent) {

	obj intermediateObject = malloc(sizeof(*parent));
	intermediateObject->frame = *parent;
	intermediateObject->structType = 2;
	return intermediateObject;
}



/*
Class (structure):
	Functions (array):
		structure (pointer):
			name (char pointer)
			implementation (function pointer)
				classname_methodname(args)
	Instance Variables (void pointer array)

*/








