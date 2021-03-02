#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#define VERSION 1.1

namespace po = boost::program_options;
namespace fs = boost::filesystem;

fs::path scriptdir;
fs::path get_cache_path(fs::path targetfile);

fs::path convertscript(fs::path inputfile, fs::path cachepath)
{
    if (!fs::exists(inputfile) || !fs::is_regular_file(inputfile))
        throw std::invalid_argument("Inputfile not found");
    if (!fs::exists(scriptdir / ".sanddollar"))
        fs::create_directory(scriptdir / ".sanddollar");

    if (!fs::exists(cachepath))
        fs::create_directory(cachepath);

    fs::path outfilepath = cachepath / "buildable.cc";

    if (fs::exists(outfilepath))
        fs::remove(outfilepath);

    fs::ifstream in(inputfile);
    std::string line;
    if (!std::getline(in, line))
    {
        throw std::invalid_argument("Unable to read file");
    }
    fs::ofstream out(outfilepath);
    while (std::getline(in, line))
        out << line << std::endl;

    return outfilepath;
}

fs::path compilescript(fs::path inputfile, fs::path cachepath)
{
    std::stringstream invoke;
    invoke << "g++-9 -o " << (cachepath / "runable").string() << " --std=c++17 -lstdc++fs -I " << scriptdir.string() << " " << inputfile.string();
    std::string s = invoke.str();
    int result = std::system(s.c_str());
    if (result)
        throw std::invalid_argument("Script file is invalid");

    return cachepath / "runable";
}

int runscript(fs::path compiledscript, std::vector<std::string> params)
{
    std::stringstream invoke;
    invoke << compiledscript.string();
    for (auto parm : params)
        invoke << " " << parm;
    std::string s = invoke.str();
    int result = std::system(s.c_str());

    return result;
}

int parse_input(int argc, char *argv[], std::vector<std::string> *target)
{
    po::options_description general("Options");

    general.add_options()("help,h", "Display this help message"),
        ("version,v", "Display the version number");

    po::options_description hidden("Hidden options");
    hidden.add_options()("infile,i", po::value<std::vector<std::string>>(target), "Input File");

    po::options_description cmdline("Commandline");

    po::positional_options_description positional;
    positional.add("infile", -1);

    cmdline.add(general).add(hidden);

    po::variables_map vm;

    po::store(po::command_line_parser(argc, argv).options(cmdline).positional(positional).allow_unregistered().run(), vm);
    po::notify(vm);
    if (vm.count("help"))
    {
        std::cout << "Usage:" << std::endl
                  << "sanddollar [options] <inputfile>" << std::endl;
        std::cout << general;
        return EXIT_SUCCESS;
    }
    if (vm.count("version"))
    {
        std::cout << "Version number :" << VERSION << std::endl;
    }

    if (!vm.count("infile"))
    {
        throw std::invalid_argument("Error no input file specified");
    }
    return 0;
}

fs::path get_cached_script(fs::path targetfile)
{
    fs::path cachedscript = get_cache_path(targetfile) / "runable";
    return (cachedscript);
}

fs::path get_cache_path(fs::path targetfile)
{
    fs::path basefolder = targetfile.parent_path();
    basefolder /= ".sanddollar";
    basefolder /= targetfile.filename();
    return (basefolder);
}

int main(int argc, char *argv[])
{
    try
    {
        std::vector<std::string> target;

        parse_input(argc, argv, &target);
        fs::path targetfile = target.front();
        fs::path compiled = get_cached_script(targetfile);
        bool compile = false;
        if (!fs::exists(compiled))
            compile = true;
        else if (fs::last_write_time(targetfile) > fs::last_write_time(compiled))
            compile = true;
        if (compile)
        {
            scriptdir = fs::absolute(targetfile).remove_filename();
            fs::path cachepath = get_cache_path(targetfile);
            fs::path converted = convertscript(targetfile, cachepath);
            compiled = compilescript(converted, cachepath);
        }
        target.erase(target.begin());
        return runscript(compiled, target);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}
