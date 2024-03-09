from os.path import join

from conan import ConanFile
from conan.tools.files import copy

class HmmConan(ConanFile):
    name = "hmm"
    version = "1.0.0"
    requires = ["math/1.0.0", "data_structure/1.0.0"]
    license = "GPL License"
    author = "Olcay Taner Yildiz olcay.yildiz@ozyegin.edu.tr"
    url = "https://github.com/StarlangSoftware/Math-CPP"
    description = "Hmm library"
    topics = ("")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "src/*"

    def package(self):
        copy(conanfile=self, keep_path=False, src=join(self.source_folder), dst=join(self.package_folder, "include"), pattern="*.h")
