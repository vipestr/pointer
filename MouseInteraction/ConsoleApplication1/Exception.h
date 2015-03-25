#pragma once
class Exception
{
public:
	Exception();
	~Exception();
	virtual const char *what();
};

