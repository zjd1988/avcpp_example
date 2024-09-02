/********************************************
 * Filename: ffmpeg_reader.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include "av.h"
#include "ffmpeg.h"
#include "codec.h"
#include "packet.h"
#include "videorescaler.h"
#include "audioresampler.h"
#include "avutils.h"

// API2
#include "format.h"
#include "formatcontext.h"
#include "codec.h"
#include "codeccontext.h"

#include "metadata/frame_metadata.h"
#include "reader/base_reader.h"

namespace AVCPP_EXAMPLE
{

    class FFmpegReader : public BaseReader
    {
    public:
        FFmpegReader(const ReaderConfig& config);
        ~FFmpegReader();

        virtual int getFrame(std::shared_ptr<BaseMetaData>& frame) override;

    private:
        int convertVideoFrameToFrameMetaData(av::VideoFrame& video_frame, std::unique_ptr<FrameMetaData>& frame);
        int initWithVideoUri(const std::string video_uri, const std::string decode_codec);
        int initAvRescaler(const std::string rescaler_format);
        int initWithConfig(const FFmpegReaderConfig& config);

    private:
        FFmpegReaderConfig                                          m_config;
        bool                                                        m_is_video_file;
        // for video input
        ssize_t                                                     m_video_stream = -1;
        std::shared_ptr<av::VideoDecoderContext>                    m_vdec;
        std::shared_ptr<av::FormatContext>                          m_ictx;
        std::shared_ptr<av::VideoRescaler>                          m_rescaler;
    };

} // namespace AVCPP_EXAMPLE