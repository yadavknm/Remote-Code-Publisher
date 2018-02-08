#pragma once

#include "File2.h"
#include "File3.h"
#include <iostream>

typedef int abc;
struct  newStruct
{

};
void func1() {}
namespace Test1 {
	void func2() {}
	class File1 {
	public:
		void show1();
	private:
		File2 file2;
		Test3::File1 a;
	};

	void File1::show1() {
		std::cout << "\nInside File1 show function\n";
		file2.show2();
	}
}
