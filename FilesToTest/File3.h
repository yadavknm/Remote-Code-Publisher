#pragma once
#include "File4.h"
#include "File1.h"
typedef int abc;

void func3() {}
namespace Test3 {
	void func4() {}
	class File3 {
	public:
		void show3();
	private:
		Test1::File1 a;
		File4 d;
	};

	void File3::show3()
	{
		a.show1();
		d.show4();
		std::cout << "\nInside File3 show function\n";
	}

}
 
