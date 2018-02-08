#pragma once

#include <iostream>
#include "File3.h"
typedef int abc;


class File2 {
public:
	void show2();
private:
	Test3::File3 c;

};

void File2::show2() {
	std::cout << "\nInside File2 show function\n";
	c.show3();
}


