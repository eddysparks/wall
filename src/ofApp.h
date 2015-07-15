#pragma once

#include <set>
#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "ofxOsc.h"
#include "ofxImageSequence.h"

#include "CommonTime/CommonTimeOsc.h"
#include "Server/ServerOscManager.h"
#include "Client/ClientOSCManager.h"
#include "Client/DataPacket.h"

/*
#define SENDHOST "192.168.1.28"
#define INFOPORT 9393
//small
#define RCVPORT 9999
//big
//#define RCVPORT 9998
*/
//#define DEBUG

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ofApp : public ofxAndroidApp
{
    
public:
    
    void				setup();
    void				update();
    void				draw();

    void touchDown(int x, int y, int id);
    void touchMoved(int x, int y, int id);
    void touchUp(int x, int y, int id);

    bool				isServer;
    void                newData( DataPacket& _packet  );
    int                 uniqueID;

    float               videoPct;
    CommonTimeOSC*		commonTimeOsc;
    ClientOSCManager*   client;
    ServerOscManager*   server;

    //ofTrueTypeFont		fontSmall;
    float               currTime, currTimeInt;
    
    int                 screenIndex;
    int                 displayWidth;
    int                 displayHeight;
    int                 viewWidth;
    int                 viewHeight;
    int                 screenOffsetX;
    int                 screenOffsetY;
    
    /*
    ofxOscReceiver receiver;
    ofxOscSender sender;
    */

    DataPacket load_data, play_data;
    string load_command, play_command;

    ofVideoPlayer video;
    float video_duration;

    bool videoPlay;
    bool previewMode;
    int numScreens;
    int numLoaded;
    string loadPath;
    int numMsx;

    ofTrueTypeFont		fontSmall, fontBig;

    string vidz[6];

    ofImage loadingImage;

    vector<ofImage> previews;
    vector<ofRectangle> previewsLoc;
    void loadPreviews(string previews_dir);
    void loadContents(string contents_dir);

    void getNumMsx(string msx_dir, int msx);
    void drawSelectMSX();
    void drawPlayMSX();
    void drawLoadImage();
    void drawPreviews();

    ofxXmlSettings XML;

    //battery stuff
    /*
    int batteryCapacity;
    int batteryStatus;
    int batteryTimeToFull;
    int batteryTimeToEmpty;

    void batteryInfo();
    */



};
