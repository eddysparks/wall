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


#define SENDHOST "192.168.1.28"
#define INFOPORT 9393
//small
#define RCVPORT 9999
//big
//#define RCVPORT 9998

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ofApp : public ofxAndroidApp
{
    
public:
    
    void				setup();
    void				update();
    void				draw();
    
    void				keyPressed(int key);
    void				keyReleased(int key);
    void				mouseMoved(int x, int y );
    void				mouseDragged(int x, int y, int button);
    void				mousePressed(int x, int y, int button);
    void				mouseReleased(int x, int y, int button);

    bool				isServer;
    void                newData( DataPacket& _packet  );

    CommonTimeOSC*		commonTimeOsc;
    ClientOSCManager*   client;
    ServerOscManager*   server;

    ofTrueTypeFont		fontSmall;
    float               currTime;
    
    int                 screenIndex;
    int                 displayWidth;
    int                 displayHeight;
    int                 viewWidth;
    int                 viewHeight;
    int                 screenOffsetX;
    int                 screenOffsetY;
    
    //display stuff
    void remapSel();

   ofxOscReceiver receiver;
   ofxOscSender sender;

    ofPlanePrimitive  videoProjection;
    //ofVideoPlayer video;
    vector<ofxImageSequence> images;
    ofRectangle selection;
    //ofImage imgz[5];
    //vector<ofVideoPlayer> images;
    int maxSel, wichVid, wichSel;

    ofShader embossShader;
    int numLoaded;
    string loadPath;

    void loadMedia(string path);


    //battery stuff
    int batteryCapacity;
    int batteryStatus;
    int batteryTimeToFull;
    int batteryTimeToEmpty;

    void batteryInfo();

};
