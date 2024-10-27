#include <Core/Core.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;

int main(int argc, char** argv) {
	GUI::Application application{ argc, argv };
	auto window = application.spawn_window({
		.title = u8"Foo Bar",
		.size = { 1280, 720 },
	});
	window->show();

	auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});

	return application.run();
}
