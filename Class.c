#include "Class.h"




int main()
{
	int (^checkShit)() = ^int() {
    		return 69;
    		printf("%d\n", 69 );
    	};
    	int (^checkShit2)() = ^int() {
    		printf("%d\n", 420 );
    		return 420;
    		
    	};
	classFunction myFunction = (classFunction){
		.name = "checkShit",
		.implementation = checkShit
	};
	classFunction myFunction2 = (classFunction){
		.name = "checkShit2",
		.implementation = checkShit2
	};


	classFunction *emptyArray = (classFunction[2 * sizeof(classFunction)]){ myFunction, myFunction2 };

	// Class myClass = (Class) {
	// 	.functions = emptyArray,
	// 	.functionsSize = 0,
	// 	.propertiesSize = 0,

	// };

	Class myClass = makeClass(NULL, emptyArray, 2, NULL, 0);

	// class_addFunction(&myClass, &myFunction);
	// class_addFunction(&myClass, &myFunction2);

	printf("lol \n");
    int checkingMoreShit(void* (^f)());
    printf("lol \n");
    printf("function %s\n", myClass.functions[0].name );
    printf("there are %d functions\n", myClass.functionsSize );
    void *(^asBlock)() = (blockType) (myClass.functions[0]).implementation;

    printf("lol \n");
    int kek = checkingMoreShit((void* (^)())asBlock);
    printf("lol \n");
    printf("%d\n",  kek);

    class_do(&myClass, "checkShit2");
    return 0;
}

void class_addFunction(Class *class, classFunction *function) {

	// int oldSize = class.functionsSize;
	// printf("old function size is %d\n", oldSize );
	// printf("new size is %lu\n", sizeof(function) * (oldSize + 1));


	int oldSize  = class->functionsSize;
	classFunction *newArray[oldSize+1];// malloc(sizeof(function) * (oldSize + 1));
	//printf("%s\n", class.functions );
	int i;
	for (i=0; i<=oldSize; i++) {
		//*(&newArray + i) = *(&class.functions + i);
		newArray[i] = &class->functions[i];
	}
	//*(&newArray + oldSize) = &function;
	newArray[oldSize] = function;
	class->functions = *newArray;
	class->functionsSize = oldSize + 1;


	
}

void* class_do(Class *class, char* functionName) {
	printf("doing function %s\n", functionName);
	classFunction* functions = class->functions;
	int numberOfFunctions = class->functionsSize;
	printf("there are %d functions\n", numberOfFunctions);
	for (int i = 0; i < numberOfFunctions; ++i)
	{
		printf("checking function %d\n", i+1);
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
	return (classFunction){
		.name = name,
		.implementation = implementation
	};
}


Class makeClass(Class *parent, classFunction *functions, int numberOfFunctions, classProperty* properties, int numberOfProperties) {
	return (Class) {
		.inherits = parent,
		.functions = functions,
		.functionsSize = numberOfFunctions,
		.properties = properties,
		.propertiesSize = numberOfProperties
	};
}

int checkingMoreShit(void* (^f)()) {
	return (int)f();
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








