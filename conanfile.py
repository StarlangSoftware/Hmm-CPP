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
    exports_sources = "src/*"
    no_copy_source = True

    def package(self):
        copy("*.h", keep_path=False, src=join(self.source_folder), dst=join(self.package_folder, "include"), pattern="*.h")
