#pragma once

#include <string>

namespace GameEngine {

	/* Because this function is not defined in the header file
	we have to include the prefix "extern" in front of it. You
	don't have to do this on the other function because they
	are all contained within a class. Since this isn't, it has
	to use "extern".*/

	extern void fatalError(std::string errorString);

}