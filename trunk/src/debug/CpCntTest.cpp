#include <iostream>
#include "CopyCounter.h"
using namespace std;

class Counted : public CopyCounter
{

};
void f(Counted c)
{
	cout << "In function f: " << endl;
	cout << "\tid: " << c.id() << " current count: " << c.currentCount()
	     << " total count: " << c.totalCount() << endl;
}

int main()
{
	Counted a;
	cout << "\tid: " << a.id() << " current count: " << a.currentCount()
	     << " total count: " << a.totalCount() << endl;
	cout << "\tid: " << a.id() << " current count: " << a.currentCount()
	     << " total count: " << Counted(a).totalCount() << endl;
	Counted *p = new Counted(a);
	cout << "\tid: " << a.id() << " current count: " << p->currentCount()
	     << " total count: " << p->totalCount() << endl;
	f(a);
	delete p;
	cout << "\tid: " << a.id() << " current count: " << a.currentCount()
	     << " total count: " << a.totalCount() << endl;
	return 0;
}
