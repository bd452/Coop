#include "Coop.h"

void initializeStuff();
void initBaseClasses();


/*
 *	Basic Usage:
 *	
 *		Create Class: 
 *			Classname inheritsFrom(Superclass or NULL);
 *
 *		Add class method:
 *			classFunc(class, method name, {
 *
 *				implementation
 *				self is the class on which function is called
 *				arguments are void pointers in array called "args"
 *			});
 *
 *		Add an instance method:
 *			instanceFunc(class, method name, {
 *				implementation
 *				self is the instance on which function is called
 *				arguments are void pointers in array called "args"
 * 			});
 *
 *		Call a method without arguments:
 *			$(class or instance, method name);
 *
 *		Call a method with arguments:
 *			$$(class or instance, method name, # of args, array of args)
 *
  */


int main()
{
	initBaseClasses();
	initializeStuff();
	
	


	return 0;
}

Class BDObject;

//Basic Initialization. Makes base classes, etc.

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


	printf( COLOR_MAGENTA "\n\n\n========= RUNTIME INITIALIZED =========\n\n\n" COLOR_RESET "\n");
}

//Currently just for testing.

void initializeStuff() {


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

	//generic_do(myClassInstance, "printlol", 0, NULL);
	$(myClassInstance, printlol);
	int stuff[2] = { 4, 5 };
	int returnedAddedValue = (int)$$(myClass, add4, 2, stuff);
	int returnedValue = (int)$(myClass, checkStuffFunction);
	printf("%d\n", returnedAddedValue);


}

//Adds a given class function to a class

void class_addClassFunction(Class *class, classFunction *function) {
	printf( COLOR_YELLOW " - adding %s to %s\n" COLOR_RESET, function->name, class->name);
	
	//Gets the current number of class functions

	int oldSize  = class->functionsSize;

	//re-allocates memory for our array with capacity for one more function

	struct classFunction *newClassFunctions = realloc(class->functions, sizeof(classFunction) * (oldSize + 1));

	if (newClassFunctions) {

		//Sets the class's function variable to our newly allocated array

		class->functions = newClassFunctions;

		//Sets the last item of the array to our new function

		class->functions[oldSize] = *function;

		//Increments the counter for the number of functions

		class->functionsSize = oldSize + 1;
	} else {

		//If something goes wrong show an error

		errorLog("couldn't add %s to %s", function->name, class->name);
	}

	
}
void class_addInstanceFunction(Class* class, instanceFunction *function) {

	printf( COLOR_YELLOW " - adding %s to %s\n" COLOR_RESET, function->name, class->name);

	//Gets the current number of instance functions

	int oldSize = class->instanceFunctionsSize;

	//re-allocates memory for our array with capacity for one more function

	struct instanceFunction *newInstanceFunctions = realloc(class->instanceFunctions, sizeof(instanceFunction) * (oldSize + 1));
	if (newInstanceFunctions) {

		//Sets the class's function variable to our newly allocated array

		class->instanceFunctions = newInstanceFunctions;

		//Sets the last item of the array to our new function

		class->instanceFunctions[oldSize] = *function;

		//Increments the counter for the number of functions

		class->instanceFunctionsSize = oldSize + 1;

	}  
	else {
		errorLog("couldn't add %s to %s", function->name, class->name);
	}
	
}

//Do a class method on a given class


imp class_do(Class *class, char* functionName, int number, void *passingArgs) {


	printf( COLOR_BLUE "doing function %s on Class %s" COLOR_RESET "\n", functionName, class->name);

	//Get the list of class functions for the given class

	classFunction* functions = class->functions;

	//Gets the number of class functions

	int numberOfFunctions = class->functionsSize;
	printf(" - there are %d functions\n", numberOfFunctions);

	//Enumerate through the functions to find the one with a matching name

	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf(" -- checking function %d: %s\n", i+1, functions[i].name);
		if (functions[i].name == functionName)
		{
			printf(" --- it's function %d\n", i+1);

			//When we find it, call its implementation

			void *(^asBlock)(void*, void*) = (blockType) (functions[i]).implementation;
			return asBlock(class, passingArgs);
		}
	}
	return NULL;
}

//Do an instance method on a given class

imp instance_do(obj instance, char* functionName, int number, void *passingArgs) {

	Class *class = &instance->frame;
	printf( COLOR_BLUE "doing function %s on instance of class %s" COLOR_RESET "\n", functionName, class->name);
	
	//Get the list of instance functions for the given class

	instanceFunction* functions = class->instanceFunctions;

	//Gets the number of instance functions

	int numberOfFunctions = class->instanceFunctionsSize;
	printf(" - there are %d functions\n", numberOfFunctions);

	//Enumerate through the functions to find the one with a matching name

	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf(" -- checking function %d: %s\n", i+1, functions[i].name);
		if (functions[i].name == functionName)
		{
			printf(" --- it's function %d\n", i+1);

			//When we find it, call its implementation

			void *(^asBlock)(void*, void*) = (blockType) (functions[i]).implementation;
			return asBlock(instance, passingArgs);
		}
	}
	return NULL;
}

//Do a class or instance method on a given class or instance

imp generic_do(void* myObject, char* functionName, int number, void *passingArgs) {
	logPrint("object type is %d", ((int*)myObject)[0]);

	//Test the structure's type (grabs the first int from myObject)
	//If it's a Class, this will be 1
	//If it's an instance, it will be 2

	if (((int*)myObject)[0] == 1) {
		return class_do(myObject, functionName, number, passingArgs);
	} else if ((*(int**)myObject)[0] == 2){
		return instance_do(myObject, functionName, number, passingArgs);
	} else {
		errorLog("error doing function %s", functionName);
		return NULL;
	}
}

//Make a new class method

classFunction makeClassFunction(char* name, imp implementation) {
	printf( COLOR_GREEN " - making class function %s\n" COLOR_RESET, name);
	classFunction newFunction;
	newFunction.name = name;
	newFunction.implementation = implementation;
	return newFunction;
}

//Make a new instance method

instanceFunction makeInstanceFunction(char* name, imp implementation) {
	printf( COLOR_GREEN " - making instance function %s\n" COLOR_RESET, name);
	instanceFunction newFunction;
	newFunction.name = name;
	newFunction.implementation = implementation;
	return newFunction;
}

//Make a new class

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

//Make a new class instance

obj makeClassInstance(Class *parent) {

	obj intermediateObject = malloc(sizeof(*parent));
	intermediateObject->frame = *parent;
	intermediateObject->structType = 2;
	return intermediateObject;
}



/








