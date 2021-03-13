/*   Copyright 2021 Paul Buxton

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "sanddollar.h"
#define VERSION 1.1

namespace po = boost::program_options;
namespace fs = boost::filesystem;

fs::path scriptdir;

fs::path convertscript(const fs::path &inputfile, const fs::path &cachepath, const po::variables_map &vm)
{
    if (!fs::exists(inputfile) || !fs::is_regular_file(inputfile))
        throw std::invalid_argument("Inputfile not  found");
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

fs::path compilescript(const fs::path &inputfile, const fs::path &cachepath, const po::variables_map &vm)
{
    std::stringstream invoke;
    invoke << "g++-9 -o ";
    if (vm.count("debug")){
        invoke << (cachepath / "debuggable").string();
        invoke << " -g ";
    }
    else
    {
        invoke << (cachepath / "runable").string();
    }
    invoke << " --std=c++17 -lstdc++fs -I " << scriptdir.string() << " " << inputfile.string();
    std::string s = invoke.str();
    int result = std::system(s.c_str());
    if (result)
        throw std::invalid_argument("Script file is invalid");
    
    if (vm.count("debug"))
        return cachepath / "debuggable";
    return cachepath / "runable";
        
}

int runscript(const fs::path &compiledscript, const std::vector<std::string> &params,  const po::variables_map &vm )
{
    std::stringstream invoke;
    if (vm.count("debug")){
        invoke <<"gdb -ex=\"start\" --silent --tui --args ";
    }
    invoke << compiledscript.string();
    for (auto parm : params)
        invoke << " " << parm;
    std::string s = invoke.str();
    int result = std::system(s.c_str());
    return result;
}

int parse_input(const int argc, const char *argv[], std::vector<std::string> *target, po::variables_map *vm)
{
    po::options_description general("Options");

    general.add_options()("help,h", "Display this help message") \
    ("version,v", "Display the version number")\
    ("check,c", "Check the script without running")\
    ("debug,d", "Compile/run with debug")\
    ("force,f", "Force recompilation");

    po::options_description hidden("Hidden options");
    hidden.add_options()("infile,i", po::value<std::vector<std::string>>(target), "Input File");

    po::options_description cmdline("Commandline");

    po::positional_options_description positional;
    positional.add("infile", -1);

    cmdline.add(general).add(hidden);

    po::store(po::command_line_parser(argc, argv).options(cmdline).positional(positional).allow_unregistered().run(), *vm);
    po::notify(*vm);
    if (vm->count("version"))
    {
        std::cout << "Version number :" << VERSION << std::endl;
    }

    if (!vm->count("infile") || (vm->count("help")))
    {
        std::cout << "Usage:" << std::endl
                  << "sanddollar [options] <inputfile>" << std::endl;
        std::cout << general;
        return -1;
    }
    return 0;
}

fs::path get_cached_script(const fs::path &targetfile, const po::variables_map &vm  )
{
    if (vm.count("debug"))
    {
        return  get_cache_path(targetfile) / "debuggable";
    }
    else
    {
        return get_cache_path(targetfile) / "runable";
    }
}

fs::path get_cache_path(const fs::path &targetfile)
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
        po::variables_map vm;

        if (parse_input(argc, (const char **)argv, &target, &vm))
            return 0;
        fs::path targetfile = target.front();
        fs::path compiled = get_cached_script(targetfile, vm);
        bool compile = vm.count("force") ? true : false;
        bool run = vm.count("check") ? false : true;

        if (!fs::exists(compiled))
            compile = true;
        else if (fs::last_write_time(targetfile) > fs::last_write_time(compiled))
            compile = true;
        if (compile)
        {
            scriptdir = fs::absolute(targetfile).remove_filename();
            fs::path cachepath = get_cache_path(targetfile);
            fs::path converted = convertscript(targetfile, cachepath, vm);
            compiled = compilescript(converted, cachepath, vm);
        }
        if (!run)
            return 0;
        target.erase(target.begin());
        return runscript(compiled, target, vm);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}
