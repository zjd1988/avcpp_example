/********************************************
 * Filename: reader_config.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include <stdint.h>
#include <map>
#include <iostream>
#include <vector>
#include <string>

namespace AVCPP_EXAMPLE
{

    typedef enum ReaderInputType
    {
        READER_INPUT_IMAGE                      = 0,
        READER_INPUT_VIDEO                      = 1,
        READER_INPUT_TENSOR                     = 2,
    } ReaderInputType;

    // ffmpeg reader config
    typedef struct FFmpegReaderConfig
    {
        std::string                             video_uri;        // video uri(video file path/stream url/camera)
        std::string                             decode_codec;
        std::string                             rescaler_format;  // convert origin format to dst format
    } FFmpegReaderConfig;

    // reader type enum
    typedef enum ReaderType
    {
        FFMPEG_READER                           = 0,
        GSTREAMER_READER                        = 1,
        OPENCV_READER                           = 2,
        STB_READER                              = 3,
        NUMPY_READER                            = 4,
    } ReaderType;

    // reader summary config 
    typedef struct ReaderConfig
    {
        ReaderType                              reader_type;
        bool                                    cycle_flag = false;
        int32_t                                 image_h = -1; // -1 for orgin image height
        int32_t                                 image_w = -1; // -1 for orgin image width
        FFmpegReaderConfig                      ffmpeg_config;
    } ReaderConfig;

    // string -> reader input type / reader input type -> string
    extern std::map<std::string, ReaderInputType> gStrToReaderInputType;
    extern std::map<ReaderInputType, std::string> gReaderInputTypeToStr;

    // string -> reader type / reader type -> string
    extern std::map<std::string, ReaderType> gStrToReaderType;
    extern std::map<ReaderType, std::string> gReaderTypeToStr;

    // get reader default config by reader type
    int getDefaultReaderConfig(ReaderConfig& reader_config, const ReaderType reader_type);

    // convert str -> reader type func / reader type -> string func
    int convertStrToReaderInputType(const std::string type_str, ReaderInputType& reader_type);
    int convertReaderInputTypeToStr(const ReaderInputType reader_type, std::string& type_str);

    // convert str -> reader type func / reader type -> string func
    int convertStrToReaderType(const std::string type_str, ReaderType& reader_type);
    int convertReaderTypeToStr(const ReaderType reader_type, std::string& type_str);

} // namespace AVCPP_EXAMPLE