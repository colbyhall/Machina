#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;

int main(int argc, char** argv) {
	GUI::Application application{ argc, argv };
	auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});

	auto window = GUI::Window::create({
		.title = u8"Foo Bar",
		.size = { 1280, 720 },
		.device = *device,
	});
	window->show();

	return application.run();
}
