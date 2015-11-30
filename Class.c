#include "Class.h"




int main()
{
	
	
	//newClass(myClass, NULL);
	Class myClass inheritsFrom(NULL);

	func(myClass, checkStuffFunction, {
		printf("checking to see if this thing works\n");
		return 420;
	});

	int returnedValue = (int)$(myClass, checkStuffFunction);
	printf("%d\n", returnedValue);

    
    return 0;
}

void class_addFunction(Class *class, classFunction *function) {

	// int oldSize = class.functionsSize;
	// printf("old function size is %d\n", oldSize );
	// printf("new size is %lu\n", sizeof(function) * (oldSize + 1));


	int oldSize  = class->functionsSize;
	realloc(class->functions, sizeof(classFunction) * (oldSize + 1));
	// classFunction newArray[sizeof(classFunction) * (oldSize + 1)];// malloc(sizeof(function) * (oldSize + 1));
	// //printf("%s\n", class.functions );
	// int i;
	// for (i=0; i<=oldSize; i++) {
	// 	//*(&newArray + i) = *(&class.functions + i);
	// 	newArray[i] = class->functions[i];
	// }
	//*(&newArray + oldSize) = &function;
	//newArray[oldSize] = *function;
	class->functions[oldSize] = *function;
	//class->functions = newArray;
	class->functionsSize = oldSize + 1;


	
}

void* class_do(Class *class, char* functionName) {
	printf("doing function %s\n", functionName);
	classFunction* functions = class->functions;
	int numberOfFunctions = class->functionsSize;
	printf("there are %d functions\n", numberOfFunctions);
	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf("checking function %d: %s\n", i+1, functions[i].name);
		if (functions[i].name == functionName)
		{
			printf("it's function %d\n", i+1);
			void *(^asBlock)() = (blockType) (functions[i]).implementation;
			return asBlock();
		}
	}
	return NULL;
}

classFunction makeFunction(char* name, void* implementation) {

	classFunction newFunction;
	newFunction.name = name;
	newFunction.implementation = implementation;
	return newFunction;
}


Class makeClass(Class *parent) {
	return (Class) {
		.inherits = parent,
		.functions = malloc(sizeof(classFunction)),
		.functionsSize = 0,
		.properties = malloc(sizeof(classProperty)),
		.propertiesSize = 0
	};
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








