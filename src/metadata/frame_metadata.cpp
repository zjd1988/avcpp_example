/********************************************
 * Filename: frame_metadata.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include <map>
#include <string>
#include "common/memory_utils.h"
#include "metadata/frame_metadata.h"

namespace AVCPP_EXAMPLE
{

    FrameMetaData::FrameMetaData(const std::vector<int64_t> shape, const FrameDataFormat format, 
        const FrameDataType dtype, void* user_data) : BaseMetaData(FRAME_METADATA)
    {
        m_dtype        = dtype;
        m_format       = format;
        m_shape        = shape;
        m_data         = nullptr;
        m_own_flag     = false;
        m_source_id    = 0xFFFFFFFF;
        m_source_name  = "";
        m_first_flag   = false;
        m_last_flag    = false;
        bool alloc_memory = (user_data == nullptr);
        if (alloc_memory)
        {
            auto memory_size = size();
            if (memory_size > 0)
            {
                m_data = (void*)memoryAllocAlign(size(), MEMORY_ALIGN_DEFAULT);
                if (nullptr == m_data)
                {
                    AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "memory alloc align fail");
                }
            }
            m_own_flag = true;
        }
        else
            m_data = user_data;
    }

    FrameMetaData::~FrameMetaData()
    {
        if (nullptr != m_data && true == m_own_flag)
        {
            memoryFreeAlign(m_data);
        }
    }

    FrameMetaData* FrameMetaData::create(const std::vector<int64_t>& shape, FrameDataFormat format, 
        FrameDataType dtype, void* data)
    {
        std::unique_ptr<FrameMetaData> frame(new FrameMetaData(shape, format, dtype, data));
        if (nullptr == frame.get() || nullptr == frame->data<void>())
            return nullptr;
        return frame.release();
    }

    FrameMetaData* FrameMetaData::create(const int32_t height, const int32_t width, const int32_t channel, 
        const FrameDataFormat format, const FrameDataType dtype, void* data)
    {
        std::vector<int64_t> shape = {int64_t(height), int64_t(width), int64_t(channel)};
        std::unique_ptr<FrameMetaData> frame(new FrameMetaData(shape, format, dtype, data));
        if (nullptr == frame.get() || nullptr == frame->data<void>())
            return nullptr;
        return frame.release();
    }

    size_t FrameMetaData::elementBytes()
    {
        if (m_dtype == FRAME_DATA_TYPE_INT8 || m_dtype == FRAME_DATA_TYPE_UINT8)
            return sizeof(uint8_t);
        else if (m_dtype == FRAME_DATA_TYPE_INT16 || m_dtype == FRAME_DATA_TYPE_UINT16)
            return sizeof(uint16_t);
        else if (m_dtype == FRAME_DATA_TYPE_INT32 || m_dtype == FRAME_DATA_TYPE_UINT32)
            return sizeof(uint32_t);
        else if (m_dtype == FRAME_DATA_TYPE_INT64 || m_dtype == FRAME_DATA_TYPE_UINT64)
            return sizeof(uint64_t);
        else if (m_dtype == FRAME_DATA_TYPE_FP32)
            return sizeof(float);
        else if (m_dtype == FRAME_DATA_TYPE_FP64)
            return sizeof(double);
        else
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "unspported data type: {}", int(m_dtype));
            return -1;
        }
    }

    size_t FrameMetaData::size()
    {
        auto data_size = elementBytes();
        if (0 >= data_size)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "frame meta element bytes {} is invalid", data_size);
            return 0;
        }
        for (auto i = 0; i < m_shape.size(); i++)
        {
            data_size *= m_shape[i];
        }
        if (FRAME_DATA_FORMAT_NV12 == m_format || FRAME_DATA_FORMAT_NV21 == m_format)
            data_size /= 2;
        return data_size;
    }

    int32_t FrameMetaData::height()
    {
        int32_t h = -1;
        switch (m_format)
        {
            case FRAME_DATA_FORMAT_BGR:
            case FRAME_DATA_FORMAT_RGB:
            case FRAME_DATA_FORMAT_GRAY:
            case FRAME_DATA_FORMAT_NV12:
            case FRAME_DATA_FORMAT_NV21:
            case FRAME_DATA_FORMAT_BGRA:
            {
                h = int32_t(m_shape[0]);
                break;
            }
            default:
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "frame metadata not a image");
                break;
            }
        }
        return h;
    }

    int32_t FrameMetaData::width()
    {
        int w = -1;
        switch (m_format)
        {
            case FRAME_DATA_FORMAT_BGR:
            case FRAME_DATA_FORMAT_RGB:
            case FRAME_DATA_FORMAT_GRAY:
            case FRAME_DATA_FORMAT_NV12:
            case FRAME_DATA_FORMAT_NV21:
            case FRAME_DATA_FORMAT_BGRA:
            {
                w = int32_t(m_shape[1]);
                break;
            }
            default:
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "frame metadata not a image");
                break;
            }
        }
        return w;
    }

    BaseMetaData* FrameMetaData::clone()
    {
        FrameDataType data_type = getFrameDataType();
        FrameDataFormat format_type = getFrameDataFormat();
        auto shape = this->shape();
        std::unique_ptr<FrameMetaData> frame_copy(FrameMetaData::create(shape, format_type, data_type));
        if (nullptr == frame_copy.get())
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "clone frame metadata fail");
            return nullptr;
        }
        auto size = this->size();
        memcpy(this->data<char*>(), frame_copy->data<char*>(), size);
        return frame_copy.release();
    }

} // namespace AVCPP_EXAMPLE