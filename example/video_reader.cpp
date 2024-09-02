/********************************************
 * Filename: video_reader.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include "cxxopts.hpp"
#include "reader/ffmpeg_reader.h"

using namespace AVCPP_EXAMPLE;

void addFrameAndSourceInfo(std::shared_ptr<BaseMetaData>& frame_metadata)
{
    FrameMetaData* frame = (FrameMetaData*)frame_metadata.get();
    if (m_reader->isFirstFrame())
        frame->setFirstFrameFlag();
    if (m_reader->isLastFrame())
        frame->setFirstFrameFlag();
    frame->setFrameNum(m_reader->getFrameIndex());

    frame->setSourceName(m_name);
    frame->setSouceId(m_id);
    return;
}

int main(int argc, char* argv[])
{
    std::string log_path;
    int log_level;
    bool cycle_flag = false;
    std::string video_path;
    std::string codec;
    std::string rescaler_fromat;
    ReaderConfig reader_config;
   // 1 parse arg
    cxxopts::Options arg_options("./video_reader", "video reader app");
    arg_options.add_options()
        ("log_path", "video reader app log path", 
            cxxopts::value<std::string>()->default_value("./video_reader.log"))
        ("log_level", "video reader app log level, for example: TRACE = 0, DEBUG = 1 INFO = 2, WARN = 3, ERROR = 4, FATAL = 5,",
            cxxopts::value<int>()->default_value("2"))
        ("cycle_flag", "video reader app cycle flag")
        ("video_path", "video reader app video path", cxxopts::value<std::string>()->default_value("./test.avi"))
        ("codec", "video reader app codec", cxxopts::value<std::string>()->default_value("/oem/etc/iqfiles/"))
        ("rescaler_format", "video reader app rescaler format", cxxopts::value<std::string>()->default_value("0"))
        // help
        ("help", "print usage");
    arg_options.allow_unrecognised_options();
    auto parse_result = arg_options.parse(argc, argv);
    if (parse_result.count("help"))
    {
        std::cout << arg_options.help() << std::endl;
        return -1;
    }
    log_path = parse_result["log_path"].as<std::string>();
    log_level = parse_result["log_level"].as<int>();
    video_path = parse_result["video_path"].as<std::string>();
    codec = parse_result["codec"].as<std::string>();
    rescaler_fromat = parse_result["rescaler_fromat"].as<std::string>();
    if (parse_result.count("cycle_flag"))
        cycle_flag = true;

    std::cout << "parsed args as follows:" << std::endl;
    std::cout << "              log path:" << log_path << std::endl;
    std::cout << "             log level:" << log_level << std::endl;
    std::cout << "            cycle flag:" << cycle_flag << std::endl;
    std::cout << "            video path:" << video_path << std::endl;
    std::cout << "                 codec:" << codec << std::endl;
    std::cout << "       rescaler format:" << rescaler_fromat << std::endl;

    reader_config.cycle_flag = cycle_flag;
    reader_config.image_h = -1;
    reader_config.image_w = -1;
    reader_config.reader_type = FFMPEG_READER;
    reader_config.ffmpeg_config.decode_codec = codec;
    reader_config.ffmpeg_config.rescaler_format = rescaler_fromat;
    reader_config.ffmpeg_config.video_uri = video_path;

    std::unique_ptr<BaseReader> reader(new FFmpegReader(reader_config));
    if (nullptr == reader.get())
    {
        std::cout << arg_options.help() << std::endl;
        return -1;
    }

    while (true)
    {
        std::shared_ptr<BaseMetaData> frame_metadata;
        if (0 != reader->getFrame(frame_metadata))
        {
            std::cout << "reader get frame fail" << std::endl;
            break;
        }
        if (nullptr != frame_metadata.get())
        {
            addFrameAndSourceInfo(frame_metadata);
        }
    }
    return 0;
}