/********************************************
 * Filename: frame_metadata.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include "metadata/base_metadata.h"
#include "config/metadata_config.h"

namespace AVCPP_EXAMPLE
{

    class FrameMetaData : public BaseMetaData
    {
    public:
        FrameMetaData(const std::vector<int64_t> shape, const FrameDataFormat format = FRAME_DATA_FORMAT_BGR, 
            const FrameDataType dtype = FRAME_DATA_TYPE_UINT8, void* user_data = nullptr);
        ~FrameMetaData();
        static FrameMetaData* create(const std::vector<int64_t>& shape, const FrameDataFormat format, 
            const FrameDataType dtype, void* data = nullptr);
        static FrameMetaData* create(const int32_t height, const int32_t width, const int32_t channel, 
            const FrameDataFormat format, const FrameDataType dtype, void* data = nullptr);
        std::vector<int64_t> shape() const { return m_shape; }
        FrameDataType getFrameDataType() const { return m_dtype; };
        FrameDataFormat getFrameDataFormat() const { return m_format; };
        virtual BaseMetaData* clone() override;

        template <typename T>
        T* data() const { return (T*)m_data; }

        size_t elementBytes();
        size_t size();

        int32_t height();
        int32_t width();

        // set frame source info
        const uint32_t getSourceId() { return m_source_id; };
        void setSouceId(const uint32_t id) { m_source_id = id; };
        const std::string getSourceName() { return m_source_name; };
        void setSourceName(const std::string name) { m_source_name = name; };
        const uint64_t getFrameNum() { return m_frame_num; };
        void setFrameNum(const uint64_t number) { m_frame_num = number; };

        // set/get first/last frame flag
        bool getFirstFrameFlag() { return m_first_flag; };
        void setFirstFrameFlag() { m_first_flag = true; };
        bool getLastFrameFlag() { return m_last_flag; };
        void setLastFrameFlag() { m_last_flag = true; };

    private:
        uint32_t                                m_source_id;
        std::string                             m_source_name;
        uint64_t                                m_frame_num;

        bool                                    m_first_flag;
        bool                                    m_last_flag;

        FrameDataFormat                         m_format;
        FrameDataType                           m_dtype;
        std::vector<int64_t>                    m_shape;

        void*                                   m_data;
        bool                                    m_own_flag;
    };

} // namespace AVCPP_EXAMPLE