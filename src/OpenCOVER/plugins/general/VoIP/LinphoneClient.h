/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef _LINPHONECLIENT_H
#define _LINPHONECLIENT_H

#include <string>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include <linphone/core.h>

// ----------------------------------------------------------------------------
// enum class LinphoneClientState
// ----------------------------------------------------------------------------
enum class LinphoneClientState
{
    off,             // init state 
    halted,
    started,
    configured,
    offline,
    registering,
    registered,
    callIncoming,
    callInit,
    callInProgress,
    callRinging,
    callFailed,
    callConnected,
    callPaused,
    callResuming,
    callStreaming,
    callEnded
};

// ----------------------------------------------------------------------------
// class LinphoneClient
// ----------------------------------------------------------------------------
class LinphoneClient
{
public:

    //! constructor, set global callbacks, init state
    LinphoneClient();

    //! destructor, wait for thread to terminate, release linphone core
    ~LinphoneClient();

    //! sets the callback handler
    void addHandler(std::function<void (LinphoneClientState, LinphoneClientState)>* handler);

    //! sets the callback notifier
    void addNotifier(std::function<void (std::string)>* handler);

    //! sets the UDP port range for audio streaming
    void setAudioPortRange(unsigned int portMin, unsigned int portMax);
    
    //! sets the UDP port range for video streaming
    void setVideoPortRange(unsigned int portMin, unsigned int portMax);
    
    //! create new thread, start core iterator
    void startCoreIterator();

    //! initiate registration
    bool doRegistration(std::string sipaddress, std::string password);

    //! initiate unregistration
    void doUnregistration();
    
    //! check if audio is enabled within a call
    bool callAudioIsEnabled();
    
    //! check if video is enabled within a call
    bool callVideoIsEnabled();
    
    //! check if camera stream is allowed to be sent within a call
    bool callCameraIsEnabled();

    //! gets the name of the currently assigned sound device for playback
    const char* getPlaybackDeviceName();

    //! gets the list of the available sound devices which can capture
    std::vector<std::string> getCaptureSoundDevicesList();

    //! gets the list of the available sound devices which can playback sound
    std::vector<std::string> getPlaybackSoundDevicesList();

    //! gets the name of the currently capture sound device
    std::string getCurrentCaptureSoundDevice();

    //! sets the name of the currently capture sound device
    void setCurrentCaptureSoundDevice(std::string newDevice);

    //! gets the name of the currently playback sound device
    std::string getCurrentPlaybackSoundDevice();

    //! sets the name of the currently playback sound device
    void setCurrentPlaybackSoundDevice(std::string newDevice);
    
    //! gets the name of the currently ringer sound device
    std::string getCurrentRingerSoundDevice();

    //! sets the name of the currently playback sound device
    void setCurrentRingerSoundDevice(std::string newDevice);
    
    //! gets the name of the currently media sound device
    //std::string getCurrentMediaSoundDevice();

    //! tells wether microphone is enabled or not
    bool getMicrophoneIsEnabled();
    
    //! check whether the device has a hardware echo canceller
    //bool getEchoCancellerAvailable();

    //! returns true if echo cancellation is enabled
    bool getEchoCancellationIsEnabled();

    //! set echo cancellation onoff
    void setEchoCancellation(bool onoff);
    
    //! returns true if echo limiter is enabled
    bool getEchoLimiterIsEnabled();

    //! set echo limiter onoff
    void setEchoLimiter(bool onoff);
    
    //! tells whether the audio adaptive jitter compensation is enabled
    bool getAudioJitterCompensation();

    //! tset audio adaptive jitter compensation onoff
    void setAudioJitterCompensation(bool onoff);

    //! get microphone gain in db
    float getMicGain();

    //! set microphone gain in db
    void setMicGain(float gain);
    
    //! get playback gain in db before entering sound card
    float getPlaybackGain();

    //! set playback gain in db before entering sound card
    void setPlaybackGain(float gain);
    
    //! tells whether video capture is enabled
    bool getVideoCaptureEnabled();

    //! set video capture onoff
    void setVideoCaptureEnabled(bool onoff);
   
    //! tells whether video display is enabled
    bool getVideoDisplayEnabled();

    //! set video display onoff
    void setVideoDisplayEnabled(bool onoff);
   
    //! tells whether video preview is enabled
    bool getVideoPreviewEnabled();

    //! set video preview onoff
    void setVideoPreviewEnabled(bool onoff);
   
    //! get the default policy for acceptance of incoming video
    bool getAutoAcceptVideo();

    //! set the default policy for acceptance of incoming video
    void setAutoAcceptVideo(bool onoff);
    
    //! get the default policy for initiating video
    bool getAutoInitiateVideo();

    //! set the default policy for initiating video
    void setAutoInitiateVideo(bool onoff);
    
    //! tells whether the video adaptive jitter compensation is enabled
    bool getVideoJitterCompensation();

    //! set the video adaptive jitter compensation onoff
    void getVideoJitterCompensation(bool onoff);
    
    //! gets a list of the available video capture devices
    std::vector<std::string> getVideoCaptureDevicesList();

    //! returns the name of the currently active video device    
    std::string getCurrentVideoCaptureDevice();

    //! set the currently active video device    
    void setCurrentVideoCaptureDevice(std::string newDevice);
    
    //! get microphone muted state
    bool getCallMicrophoneMuted();
    
    //! get speaker muted state
    bool getCallSpeakerMuted();
    
    //! self view during call is enabled
    bool getCallSelfViewEnabled();
    
    //! camera stream are allowed to be send to remote
    bool getCallCameraEnabled();

    //! get current linphone state
    LinphoneClientState getCurrentState();

    //! easier readability fordebugging purposes
    std::string getStateString(LinphoneClientState);

    //! initiate call to given sip address, keep valid reference to call ptr
    bool initiateCall(std::string);

    //! hang up current call
    void hangUpCall();

    //! pause current call
    void pauseCall(bool pause);

    //! mute mic while in call
    void setMicMute(bool mute);

    //! mute speaker while in call
    void setSpeakerMute(bool mute);

    //! enable mic when starting a call
    void setMicrophoneEnabled(bool enable);
    
    //! enable  camera when starting a call
    void setCallCameraEnabled(bool enable);

    //! enable self view/pip in video window
    void setCallSelfViewEnabled(bool enable);

    //! get no. of calls
    int getNoOfCalls();

    //! accept current call
    void acceptIncomingCall();

    //! reject current call with reason declined
    void rejectIncomingCall();

    //! get remote address of current call
    std::string getCurrentRemoteAddress();
    
protected:

    std::vector< std::function<void (LinphoneClientState, LinphoneClientState)> * > handlerList;
    std::vector< std::function<void (std::string)> * > notifierList;
    
    LinphoneCore *lc = NULL;
    LinphoneCall *call = NULL;

    LinphoneCoreVTable vtable = {0};
    LinphoneProxyConfig* proxy_cfg = NULL;
    LinphoneAddress *from = NULL;
    LinphoneAuthInfo *info = NULL;

    const char* identity;
    const char* password;
    const char* server_addr;
    
    volatile bool stopThread = false;
    std::thread thdFunction;

    bool oneTime = false;  // TODO: remove
    
    float savedSpeakerGain; // FIXME: workaround for missing speaker mute
    
    const unsigned int timeout = 10; // * intervall ms
    const unsigned int intervall = 500; // time intervall core iterator polls
    
    //! linphoneclient main loop
    void thdMain();
};

// ----------------------------------------------------------------------------

#endif
