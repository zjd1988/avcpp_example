/********************************************
 * Filename: metadata_config.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include <map>
#include <string>
#include "config/metadata_config.h"

namespace AVCPP_EXAMPLE
{

    // MetaDataType -> string map / string -> MetaDataType map
    std::map<MetaDataType, std::string> gMetaDataTypeToStr = {
        {TIMER_METADATA,                                "timer_metadata"},
        {FRAME_METADATA,                                "frame_metadata"},
    };

    std::map<std::string, MetaDataType> gStrToMetaDataType = {
        {"timer_metadata",                              TIMER_METADATA},
        {"frame_metadata",                              FRAME_METADATA},
    };

    // str -> frame datatype / frame datatype -> str map
    std::map<std::string, FrameDataType> gStrToFrameDataType = {
        {"UINT8",                                       FRAME_DATA_TYPE_UINT8},
        {"UINT16",                                      FRAME_DATA_TYPE_UINT16},
        {"UINT32",                                      FRAME_DATA_TYPE_UINT32},
        {"UINT64",                                      FRAME_DATA_TYPE_UINT64},
        {"INT8",                                        FRAME_DATA_TYPE_INT8},
        {"INT16",                                       FRAME_DATA_TYPE_INT16},
        {"INT32",                                       FRAME_DATA_TYPE_INT32},
        {"INT64",                                       FRAME_DATA_TYPE_INT64},
        {"FP16",                                        FRAME_DATA_TYPE_INT16},
        {"FP32",                                        FRAME_DATA_TYPE_INT32},
        {"FP64",                                        FRAME_DATA_TYPE_INT64},
    };

    std::map<FrameDataType, std::string> gFrameDataTypeToStr = {
        {FRAME_DATA_TYPE_UINT8,                         "UINT8"},
        {FRAME_DATA_TYPE_UINT16,                        "UINT16"},
        {FRAME_DATA_TYPE_UINT32,                        "UINT32"},
        {FRAME_DATA_TYPE_UINT64,                        "UINT64"},
        {FRAME_DATA_TYPE_INT8,                          "INT8"},
        {FRAME_DATA_TYPE_INT16,                         "INT16"},
        {FRAME_DATA_TYPE_INT32,                         "INT32"},
        {FRAME_DATA_TYPE_INT64,                         "INT64"},
        {FRAME_DATA_TYPE_FP16,                          "FP16"},
        {FRAME_DATA_TYPE_FP32,                          "FP32"},
        {FRAME_DATA_TYPE_FP64,                          "FP64"},
    };

    // str -> frame data format / frame data format -> str map
    std::map<std::string, FrameDataFormat> gStrToFrameDataFormat = {
        {"BGR",                                         FRAME_DATA_FORMAT_BGR},
        {"RGB",                                         FRAME_DATA_FORMAT_RGB},
        {"GRAY",                                        FRAME_DATA_FORMAT_GRAY},
        {"NV12",                                        FRAME_DATA_FORMAT_NV12},
        {"NV21",                                        FRAME_DATA_FORMAT_NV21},
        {"BGRA",                                        FRAME_DATA_FORMAT_BGRA},
        {"BLOB",                                        FRAME_DATA_FORMAT_BLOB},
    };

    std::map<FrameDataFormat, std::string> gFrameDataFormatToStr = {
        {FRAME_DATA_FORMAT_BGR,                         "BGR"},
        {FRAME_DATA_FORMAT_RGB,                         "RGB"},
        {FRAME_DATA_FORMAT_GRAY,                        "GRAY"},
        {FRAME_DATA_FORMAT_NV12,                        "NV12"},
        {FRAME_DATA_FORMAT_NV21,                        "NV21"},
        {FRAME_DATA_FORMAT_BGRA,                        "BGRA"},
        {FRAME_DATA_FORMAT_BLOB,                        "BLOB"},
    };

    // MetaDataType -> string / string -> MetaDataType funcs
    int convertMetaDataTypeToStr(const MetaDataType metadata_type, std::string& type_str)
    {
        if (gMetaDataTypeToStr.end() == gMetaDataTypeToStr.find(metadata_type))
            return -1;

        type_str = gMetaDataTypeToStr[metadata_type];
        return 0;
    }
    int convertStrToMetaDataType(const std::string type_str, MetaDataType& metadata_type)
    {
        if (gStrToMetaDataType.end() == gStrToMetaDataType.find(type_str))
            return -1;

        metadata_type = gStrToMetaDataType[type_str];
        return 0;
    }


    // str -> frame datatype / frame datatype -> str map funcs
    int convertStrToFrameDataType(const std::string type_str, FrameDataType& frame_type)
    {
        if (gStrToFrameDataType.end() == gStrToFrameDataType.find(type_str))
            return -1;

        frame_type = gStrToFrameDataType[type_str];
        return 0;
    }

    int convertFrameDataTypeToStr(const FrameDataType frame_type, std::string& type_str)
    {
        if (gFrameDataTypeToStr.end() == gFrameDataTypeToStr.find(frame_type))
            return -1;

        type_str = gFrameDataTypeToStr[frame_type];
        return 0;
    }

    // str -> frame data format / frame data format -> str map funcs
    int convertStrToFrameDataFormat(const std::string format_str, FrameDataFormat& frame_format)
    {
        if (gStrToFrameDataFormat.end() == gStrToFrameDataFormat.find(format_str))
            return -1;

        frame_format = gStrToFrameDataFormat[format_str];
        return 0;
    }

    int convertFrameDataFormatToStr(const FrameDataFormat frame_format, std::string& format_str)
    {
        if (gFrameDataFormatToStr.end() == gFrameDataFormatToStr.find(frame_format))
            return -1;

        format_str = gFrameDataFormatToStr[frame_format];
        return 0;
    }

} // namespace AVCPP_EXAMPLE