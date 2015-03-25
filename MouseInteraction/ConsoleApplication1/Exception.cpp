#include "stdafx.h"
#include "Exception.h"


Exception::Exception()
{
}


Exception::~Exception()
{
}

const char * Exception::what() {
	return "Unknown error";
}