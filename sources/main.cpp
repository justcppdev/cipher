#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    using ns_cryptor::encrypt;
    using ns_cryptor::decrypt;
    using ns_cryptor::wipe;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;
    fs::path output_dir = fs::current_path();
    bool test = false;

    po::options_description description("Allowed options");
    description.add_options()
        ("test,t",
            po::bool_switch(&test)
            ->default_value(false), "mode")

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
            for (auto const& path:
                vm["encrypt"].as<std::vector<fs::path>>())
                    encrypt(path, output_dir, test);
        }

        if (vm.count("decrypt")) {
            for (auto const& path:
                vm["decrypt"].as<std::vector<fs::path>>())
                    decrypt(path, output_dir, test);
        }

        if (vm.count("wipe")) {
            for (auto const& path:
                vm["wipe"].as<std::vector<fs::path>>())
                    wipe(path, test);
        }

    } catch (po::error const& e) {
        std::cout << e.what() << std::endl;
    } catch (std::string const& e) {
        std::cout << e << std::endl;
    } catch (char const* e) {
        std::cout << e << std::endl;
    }
}
