#include <core/core.h>
#include <gui/application.h>

using namespace grizzly;

int main(int argc, char** argv) {
	gui::Application application{ argc, argv };
	auto window = application.spawn_window({
		.title = u8"Foo Bar",
		.size = { 1280, 720 },
	});
	return application.run();
}
