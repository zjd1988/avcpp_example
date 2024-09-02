/********************************************
 * Filename: base_metadata.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>
#include "common/non_copyable.h"
#include "common/log.h"
#include "config/metadata_config.h"

namespace AVCPP_EXAMPLE
{

    class BaseMetaData : public NonCopyable
    {
    public:
        BaseMetaData(const MetaDataType& type)
        {
            m_type = type;
            auto now = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
            m_time_stamp = time.time_since_epoch().count();
            m_need_feedback = false;
        };
        virtual ~BaseMetaData() = default;

        MetaDataType getMetaDataType() const { return m_type; }
        void setSrcName(const std::string& source_name) { m_src = source_name; }
        void setDestName(const std::string& dest_name) { m_dest = dest_name; }
        const std::string getSrcName() const { return m_src; }
        const std::string getDestName() const { return m_dest; }
        std::string getTimeStampStr() const ;
        std::time_t getTimeStamp() const { return m_time_stamp; }
        bool getNeedFeedback() const { return m_need_feedback; }
        void setNeedFeedback(bool need_feedback) { m_need_feedback = need_feedback; }
        virtual BaseMetaData* clone() = 0;

    private:
        MetaDataType                           m_type;
        std::string                            m_src;
        std::string                            m_dest;
        std::time_t                            m_time_stamp;
        bool                                   m_need_feedback;
    };

} // namespace AVCPP_EXAMPLE