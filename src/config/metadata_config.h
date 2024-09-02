/********************************************
 * Filename: metadata_config.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include <stdint.h>
#include <map>
#include <vector>

namespace AVCPP_EXAMPLE
{

    typedef enum FrameDataFormat
    {
        FRAME_DATA_FORMAT_INVALID                                = -1,
        FRAME_DATA_FORMAT_BGR                                    = 0,
        FRAME_DATA_FORMAT_RGB                                    = 1,
        FRAME_DATA_FORMAT_GRAY                                   = 2,
        FRAME_DATA_FORMAT_NV12                                   = 3,
        FRAME_DATA_FORMAT_NV21                                   = 4,
        FRAME_DATA_FORMAT_BGRA                                   = 5,
        FRAME_DATA_FORMAT_BLOB                                   = 6,
    } FrameDataFormat;

    typedef enum FrameDataType
    {
        FRAME_DATA_TYPE_INVALID                                  = -1,
        FRAME_DATA_TYPE_UINT8                                    = 0,
        FRAME_DATA_TYPE_UINT16                                   = 1,
        FRAME_DATA_TYPE_UINT32                                   = 2,
        FRAME_DATA_TYPE_UINT64                                   = 3,
        FRAME_DATA_TYPE_INT8                                     = 4,
        FRAME_DATA_TYPE_INT16                                    = 5,
        FRAME_DATA_TYPE_INT32                                    = 6,
        FRAME_DATA_TYPE_INT64                                    = 7,
        FRAME_DATA_TYPE_FP16                                     = 8,
        FRAME_DATA_TYPE_FP32                                     = 9,
        FRAME_DATA_TYPE_FP64                                     = 10,
    } FrameDataType;

    typedef enum MetaDataType
    {
        TIMER_METADATA                                           = 0,
        FRAME_METADATA                                           = 1,
    } MetaDataType;

    // MetaDataType -> string / string -> MetaDataType map
    extern std::map<MetaDataType, std::string> gMetaDataTypeToStr;
    extern std::map<std::string, MetaDataType> gStrToMetaDataType;

    // str -> frame datatype / frame datatype -> str map
    extern std::map<std::string, FrameDataType> gStrToFrameDataType;
    extern std::map<FrameDataType, std::string> gFrameDataTypeToStr;

    // str -> frame data format / frame data format -> str map
    extern std::map<std::string, FrameDataFormat> gStrToFrameDataFormat;
    extern std::map<FrameDataFormat, std::string> gFrameDataFormatToStr;

    // MetaDataType -> string / string -> MetaDataType funcs
    int convertMetaDataTypeToStr(const MetaDataType metadata_type, std::string& type_str);
    int convertStrToMetaDataType(const std::string type_str, MetaDataType& metadata_type);

    // str -> frame datatype / frame datatype -> str map funcs
    int convertStrToFrameDataType(const std::string type_str, FrameDataType& frame_type);
    int convertFrameDataTypeToStr(const FrameDataType frame_type, std::string& type_str);

    // str -> frame data format / frame data format -> str map funcs
    int convertStrToFrameDataFormat(const std::string format_str, FrameDataFormat& frame_format);
    int convertFrameDataFormatToStr(const FrameDataFormat frame_format, std::string& format_str);

} // namespace AVCPP_EXAMPLE
