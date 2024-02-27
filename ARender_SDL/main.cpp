
#include "apllication.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	Apllication app;

	if (!app.initApp()) {
		return 0;
	}
	app.runApp();
	app.endApp();

	return 0;
}
