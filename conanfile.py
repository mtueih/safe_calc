from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class SafeCalcConan(ConanFile):
    name = "safe_calc"
    version = "0.1.0"

    # ------------------------------------------------------------------
    # 包元数据
    # ------------------------------------------------------------------
    package_type = "static-library"
    license = "GPL-3.0-or-later"
    author = "mtueih"
    description = (
        "A safe arithmetic library for size_t operations "
        "(add / sub / mul / align_up) with overflow detection."
    )
    topics = ("arithmetic", "safe", "overflow", "size_t")
    url = "https://github.com/mtueih/safe_calc"

    # ------------------------------------------------------------------
    # 配置维度
    # ------------------------------------------------------------------
    settings = "os", "compiler", "build_type", "arch"

    # ------------------------------------------------------------------
    # 随包分发的文件（不参与构建）
    # ------------------------------------------------------------------
    exports = "LICENSE"

    # ------------------------------------------------------------------
    # 打包时导出的源码（排除 tests/）
    # ------------------------------------------------------------------
    exports_sources = (
        "CMakeLists.txt",
        "src/*",
        "include/*",
        "cmake/*"
    )

    # ------------------------------------------------------------------
    # 布局
    # ------------------------------------------------------------------
    def layout(self):
        cmake_layout(self)

    # ------------------------------------------------------------------
    # 生成工具链 + 传递 CMake 变量
    # ------------------------------------------------------------------
    def generate(self):
        tc = CMakeToolchain(self)
        # 打包时不构建测试，仅产出库
        tc.variables["BUILD_TESTING"] = "OFF"
        tc.generate()

    # ------------------------------------------------------------------
    # 构建
    # ------------------------------------------------------------------
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    # ------------------------------------------------------------------
    # 打包（复用 CMakeLists.txt 中已有的 install 规则）
    # ------------------------------------------------------------------
    def package(self):
        cmake = CMake(self)
        cmake.install()

    # ------------------------------------------------------------------
    # 消费者信息
    # ------------------------------------------------------------------
    def package_info(self):
        self.cpp_info.libs = ["safe_calc"]
        self.cpp_info.set_property("cmake_target_name", "safe_calc::safe_calc")
        self.cpp_info.set_property("cmake_file_name", "safe_calc")
