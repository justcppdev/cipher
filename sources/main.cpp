#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    using ns_cryptor::encrypt_file;
    using ns_cryptor::decrypt_file;
    using ns_cryptor::encrypt_directory;
    using ns_cryptor::decrypt_directory;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;
    fs::path output_dir = fs::current_path();

    po::options_description description("Allowed options");
    description.add_options()
        ("help,h", "show this message")
        ("sync,s", "sync with output directory")

        ("encrypt,e", po::value<std::vector<fs::path>>(), "")
        ("decrypt,d", po::value<std::vector<fs::path>>(), "")

        ("output,o", po::value<fs::path>(&output_dir), "output directory")
    ;

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (fs::exists(output_dir) == false)
            throw "output directory doesn't exist";

        if (vm.count("encrypt")) {
            for (auto const& path: vm["encrypt"].as<std::vector<fs::path>>()) {
                if (fs::exists(path)) {
                    if (fs::is_regular(path)) encrypt_file(path); else
                    if (fs::is_directory(path)) encrypt_directory(path);
                }
            }
        }

        if (vm.count("decrypt")) {
            for (auto const& source_path: vm["decrypt"].as<std::vector<fs::path>>()) {
            }
        }

    } catch (po::error const& e) {
        std::cout << e.what() << std::endl;
    } catch (char const* e) {
        std::cout << e << std::endl;
    }
}
