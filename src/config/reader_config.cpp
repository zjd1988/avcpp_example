/********************************************
 * Filename: reader_config.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include "common/log.h"
#include "config/reader_config.h"

namespace AVCPP_EXAMPLE
{

    // string -> reader input type / reader input type -> string
    std::map<std::string, ReaderInputType> gStrToReaderInputType = {
        {"image",                            READER_INPUT_IMAGE},
        {"video",                            READER_INPUT_VIDEO},
        {"tensor",                           READER_INPUT_TENSOR},
    };

    std::map<ReaderInputType, std::string> gReaderInputTypeToStr = {
        {READER_INPUT_IMAGE,                 "image"},
        {READER_INPUT_VIDEO,                 "video"},
        {READER_INPUT_TENSOR,                "tensor"},
    };

    // string to reader type map
    std::map<std::string, ReaderType> gStrToReaderType = {
        {"ffmpeg",                           FFMPEG_READER},
        {"gstreamer",                        GSTREAMER_READER},
        {"opencv",                           OPENCV_READER},
        {"stb",                              STB_READER},
        {"numpy",                            NUMPY_READER},
    };

    // reader type to string map
    std::map<ReaderType, std::string> gReaderTypeToStr = {
        {FFMPEG_READER,                      "ffmpeg"},
        {GSTREAMER_READER,                   "gstreamer"},
        {OPENCV_READER,                      "opencv"},
        {STB_READER,                         "stb"},
        {NUMPY_READER,                       "numpy"},
    };

    void getFFmpegReaderDefaultConfig(ReaderConfig& reader_config)
    {
        reader_config.ffmpeg_config.decode_codec = "";
        reader_config.ffmpeg_config.video_uri = "";
        reader_config.ffmpeg_config.rescaler_format = "bgr24";
        return;
    }

    // get reader default config by reader type
    int getDefaultReaderConfig(ReaderConfig& reader_config, const ReaderType reader_type)
    {
        reader_config.reader_type = reader_type;
        reader_config.image_h = -1;
        reader_config.image_w = -1;
        reader_config.cycle_flag = false;
        switch (reader_type)
        {
            case FFMPEG_READER:
            {
                getFFmpegReaderDefaultConfig(reader_config);
                break;
            }
            default:
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "current not support reader type:{}", int(reader_type));
                return -1;
            }
        }
        return 0;
    }

    // convert str -> reader input type func / reader input type -> string func
    int convertStrToReaderInputType(const std::string type_str, ReaderInputType& input_type)
    {
        if (gStrToReaderInputType.end() == gStrToReaderInputType.find(type_str))
            return -1;

        input_type = gStrToReaderInputType[type_str];
        return 0;
    }

    int convertReaderInputTypeToStr(const ReaderInputType input_type, std::string& type_str)
    {
        if (gReaderInputTypeToStr.end() == gReaderInputTypeToStr.find(input_type))
            return -1;

        type_str = gReaderInputTypeToStr[input_type];
        return 0;
    }

    // convert str -> reader type func / reader type -> string func
    int convertStrToReaderType(const std::string type_str, ReaderType& reader_type)
    {
        if (gStrToReaderType.end() == gStrToReaderType.find(type_str))
            return -1;

        reader_type = gStrToReaderType[type_str];
        return 0;
    }

    int convertReaderTypeToStr(const ReaderType reader_type, std::string& type_str)
    {
        if (gReaderTypeToStr.end() == gReaderTypeToStr.find(reader_type))
            return -1;

        type_str = gReaderTypeToStr[reader_type];
        return 0;
    }

} // namespace AVCPP_EXAMPLE