#ifndef _sanddollar_h
#define _sanddollar_h
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

fs::path get_cache_path(const fs::path &targetfile);
fs::path convertscript(const fs::path &inputfile, const fs::path &cachepath, const po::variables_map &vm);
fs::path compilescript(const fs::path &inputfile, const fs::path &cachepath, const po::variables_map &vm);
int runscript(const fs::path &compiledscript, const std::vector<std::string> &params, const po::variables_map &vm);
int parse_input(const int argc, const char *argv[], std::vector<std::string> *target);
fs::path get_cached_script(const fs::path &targetfile, const po::variables_map &vm);
fs::path get_cache_path(const fs::path &targetfile);

#endif
