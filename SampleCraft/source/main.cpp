#include "Application.h"



int main(int argc, char* argv[])
{
	SampleCraft::Application* application = SampleCraft::Application::createApplication();

	application->init();
	application->run();

	delete application;
	return 0;
}

