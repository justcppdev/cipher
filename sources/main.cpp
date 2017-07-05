#include "cipher.hpp"
#include "password.hpp"
#include "wipe.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

int main(int argc, char* argv[]) {
    using ns_cryptor::Cipher;
    using ns_cryptor::Cipher_aes_gcm;
    using ns_cryptor::get_password;
    using ns_cryptor::wipe;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;
    fs::path output_dir = fs::current_path();

    po::options_description description("Allowed options");
    description.add_options()
        ("help,h", "show this message")

        ("encrypt,e",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file")

        ("decrypt,d",
            po::value<std::vector<fs::path>>()
            ->value_name("path"), "file")

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

        if (vm.count("help")) {
            std::cout << description << std::endl;
            return 0;
        }

        if (fs::exists(output_dir) == false) {
            throw std::string(output_dir.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_directory(output_dir) == false) {
            throw std::string(output_dir.c_str()) +=
                " is not directory";
        }

        std::unique_ptr<Cipher> cipher =
            std::make_unique<Cipher_aes_gcm>(get_password());

        if (vm.count("encrypt")) {
            for(auto const& path: vm["encrypt"].as<std::vector<fs::path>>()) {
                cipher->encrypt(path, output_dir);
            }
        }

        if (vm.count("decrypt")) {
            for(auto const& path: vm["decrypt"].as<std::vector<fs::path>>()) {
                cipher->decrypt(path, output_dir);
            }
        }

        if (vm.count("wipe")) {
            for(auto const& path: vm["wipe"].as<std::vector<fs::path>>()) {
                wipe(path);
                fs::remove_all(path);
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
