#include <memory>
#include <iostream>

#include <App/App.h>

int main() {
	std::cout << "App start!" << std::endl;

	std::unique_ptr<App> app = std::make_unique<App>();

	app->Initialize();

	app->Loop();
	
	app->Terminate();

	std::cout << "App end!" << std::endl;

	return 0;
}