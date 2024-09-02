/********************************************
 * Filename: ffmpeg_reader.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include <system_error>
#include "ghc/filesystem.hpp"
#include "common/log.h"
#include "reader/ffmpeg_reader.h"

namespace AVCPP_EXAMPLE
{

    static std::once_flag s_flag;
    void avcppInit()
    {
        std::call_once(s_flag, [&]() {
            av::init();
            av::setFFmpegLoggingLevel(AV_LOG_DEBUG);
        });
    }

    // https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/pixdesc.c
    static std::map<std::string, FrameDataFormat> gAVPixelFormatToFrameDataFormat = {
        {"bgr24",                                   FRAME_DATA_FORMAT_BGR},
        {"rgb24",                                   FRAME_DATA_FORMAT_RGB},
        {"gray",                                    FRAME_DATA_FORMAT_GRAY},
        {"nv12",                                    FRAME_DATA_FORMAT_NV12},
        {"nv21",                                    FRAME_DATA_FORMAT_NV21},
        {"bgra",                                    FRAME_DATA_FORMAT_BGRA},
    };

    static int convertAVPixelFormatToFrameDataFormat(const std::string av_format, FrameDataFormat& frame_format)
    {
        if (gAVPixelFormatToFrameDataFormat.end() == gAVPixelFormatToFrameDataFormat.find(av_format))
            return -1;
        frame_format = gAVPixelFormatToFrameDataFormat[av_format];
        return 0;
    }

    FFmpegReader::FFmpegReader(const ReaderConfig& config) : BaseReader(config)
    {
        // try to init avcpp first
        avcppInit();
        m_config = config.ffmpeg_config;
        m_status = (0 == initWithConfig(m_config)) ? true : false;
    }

    FFmpegReader::~FFmpegReader()
    {
        m_ictx.reset();
        m_vdec.reset();
    }

    int FFmpegReader::initWithVideoUri(const std::string video_uri, const std::string decode_codec)
    {
        if (true == ghc::filesystem::exists(video_uri))
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_INFO, "try to init reader from video file {}", video_uri);
            m_is_video_file = true;
        }
        else
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_INFO, "try to init reader from video url {}", video_uri);
            m_is_video_file = false;
        }
        m_ictx.reset(new av::FormatContext());
        if (nullptr == m_ictx)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "create video input context fail");
            return -1;
        }

        std::error_code ec;
        m_ictx->openInput(video_uri, ec);
        if (ec)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "cannot open video uri:{}", video_uri);
            return -1;
        }
        AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_INFO, "video uri:{} with streams count:{}", video_uri, m_ictx->streamsCount());

        m_ictx->findStreamInfo(ec);
        if (ec)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "cannot find streams for video uri:{}, ec:{}, {}", 
                video_uri, ec.value(), ec.message());
            return -1;
        }

        av::Stream video_st;
        for (size_t i = 0; i < m_ictx->streamsCount(); ++i)
        {
            auto st = m_ictx->stream(i);
            if (st.mediaType() == AVMEDIA_TYPE_VIDEO)
            {
                m_video_stream = i;
                video_st = st;
                break;
            }
        }

        if (video_st.isNull())
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "cannot find stream for video uri:{}", video_uri);
            return -1;
        }

        if (video_st.isValid())
        {
            m_vdec.reset(new av::VideoDecoderContext(video_st));
            if (nullptr == m_vdec.get())
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "create video decode context fail");
                return -1;
            }
            av::Codec codec;
            if ("" != decode_codec)
                codec = av::findDecodingCodec(decode_codec);
            else
                codec = av::findDecodingCodec(m_vdec->raw()->codec_id);
            m_vdec->setCodec(codec);
            m_vdec->setRefCountedFrames(true);
            m_vdec->open(av::Codec(), ec);
            if (ec)
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "cannot open codec for video uri:{}, ec:{}, {}", 
                    video_uri, ec.value(), ec.message());
                return -1;
            }
        }
        return 0;
    }

    int FFmpegReader::initAvRescaler(const std::string rescaler_format)
    {
        m_rescaler.reset(new av::VideoRescaler(m_vdec->width(), m_vdec->height(), av::PixelFormat(rescaler_format), 
            m_vdec->width(), m_vdec->height(), m_vdec->pixelFormat()));

        if (nullptr == m_rescaler.get() || false == m_rescaler->isValid())
            return -1;
        return 0;
    }

    int FFmpegReader::initWithConfig(const FFmpegReaderConfig& config)
    {
        std::string video_uri = config.video_uri;
        std::string decode_codec = config.decode_codec;
        if (0 != initWithVideoUri(video_uri, decode_codec))
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "init with video uri:{} decode_codec:{} fail", 
                video_uri, decode_codec);
            return -1;
        }
        std::string rescaler_format = m_config.rescaler_format;
        if ("" != rescaler_format && 0 != strcmp(av::PixelFormat(rescaler_format).name(), m_vdec->pixelFormat().name()) && 
            0 != initAvRescaler(rescaler_format))
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "init rescaler with format:{} fail", rescaler_format);
            return -1;
        }
        return 0;        
    }

    int FFmpegReader::convertVideoFrameToFrameMetaData(av::VideoFrame& video_frame, std::unique_ptr<FrameMetaData>& frame)
    {
        std::error_code ec;
        int frame_h = video_frame.height();
        int frame_w = video_frame.width();
        auto frame_size = video_frame.bufferSize();
        auto av_format = video_frame.pixelFormat().name(ec);
        const int gray_ch = 1;
        const int color_ch = 3;

        FrameDataType frame_type = FRAME_DATA_TYPE_UINT8;
        std::vector<int64_t> frame_shape = {frame_h, frame_w};
        FrameDataFormat frame_format;
        if (0 != convertAVPixelFormatToFrameDataFormat(av_format, frame_format))
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "av pixel format:{} cannot find matched frame format",
                av_format);
            return -1;
        }
        switch (frame_format)
        {
            case FRAME_DATA_FORMAT_GRAY:
            {
                break;
            }
            case FRAME_DATA_FORMAT_RGB:
            case FRAME_DATA_FORMAT_BGR:
            {
                frame_shape.push_back(color_ch);
                break;
            }
            default:
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "current not support frame format:{}", int(frame_format));
                return -1;
            }
        }

        frame.reset(FrameMetaData::create(frame_shape, frame_format, frame_type));
        if (nullptr == frame || nullptr == frame->data<void>())
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "create frame is nullptr or frame data is nullptr");
            return -1;
        }
        if (false == video_frame.copyToBuffer(frame->data<uint8_t>(), frame_size, 1, ec))
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "copy video buffer to frame meta data fail, ec:{}, {}", 
                ec.value(), ec.message());
            return -1;
        }
        return 0;
    }

    int FFmpegReader::getFrame(std::shared_ptr<BaseMetaData>& frame)
    {
        if (false == m_status)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "ffmpeg reader init fail");
            return -1;
        }

        std::error_code ec;
        av::Packet pkt = m_ictx->readPacket(ec);
        if (ec)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "packet reading error, ec:{}, {}", ec.value(), ec.message());
            return -1;
        }
        if (pkt.streamIndex() != m_video_stream)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "packet's video stream index:{} is not match with init value:{}", 
                pkt.streamIndex(), m_video_stream);
            return -1;
        }

        auto ts = pkt.ts();
        AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_DEBUG, "read packet: {} / {} / {} / st:{}", ts.timestamp(), ts.seconds(), 
            pkt.timeBase().getDouble(), pkt.streamIndex());

        av::VideoFrame decode_frame = m_vdec->decode(pkt, ec);
        if (ec)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "decode packet with error, ec:{}, {}", ec.value(), ec.message());
            return -1;
        }
        else if (!decode_frame)
        {
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_DEBUG, "decode packet get empty frame");
            if (m_cycle && m_is_video_file)
            {
                AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_INFO, "reach end of video file, try to seek the begin of video file");
                m_ictx->seek({0, {1,1}}, ec);
                if (ec)
                {
                    AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "try to seek the begin of video file fail, ec:{}, {}", 
                        ec.value(), ec.message());
                    return -1;
                }
            }
        }
        else
        {
            auto ts = decode_frame.pts();
            AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_INFO, "frame: {}x{}, size={}, ts={}, tm={}, tb={}, ref={}:{}", 
                decode_frame.width(), decode_frame.height(), decode_frame.size(), ts.timestamp(), ts.seconds(), 
                decode_frame.timeBase().getDouble(), decode_frame.isReferenced(), decode_frame.refCount());

            std::unique_ptr<FrameMetaData> frame_ptr;
            if (m_rescaler->isValid())
            {
                av::VideoFrame rescaler_frame = m_rescaler->rescale(decode_frame, ec);
                if (ec)
                {
                    AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "rescale frame {}x{} format:{} to {}x{} format{} error, ec:{}, {}", 
                        decode_frame.height(), decode_frame.width(), decode_frame.pixelFormat().name(), 
                        m_rescaler->dstHeight(), m_rescaler->dstWidth(), m_rescaler->dstPixelFormat().name(), 
                        ec.value(), ec.message());
                    return -1;
                }
                if (0 != convertVideoFrameToFrameMetaData(rescaler_frame, frame_ptr) || nullptr == frame_ptr)
                {
                    AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "create frame metadata from video rescale frame fail");
                    return -1;
                }
            }
            else
            {
                if (0 != convertVideoFrameToFrameMetaData(decode_frame, frame_ptr) || nullptr == frame_ptr)
                {
                    AVCPP_EXAMPLE_LOG(AVCPP_EXAMPLE_LOG_LEVEL_ERROR, "create frame metadata from video decode frame fail");
                    return -1;
                }
            }

            frame.reset(frame_ptr.release());
            m_frame_index += 1;
        }
        return 0;
    }

} // namespace AVCPP_EXAMPLE