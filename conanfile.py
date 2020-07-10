from conans import ConanFile, CMake


class DI(ConanFile):
    name = "DI"
    version = "latest"
    url = "https://github.com/boost-ext/di"
    license = "Boost"
    description = "[Boost::ext].DI - C++14 Dependency Injection Library"
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.hpp")

