# Coop
Dirty dynamic runtime and Object-Oriented Programming for LLVM C

This is my first project using regular C, so it's probably really bad.

Uses blocks and structures to create a dynamic runtime, including
 - Classes
 - Class Properties
 - Dynamic Method Calling
 - Method Swizzling
 
Basic Usage
----

	
Create A Class: 

			Classname inheritsFrom(Superclass or NULL);

Add class method:

 			classFunc(class, method name, {
				(implementation)
 				self is the class on which function was called
 				arguments are void pointers in array called "args"
 			});
 
Add an instance method:
 
 			instanceFunc(instance, method name, {
				(implementation)
 				self is the instance on which the function was called
 				arguments are void pointers in array called "args"
  			});
 
Call a method without arguments (cast to return type):

 			$(class or instance, method name);
 
Call a method with arguments:

 			$$(class or instance, method name, # of args, array of args)

Working
---
 - Class creation
 - Function creation
 - Dynamic Function Calling
 - Function Arguments
 - Class Instances

To Do
---
 - Properties
 - Proper Memory Management
 - Everything Else
 
Compiling the Sample
---

  You literally cd to the directory and type `clang Coop.c -o Coop`

== 

####You really shouldn't use this.

Seriously. It's awful.
