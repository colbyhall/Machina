#include <Core/Core.hpp>
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

	return application.run();
}
