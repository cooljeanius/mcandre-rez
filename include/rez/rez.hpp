#pragma once

/**
 * @copyright 2021 YelloSoft
 * @mainpage
 *
 * @ref rez runs C++ tasks.
 */

#include <filesystem>
#include <optional>

/**
 * @brief rez manages C++ tasks.
 */
namespace rez {
/**
 * @brief Version is semver.
 */
static const char *Version = "0.0.1";

/**
 * @brief RezDefinitionPathCpp denotes the path to a C++ task definition source file.
 */
static const char *RezDefinitionPathCpp = "rez.cpp";

/**
 * @brief RezDefinitionPathC denotes the path to a C task definition source file.
 */
static const char *RezDefinitionPathC = "rez.c";

/**
 * @brief CacheDir denotes the path to the rez internal cache directory.
 */
static const char *CacheDir = ".rez";

/**
 * @brief CacheFileBasename denotes the basename of the rez internal cache file.
 *
 * The cache file is nested as a subfile of @ref CacheDir.
 *
 * The file is primarily in Windows runtime environments to track low level MSVC configuration details.
 */
static const char *CacheFileBasename = "rez-env.txt";

/**
 * @brief ArtifactDirBasename denotes the basename of the cache subdirectory where user task binaries are housed.
 */
static const char *ArtifactDirBasename = "bin";

/**
 * @brief ArtifactBinaryUnix denotes the basename of user task binaries generated by UNIX compilers.
 */
static const char *ArtifactBinaryUnix = "delegate-rez";

/**
 * @brief DefaultCompilerWindows denotes the standard Microsoft Visual C++ (MSVC) compiler executable basename.
 *
 * This compiler is activated automatically when the runtime environment is detected as (COMSPEC) Windows.
 *
 * The compiler may be overridden by supplying a non-blank value to the CXX environment variable.
 *
 * Custom flags may be passed to the compiler via a CPPFLAGS or CXXFLAGS environment variable.
 */
static const char *DefaultCompilerWindows = "cl";

/**
 * @brief DefaultCompilerUnixCpp denotes the standard UNIX C++ compiler executable basename.
 *
 * The compiler may be overridden by supplying a non-blank value to the CXX environment variable.
 *
 * Custom flags may be passed to the compiler via a CPPFLAGS and/or CXXFLAGS environment variable.
 */
static const char *DefaultCompilerUnixCpp = "c++";

/**
 * @brief DefaultCompilerUnixC denotes the standard UNIX C compiler executable basename.
 *
 * The compiler may be overridden by supplying a non-blank value to the CFLAGS environment variable.
 *
 * Custom flags may be passed to the compiler via a CPPFLAGS and/or CFLAGS environment variable.
 */
static const char *DefaultCompilerUnixC = "cc";

/**
 * @brief DefaultMSVCToolchainQueryScript denotes the standard script which prepares environment variables for executing MSVC cl commands.
 *
 * To override this, set a REZ_TOOLCHAIN_QUERY_PATH environment variable.
 */
static const char *DefaultMSVCToolchainQueryScript = R"(C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat)";

/**
 * @brief ArchitectureMsvcAmd64 denotes the amd64 architecture in MSVC nomenclature.
 */
static const char *ArchitectureMsvcAmd64 = "x64";

/**
 * @brief Lang denotes a programming language.
 */
enum class Lang {
    /**
     * @brief Cpp denotes C++, object oriented C.
     */
    Cpp,

    /**
     * @brief C denotes C, the successor to BCPL.
     */
    C
};

/**
 * @brief << formats a Lang to an ostream.
 *
 * @param os an output stream
 * @param o a Lang
 * @returns the output stream result
 */
std::ostream &operator<<(std::ostream &os, const Lang &o);

/**
 * @brief GetEnvironmentVariable retrieves environment variables.
 *
 * @param key the name of an environment variable
 * @returns std::nullopt on missing environment variables
 */
std::optional<std::string> GetEnvironmentVariable(const std::string &key);

/**
 * @brief DetectWindowsEnvironment determines whether the runtime environment is (COMSPEC) Windows.
 *
 * @returns true when COMSPEC Windows is detected.
 *
 * Native Command Prompt and PowerShell environments are expected to evaluate as Windows.
 *
 * Cygwin-style environments, such as Windows Subsystem for Linux, Cygwin, MinGW, MSYS2, Git Bash, Strawberry Perl, etc., are expected to evaluate as not Windows.
 */
bool DetectWindowsEnvironment();

/**
 * @brief Config parameterizes rez builds.
 */
struct Config {
    /**
     * @brief debug controls whether additional logging is performed. (Default: false)
     *
     * Examples:
     *
     * * false
     * * true
     */
    bool debug = false;

    /**
     * @brief cache_dir_path denotes the location of the internal cache directory. (Default: Determined at runtime by @ref Load)
     *
     * The path may not necessarily exist prior to invoking rez tasks.
     *
     * Example: std::filesystem::path(".rez")
     */
    std::filesystem::path cache_dir_path;

    /**
     * @brief cache_file_path denotes the qualified path of the internal cache file. (Default: Determined at runtime by @ref Load)
     *
     * The path may not necessarily exist prior to invoking rez tasks.
     *
     * Example: std::filesystem::path(".rez\rez-cache.txt")
     */
    std::filesystem::path cache_file_path;

    /**
     * @brief windows denotes whether the runtime environment is (COMSPEC) Windows. (Default: Determined at runtime by @ref Load)
     *
     * Examples:
     *
     * * false
     * * true
     */
    bool windows = false;

    /**
     * @brief rez_definition_path denotes the user's task definition source file. (Default: rez.cpp)
     *
     * If no rez.cpp file is present, then rez.c is checked as a fallback.
     *
     * Examples:
     *
     * * std::filesystem::path("rez.cpp")
     * * std::filesystem::path("rez.c")
     */
    std::filesystem::path rez_definition_path;

    /**
     * @brief Lang denotes the programming language for the user's task definition source file. (Default: Lang::Cpp)
     *
     * Examples:
     *
     * * Lang::Cpp
     * * Lang::C
     */
    Lang rez_definition_lang = Lang::Cpp;

    /**
     * @brief compiler denotes the executable used to build the user task tree. (Default: Determined at runtime by @ref Load)
     *
     * Examples:
     *
     * * "c++"s
     * * "cc"s
     * * "cl"s
     * * "clang++"s
     * * "clang"s
     * * "g++"s
     * * "gcc"s
     */
    std::string compiler;

    /**
     * @brief artifact_dir_path denotes the path to the artifact subdirectory. (Default: Determined at runtime by @ref Load)
     *
     * Examples:
     *
     * * std::filesystem::path(".rez") / "bin"
     */
    std::filesystem::path artifact_dir_path;

    /**
     * @brief artifact_file_path denotes the binary path where user task executable shall be generated (Default: Determined at runtime by @ref Load)
     *
     * Examples:
     *
     * * std::filesystem::path(".rez") / "bin" / "rez"
     * * std::filesystem::path(".rez") / "bin" / "rez.exe"
     */
    std::filesystem::path artifact_file_path;

    /**
     * @brief build_command denotes the compilation step for the user task source file (Default: Determined at runtime by @ref Load)
     *
     * Examples:
     *
     * * "c++ -o .rez/bin/rez rez.cpp"s
     * * R"(cl rez.cpp /link /out:.rez\bin\rez.exe)"s
     */
    std::string build_command;

    /**
     * @brief ApplyMSVCToolchain loads MSVC environment variables for cl into the current process.
     *
     * By default, the target architecture x64 is assumed. Set a environment variable REZ_ARCH to override.
     *
     * @throws an error in the event of a problem
     */
    void ApplyMSVCToolchain() const;

    /**
     * @brief Load populates build parameters according to the documented defaults and override mechanisms.
     *
     * @throws an error in the event of a problem
     */
    void Load();
};

/**
 * @brief << formats a Config to an ostream.
 *
 * @param os an output stream
 * @param o a Config
 * @returns the output stream result
 */
std::ostream &operator<<(std::ostream &os, const Config &o);
}
