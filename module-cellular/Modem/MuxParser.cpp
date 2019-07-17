
/*
 * @file MuxParser.cpp
 * @author Mateusz Piesta (mateusz.piesta@mudita.com)
 * @date 17.07.19
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */


#include "MuxParser.hpp"

#include "log/log.hpp"
#include "MuxDaemon.hpp"
#include "MuxChannel.hpp"
#include "GSM0710.hpp"
#include "InOutSerialWorker.hpp"
#include "bsp/cellular/bsp_cellular.hpp"

std::optional<std::unique_ptr<MuxParser>> MuxParser::Create(MuxDaemon *mux, InOutSerialWorker *inOutSerial, bsp::Cellular *cellular) {
    auto inst = std::make_unique<MuxParser>(mux,inOutSerial,cellular);

    if(inst->isInitialized){
        return inst;
    }
    else{
        return {};
    }
}

MuxParser::MuxParser(MuxDaemon *mux, InOutSerialWorker *inOutSerial, bsp::Cellular *cellular):
mux(mux), inOutSerialWorker(inOutSerial),cellular(cellular){

    inputBuffer = std::make_unique<GSM0710Buffer>(mux->virtualPortsCount);
    if(inputBuffer == nullptr){
        LOG_ERROR("Failed to create GSM0710Buffer");
        return;
    }
    isInitialized = true;
}

ssize_t MuxParser::SendMuxFrame(int channel, const unsigned char *input, int length, unsigned char type) {
    /* flag, GSM0710_EA=1 C channel, frame type, length 1-2 */
    unsigned char prefix[5] = {static_cast<unsigned char>(MuxDefines::GSM0710_FRAME_FLAG),
                               static_cast<unsigned char>(MuxDefines::GSM0710_EA) |
                               static_cast<unsigned char>(MuxDefines::GSM0710_CR), 0, 0, 0};
    unsigned char postfix[2] = {0xFF, static_cast<unsigned char>(MuxDefines::GSM0710_FRAME_FLAG )};
    ssize_t prefix_length = 4;
    int c;
    unsigned char tmp[GSM0710Buffer::cmux_FRAME];

    LOG_DEBUG("Sending frame to channel %d", channel);

    /* GSM0710_EA=1, Command, let's add address */
    prefix[1] = prefix[1] | ((63 & (unsigned char) channel) << 2);

/* let's set control field */
    prefix[2] = type;
    if ((type == static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UIH) ||
         type == static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UI)) &&
        mux->uihPfBitReceived == 1 &&
        GSM0710Buffer::GSM0710_COMMAND_IS(MuxDefines::GSM0710_CONTROL_MSC, input[0])) {
        prefix[2] = prefix[2] |
                    static_cast<unsigned char>(MuxDefines::GSM0710_PF); //Set the P/F bit in Response if Command from modem had it set
        mux->uihPfBitReceived = 0; //Reset the variable, so it is ready for next command
    }
/* let's not use too big frames */
    length = std::min(GSM0710Buffer::cmux_N1, static_cast<uint32_t >(length));
    // Only basic mode is supported
    {
/* Modified acording PATCH CRC checksum */
/* postfix[0] = frame_calc_crc (prefix + 1, prefix_length - 1); */
/* length */
        if (length > 127) {
            prefix_length = 5;
            prefix[3] = (0x007F & length) << 1;
            prefix[4] = (0x7F80 & length) >> 7;
        } else {
            prefix[3] = 1 | (length << 1);
        }
        postfix[0] = GSM0710Buffer::frameCalcCRC(prefix + 1, prefix_length - 1);

        memcpy(tmp, prefix, prefix_length);

        if (length > 0) {
            memcpy(tmp + prefix_length, input, length);
        }

        memcpy(tmp + prefix_length + length, postfix, 2);
        c = prefix_length + length + 2;

        //Write newly created frame into serial output buffer
        inOutSerialWorker->WriteData(tmp, c);
    }

    return length;
}


int MuxParser::ProcessNewData() {
    if (ReadIncomingData() > 0) {
        if (inputBuffer->GetDataLength() > 0) {

            if (ExtractFrames()) {
                //TODO:M.P implement error handling ?
            }

            // Reorganize data in buffer if necessary
            inputBuffer->ReorganizeBuffer();
        }
    }
}


int MuxParser::ReadIncomingData() {
    int length = 0;

    switch (mux->GetState()) {
        case MuxDaemon::States::MUX_STATE_MUXING:
            length = std::min(inputBuffer->GetFreeSpace(),
                              (uint32_t) (inputBuffer->endp - inputBuffer->writep));

            if (length > 0) /*available space in buffer (not locked since we want to utilize all available space)*/
            {
                length = cellular->Read(inputBuffer->writep, length);
                if (length > 0) {
                    LOG_DEBUG("READ SIZE : %d", length);
                    //syslogdump("<s ", serial->in_buf->writep, length);
                    inputBuffer->writep += length;
                    if (inputBuffer->writep == inputBuffer->endp)
                        inputBuffer->writep = inputBuffer->data;

                    inputBuffer->datacount += length; /*updating the data-not-yet-read counter*/
                    LOG_DEBUG("GSM0710 buffer (up-to-date): written %d, free %d, stored %d",
                              length, inputBuffer->GetFreeSpace(), inputBuffer->GetDataLength());

                    if (length > inputBuffer->max_count) {
                        LOG_INFO("MAX READ SIZE : %d", length);
                        LOG_INFO("GSM0710 buffer (up-to-date): written %d, free %d, stored %d",
                                 length, inputBuffer->GetFreeSpace(),
                                 inputBuffer->GetDataLength());
                        inputBuffer->max_count = length;
                    }
                } else {
                    LOG_INFO("READ SIZE : %d", length);
                }
            } else {
                LOG_DEBUG("Not enough space in buffer");
            }
            break;

        default:
            LOG_WARN("Don't know how to handle reading in state %d", mux->GetState());
            break;
    }

    return length;
}

int MuxParser::ExtractFrames() {
    int frames_extracted = 0;
    GSM0710Frame local_frame;
    GSM0710Frame *frame = &local_frame;


    while (inputBuffer->GetCompleteFrame(frame) != NULL) {
        frames_extracted++;
        auto muxChannel = static_cast<MuxChannel::MuxChannelType>(frame->channel);
        auto muxChannelInst = mux->GetMuxChannel(muxChannel).value_or(nullptr);
        if(muxChannelInst == nullptr){
            LOG_ERROR("Invalid mux channel");
            continue;
        }

        if ((inputBuffer->GSM0710_FRAME_IS(MuxDefines::GSM0710_TYPE_UI, frame) ||
             inputBuffer->GSM0710_FRAME_IS(MuxDefines::GSM0710_TYPE_UIH, frame))) {
            LOG_DEBUG("Frame is UI or UIH");
            if (frame->control & static_cast<unsigned char>(MuxDefines::GSM0710_PF)) {
                mux->uihPfBitReceived = 1;
            }

            //data from logical channel
            if (frame->channel > 0) {
                LOG_DEBUG("Writing %d byte frame received on channel %d to %s", frame->length, frame->channel,
                          muxChannelInst->GetName().c_str());

                int write_result = 0;

                if (muxChannelInst->GetState() ==
                    MuxChannel::State::Opened) {//reopening, discard the data

                    // Send received message to virtual channel for further processing
                    write_result = muxChannelInst->SendData(frame->data, frame->length);
                } else {
                    LOG_INFO("channel %d closed, discard the frame", frame->channel);
                    write_result = frame->length;
                }

                if (write_result < frame->length) {
                    LOG_INFO("write() returned. Written %d/%d bytes of frame to %s",
                             write_result, frame->length, muxChannelInst->GetName().c_str());

                    //send_modem_status(frame->channel,1);
                    //fill_in_list(frame->channel,frame->data + write_result,frame->length - write_result);
                    //MSC_flag |=  1 << (frame->channel - 1);
                }
            } else {
                //control channel command
                LOG_DEBUG("Frame channel == 0, control channel command");
                HandleCtrlChannelCommands(frame);
            }
        } else {
            //not an information frame
            LOG_DEBUG("Not an information frame");
            MuxDefines frameType = static_cast<MuxDefines >(frame->control &
                                                            ~static_cast<unsigned char>(MuxDefines::GSM0710_PF));
            switch (frameType) {
                case MuxDefines::GSM0710_TYPE_UA:
                    LOG_DEBUG("Frame is UA");
                    if (muxChannelInst->GetState() == MuxChannel::State::Opened) {
                        LOG_INFO("Logical channel %d for %s closed",
                                 frame->channel, muxChannelInst->GetName().c_str());
                        mux->RemoveMuxChannel(muxChannel);

                    } else {
                        if (muxChannelInst->disc_ua_pending == 0) {
                            muxChannelInst->SetState(MuxChannel::State::Opened);
                            if (frame->channel == 0) {
                                LOG_DEBUG("Control channel opened");
                            } else
                                LOG_INFO("Logical channel %d opened", frame->channel);
                        } else {
                            LOG_INFO("UA to acknowledge DISC on channel %d received", frame->channel);
                            muxChannelInst->disc_ua_pending = 0;
                        }


                    }
                    break;

                case MuxDefines::GSM0710_TYPE_DM:
                    if (muxChannelInst->GetState() == MuxChannel::State::Opened) {

                        LOG_INFO("DM received, so the channel %d for %s was already closed",
                                 frame->channel, muxChannelInst->GetName().c_str());

                        mux->RemoveMuxChannel(muxChannel);

                    } else {
                        if (frame->channel == 0) {
                            LOG_INFO("Couldn't open control channel.\n->Terminating");
                            mux->SetState(MuxDaemon::States::MUX_STATE_CLOSING);
//close channels
                        } else
                            LOG_INFO("Logical channel %d for %s couldn't be opened", frame->channel,
                                     muxChannelInst->GetName().c_str());
                    }
                    break;
                case MuxDefines::GSM0710_TYPE_DISC:
                    if (muxChannelInst->GetState() == MuxChannel::State::Opened) {

                        SendMuxFrame(frame->channel, NULL, 0,
                                                 static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UA) |
                                                 static_cast<unsigned char>(MuxDefines::GSM0710_PF));

                        if (frame->channel == 0) {
                            mux->SetState(MuxDaemon::States::MUX_STATE_CLOSING);
                            LOG_INFO("Control channel closed");
                        } else {
                            LOG_INFO("Logical channel %d for %s closed", frame->channel,
                                     muxChannelInst->GetName().c_str());
                        }

                        mux->RemoveMuxChannel(muxChannel);


                    } else {
//channel already closed
                        LOG_WARN("Received DISC even though channel %d for %s was already closed",
                                 frame->channel, muxChannelInst->GetName().c_str());
                        // Send Mux frame
                        SendMuxFrame(frame->channel, NULL, 0,
                                                 static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_DM) |
                                                 static_cast<unsigned char>(MuxDefines::GSM0710_PF));
                    }
                    break;
                case MuxDefines::GSM0710_TYPE_SABM:
//channel open request
                    if (muxChannelInst->GetState() == MuxChannel::State::Opened) {
                        if (frame->channel == 0)
                            LOG_INFO("Control channel opened");
                        else
                            LOG_INFO("Logical channel %d for %s opened",
                                     frame->channel, muxChannelInst->GetName().c_str());
                    } else
//channel already opened
                        LOG_WARN("Received SABM even though channel %d for %s was already closed",
                                 frame->channel, muxChannelInst->GetName().c_str());
                    muxChannelInst->SetState(MuxChannel::State::Opened);
                    // Send mux frame
                    SendMuxFrame(frame->channel, NULL, 0,
                                             static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UA) |
                                             static_cast<unsigned char>(MuxDefines::GSM0710_PF));

                    break;

                default:
                    break;
            }
        }
    }

    return frames_extracted;

}

int MuxParser::HandleCtrlChannelCommands(GSM0710Frame *frame) {
    unsigned char type, signals;
    int length = 0, i, type_length, channel, supported = 1;
    unsigned char *response;

    //struct ussp_operation op;
    if (frame->length > 0) {
        type = frame->data[0];// only a byte long types are handled now skip extra bytes
        for (i = 0; (frame->length > i &&
                     (frame->data[i] & static_cast<unsigned char>(MuxDefines::GSM0710_EA)) == 0); i++);
        i++;
        type_length = i;
        if ((type & static_cast<unsigned char>(MuxDefines::GSM0710_CR)) ==
            static_cast<unsigned char>(MuxDefines::GSM0710_CR)) {
//command not ack extract frame length
            while (frame->length > i) {
                length = (length * 128) + ((frame->data[i] & 254) >> 1);
                if ((frame->data[i] & 1) == 1)
                    break;
                i++;
            }
            i++;
            MuxDefines ftype = static_cast<MuxDefines >(type & ~static_cast<unsigned char>(MuxDefines::GSM0710_CR));
            switch (ftype) {
                case MuxDefines::GSM0710_CONTROL_CLD:
                    //TODO: M.P implement CMUX close
                    LOG_INFO("The mobile station requested mux-mode termination");
                    mux->SetState(MuxDaemon::States::MUX_STATE_CLOSING);
                    break;
                case MuxDefines::GSM0710_CONTROL_PSC:
                    LOG_INFO("Power Service Control command: ***");
                    LOG_INFO("Frame->data = %s / frame->length = %d", frame->data + i, frame->length - i);
                    break;
                case MuxDefines::GSM0710_CONTROL_TEST:
                    LOG_INFO("Test command: ");
                    LOG_INFO("Frame->data = %s / frame->length = %d", frame->data + i, frame->length - i);
                    //serial->ping_number = 0;
                    break;
                case MuxDefines::GSM0710_CONTROL_MSC:
                    LOG_INFO("GET MSC");
                    if (i + 1 < frame->length) {
                        channel = ((frame->data[i] & 252) >> 2);
                        i++;
                        signals = (frame->data[i]);
//op.op = USSP_MSC;
//op.arg = USSP_RTS;
//op.len = 0;

                        auto muxChannel = static_cast<MuxChannel::MuxChannelType>(frame->channel);
                        auto muxChannelInst = mux->GetMuxChannel(muxChannel).value_or(nullptr);
                        if(muxChannelInst == nullptr){
                            LOG_ERROR("Invalid mux channel");
                            break;
                        }

                        LOG_INFO("Modem status command on channel %d", channel);
                        muxChannelInst->frameAllowed = ((signals &
                                                                       static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_FC)) !=
                                                                      static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_FC));
                        if ((signals & static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_FC)) ==
                            static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_FC))
                            LOG_INFO("No frames allowed");
                        else {
//op.arg |= USSP_CTS;
                            LOG_INFO("Frames allowed");
                        }
                        if ((signals & static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_RTC)) ==
                            static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_RTC)) {
//op.arg |= USSP_DSR;
                            LOG_INFO("Signal RTC");
                        }
                        if ((signals & static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_IC)) ==
                            static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_IC)) {
//op.arg |= USSP_RI;
                            LOG_INFO("Signal Ring");
                        }
                        if ((signals & static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_DV)) ==
                            static_cast<unsigned char>(MuxDefines::GSM0710_SIGNAL_DV)) {
//op.arg |= USSP_DCD;
                            LOG_INFO("Signal DV");
                        }
                    } else
                        LOG_ERROR("Modem status command, but no info. i: %d, len: %d, data-len: %d",
                                  i, length, frame->length);
                    break;
                default:
                    LOG_ERROR("Unknown command (%d) from the control channel", type);
                    if ((response = static_cast<unsigned char *>(malloc(sizeof(char) * (2 + type_length)))) != NULL) {
                        i = 0;
                        response[i++] = static_cast<unsigned char>(MuxDefines::GSM0710_CONTROL_NSC);
                        type_length &= 127; //supposes that type length is less than 128
                        response[i++] = static_cast<unsigned char>(MuxDefines::GSM0710_EA) | (type_length << 1);
                        while (type_length--) {
                            response[i] = frame->data[i - 2];
                            i++;
                        }
                        SendMuxFrame(0, response, i,
                                                 static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UIH));
                        free(response);
                        supported = 0;
                    } else
                        LOG_ERROR("Out of memory, when allocating space for response");
                    break;
            }
            if (supported) {
//acknowledge the command
                frame->data[0] = frame->data[0] & ~static_cast<unsigned char>(MuxDefines::GSM0710_CR);
                LOG_INFO("response MSC...");
                SendMuxFrame(0, frame->data, frame->length,
                                         static_cast<unsigned char>(MuxDefines::GSM0710_TYPE_UIH));
                LOG_INFO("response MSC");
#if 0
                switch ((type & ~GSM0710_CR)){
                  case GSM0710_CONTROL_MSC:
                    if (frame->control & GSM0710_PF){ //Check if the P/F var needs to be set again (cleared in write_frame)
                      uihPfBitReceived = 1;
                    }
                    LOGMUX(LOG_DEBUG, "Sending 1st MSC command App->Modem");
                    frame->data[0] = frame->data[0] | GSM0710_CR; //setting the C/R bit to "command"
                    write_frame(0, frame->data, frame->length, GSM0710_TYPE_UIH);
                    break;
                  default:
                    break;
                }
#endif
            }
        } else {
//received ack for a command
            if (GSM0710Buffer::GSM0710_COMMAND_IS(static_cast<MuxDefines >(type),
                                                  static_cast<unsigned char>(MuxDefines::GSM0710_CONTROL_NSC)))
                LOG_ERROR("The mobile station didn't support the command sent");
            else if (GSM0710Buffer::GSM0710_COMMAND_IS(static_cast<MuxDefines >(type),
                                                       static_cast<unsigned char>(MuxDefines::GSM0710_CONTROL_MSC))) {
                LOG_INFO("Channel:%d FC:%d", (frame->data[i] & 252) >> 2, frame->data[0] & 0x2);
                LOG_INFO("\n\nGET ACK FOR MSC\n\n");
                //sleep(1);
            } else
                LOG_INFO("Command acknowledged by the mobile station");
        }
    }
    return 0;
}
