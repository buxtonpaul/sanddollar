#ifndef _sanddollar_h
#define _sanddollar_h
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

fs::path get_cache_path(fs::path targetfile);
fs::path convertscript(fs::path inputfile, fs::path cachepath);
fs::path compilescript(fs::path inputfile, fs::path cachepath);
int runscript(fs::path compiledscript, std::vector<std::string> params);
int parse_input(int argc, char *argv[], std::vector<std::string> *target);
fs::path get_cached_script(fs::path targetfile);
fs::path get_cache_path(fs::path targetfile);

#endif
