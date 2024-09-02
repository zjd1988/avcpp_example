/********************************************
 * Filename: base_reader.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "common/non_copyable.h"
#include "config/reader_config.h"
#include "metadata/frame_metadata.h"

namespace AVCPP_EXAMPLE
{

    #define MAX_FRAME_COUNT 0xFFFFFFFFFFFFFFFF

    class BaseReader : public NonCopyable
    {
    public:
        BaseReader(const ReaderConfig& config) {
            m_type = config.reader_type;
            m_cycle = config.cycle_flag;
            m_img_h = config.image_h;
            m_img_w = config.image_w;
            m_frame_index = 0;
            m_frame_count = MAX_FRAME_COUNT;
        }
        virtual ~BaseReader() = default;
        bool status() { return m_status; }
        ReaderType getReaderType() { return m_type; }

        virtual int getFrame(std::shared_ptr<BaseMetaData>& frame) = 0;
        bool isFirstFrame() { return m_frame_index == 1; }
        bool isLastFrame() { return (m_frame_index == m_frame_count) && (false == m_cycle); }
        uint64_t getFrameIndex() { return m_frame_index; }

    protected:
        ReaderType                        m_type;
        bool                              m_status = false;
        bool                              m_cycle = false;
        int32_t                           m_img_h = -1;
        int32_t                           m_img_w = -1;
        uint64_t                          m_frame_index;
        uint64_t                          m_frame_count;
    };

} // namespace AVCPP_EXAMPLE