#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    using ns_cryptor::encrypt_file;
    using ns_cryptor::decrypt_file;
    using ns_cryptor::encrypt_directory;
    using ns_cryptor::decrypt_directory;
    using ns_cryptor::wipe_file;
    using ns_cryptor::wipe_directory;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;
    fs::path output_dir = fs::current_path();
    bool debug = false;

    po::options_description description("Allowed options");
    description.add_options()

//#ifndef NDEBUG
        ("debug",
            po::bool_switch(&debug)
            ->default_value(false), "mode")
//#endif

        ("help,h", "show this message")

        ("encrypt,e",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file or directory")

        ("decrypt,d",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file or directory")

        ("wipe,w",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file or directory")

        ("sync,s",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file or directory with output directory")

        ("output,o",
            po::value<fs::path>(&output_dir)
            ->value_name("path"), "output directory")
    ;

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (fs::exists(output_dir) == false)
            throw std::string(output_dir.c_str()) +=
                " doesn't exist";

        if (vm.count("help")) {
            std::cout << description << std::endl;
            return 0;
        }

        if (vm.count("encrypt")) {
            for (auto const& path: vm["encrypt"].as<std::vector<fs::path>>()) {
                if (fs::exists(path)) {
                    if (fs::is_regular(path)) encrypt_file(path); else
                    if (fs::is_directory(path)) encrypt_directory(path);
                }
            }
        }

        if (vm.count("decrypt")) {
            for (auto const& path: vm["decrypt"].as<std::vector<fs::path>>()) {
                if (fs::exists(path)) {
                    if (fs::is_regular(path)) decrypt_file(path); else
                    if (fs::is_directory(path)) decrypt_directory(path);
                }
            }
        }

        if (vm.count("wipe")) {
            for (auto const& path: vm["wipe"].as<std::vector<fs::path>>()) {
                if (fs::exists(path)) {
                    if (fs::is_regular(path)) wipe_file(path, debug); else
                    if (fs::is_directory(path)) wipe_directory(path, debug);
                }
            }
        }

    } catch (po::error const& e) {
        std::cout << e.what() << std::endl;
    } catch (std::string const& e) {
        std::cout << e << std::endl;
    } catch (char const* e) {
        std::cout << e << std::endl;
    }
}
