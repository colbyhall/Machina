#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;

int main(int argc, char** argv) {
	auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});
	auto application = GUI::Application::create(*device);

	auto window = GUI::Window::create({
		.title = u8"Foo Bar",
		.size = { 1280, 720 },
	});
	window->show();

	return application->run();
}
