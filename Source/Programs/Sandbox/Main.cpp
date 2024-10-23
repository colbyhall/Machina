#include <Core/Core.h>
#include <Gui/Application.h>
#include <Gui/Window.h>

using namespace Grizzly;

int main(int argc, char** argv) {
	Gui::Application application{ argc, argv };
	auto window = application.spawn_window({
		.title = u8"Foo Bar",
		.size = { 1280, 720 },
	});
	window->show(Gui::Window::Visibility::Visible);
	return application.run();
}
