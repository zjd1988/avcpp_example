/********************************************
 * Filename: video_reader.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include "cxxopts.hpp"
#include "reader/ffmpeg_reader.h"

using namespace AVCPP_EXAMPLE;

int main(int argc, char* argv[])
{
    std::string log_path;
    int log_level;
    bool cycle_flag;
    std::string video_path;
   // 1 parse arg
    cxxopts::Options arg_options("./video_reader", "video reader app");
    arg_options.add_options()
        ("log_path", "video reader app log path", 
            cxxopts::value<std::string>()->default_value("./video_reader.log"))
        ("log_level", "video reader app log level, for example: TRACE = 0, DEBUG = 1 INFO = 2, WARN = 3, ERROR = 4, FATAL = 5,",
            cxxopts::value<int>()->default_value("2"))
        ("cycle_flag", "video reader app cycle flag")
        ("video_path", "video reader app video path", cxxopts::value<std::string>()->default_value("./rtsp_config.cfg"))
        ("codec", "video reader app codec", cxxopts::value<std::string>()->default_value("/oem/etc/iqfiles/"))
        ("rescaler_format", "video reader app rescaler format", cxxopts::value<std::string>()->default_value("0"))
        // help
        ("help", "print usage");
    arg_options.allow_unrecognised_options();
    auto parse_result = arg_options.parse(argc, argv);


    std::unique_ptr<BaseReader> m_reader;
    return 0;
}