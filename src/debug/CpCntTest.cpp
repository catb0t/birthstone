#include <iostream>
#include "CopyCounter.h"
using namespace std;

class Counted : public CopyCounter
{

};
void f(Counted c)
{
	cout << "In function f: " << endl;
	cout << "\tcurrent count: " << c.currentCount()
	     << " total count: " << c.totalCount() << endl;
}

int main()
{
	Counted a;
	cout << "\tcurrent count: " << a.currentCount()
	     << " total count: " << a.totalCount() << endl;
	cout << "\tcurrent count: " << a.currentCount()
	     << " total count: " << Counted(a).totalCount() << endl;
	Counted *p = new Counted(a);
	cout << "\tcurrent count: " << p->currentCount()
	     << " total count: " << p->totalCount() << endl;
	f(a);
	delete p;
	cout << "\tcurrent count: " << a.currentCount()
	     << " total count: " << a.totalCount() << endl;
	return 0;
}
