/********************************************
 * Filename: base_metadata.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include <map>
#include <string>
#include <ctime>
#include <iomanip>
#include "metadata/base_metadata.h"

namespace AVCPP_EXAMPLE
{

    std::string BaseMetaData::getTimeStampStr() const 
    {
        std::stringstream ss;
        time_t time_sec = m_time_stamp / 1000;
        ss << std::put_time(std::localtime(&time_sec), "%F %X") << ":" << m_time_stamp % 1000;
        // ss << std::put_time(std::localtime(&time_sec), "%Y-%m-%d %H:%M:%S") << ":" << m_time_stamp % 1000;
        return ss.str();
    }

} // namespace AVCPP_EXAMPLE