#include "ofApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::setup()
{
     ofSetLogLevel(OF_LOG_NOTICE);

    fontSmall.loadFont("Fonts/DIN.otf", 30 );
    fontBig.loadFont("Fonts/DIN.otf", 60);
    ofBackground(0);
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
    isServer = true;
    loadPath = "/storage/emulated/0/wall_box/";
    //loadPath = "http://192.168.1.37:8000";
    ofEnableAlphaBlending();
    ofEnableArbTex();
    ofSeedRandom();

    ////SYNC STUFF
    uniqueID = ofRandom( 999999999 );
    server = NULL;
	client = NULL;
    ofLog(OF_LOG_NOTICE, "setup server");

    if( isServer )
	{
		server = new ServerOscManager();

        server->init("192.168.1.255", 7776, 7777);
		isServer = server->isInitialised();

        ofAddListener( server->newDataEvent, this, &ofApp::newData );

        ofLog(OF_LOG_NOTICE, "starting server");
	}
	else
	{   ofLog(OF_LOG_NOTICE, "starting client");
        ofxXmlSettings XML;
        bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
        if( loadedFile )
        {
            screenIndex = 3;
            displayWidth = XML.getValue("Settings:DisplayWidth", 1920);
            displayHeight = XML.getValue("Settings:DisplayHeight", 1080);
            viewWidth = XML.getValue("Settings:ViewWidth", ofGetWidth());
            viewHeight = XML.getValue("Settings:ViewHeight", ofGetHeight());

        }

		client = new ClientOSCManager();
		client->init( screenIndex, 7776);
        ofLog(OF_LOG_NOTICE, "done init client");

		commonTimeOsc = client->getCommonTimeOscObj();
		commonTimeOsc->setEaseOffset( false );

		ofAddListener( client->newDataEvent, this, &ofApp::newData );
		}


    if(!isServer){
       loadingImage.load("loadscreen.jpg");
    }else if(isServer){
        ofLog(OF_LOG_NOTICE, "loading previews");
        loadPreviews(loadPath + "MSX_PREVIEWS");
    }
    previewMode = true;
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::update()
{
    currTime = 0.0f;
    if( isServer ) {currTime = ofGetElapsedTimef();
    }
    else { currTime = commonTimeOsc->getTimeSecs();

    }
    if(video.isLoaded()){
        video.update();
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::draw(){

           if(video.isPlaying()){
                video.draw(0, 0, ofGetWidth(), ofGetHeight());
           }
           else{
                if(isServer){

                    if(video.isLoaded() == false && video.isPlaying()==false){
                        drawPreviews();
                    }else if(video.isPlaying() == false && video.isLoaded()==true){
                        drawPlayMSX();
                    }

                }else if(!isServer){
                        drawLoadImage();
                }
           }
                //to be double checked
                #ifdef DEBUG
                ofSetColor(0, 255, 0);
                //server->draw();
                if( isServer )
                {
                    //fontSmall.drawString( "Server   Time: " + ofToString( currTime, 2), 600, 85 );
                }
                else
                {
                    fontSmall.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 300, 30 );
                    fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 300, 80 );
                    fontSmall.drawString( "uuid: " + ofToString(screenIndex), 300, 120 );

                }
                ofSetColor(255, 255, 255);
                #endif
}

void ofApp::newData( DataPacket& _packet){

    if(!isServer){

        if(_packet.valuesString.size() > 0){
            if(_packet.valuesString[0] == "P"){
                videoPlay = !videoPlay;
                if(videoPlay){
                    video.setPosition(0);
                    video.setPaused(false);
                }else{

                    video.setPosition(0);
                    video.setPaused(true);
                    video.close();
                }
            }else if(_packet.valuesString[0]=="F"){
                ofLog(OF_LOG_NOTICE, ofToString(_packet.valuesInt[0]));
                getNumMsx(loadPath + "MSX", _packet.valuesInt[0]);
            }
        }
    }
}

void ofApp::touchUp(int x, int y, int id){

    ofPoint touchPoint;
    touchPoint.x = x;
    touchPoint.y = y;
    ofLog(OF_LOG_NOTICE, ofToString(previewMode));

    if(isServer){
        if(previewMode == false){
            ofSetOrientation(OF_ORIENTATION_UNKNOWN);
            play_command = "P";
            play_data.valuesString.push_back(play_command);
            server->sendData(play_data);
            videoPlay = !videoPlay;

            if(videoPlay){
                video.setPosition(0);
                video.setPaused(false);
            }else{
                video.setPosition(0);
                video.setPaused(true);
                video.stop();
                video.close();
                previewMode = true;
             }
        }else if(previewMode == true){

            if(previewsLoc.size()>0){

                for(int i = 0; i<previewsLoc.size(); i++){

                    if(previewsLoc[i].inside(touchPoint)){

                        getNumMsx(loadPath + "MSX", i);
                        previewMode = false;
                    }
                }
            }
        }
    }
}

void ofApp::touchDown(int x, int y, int id){}
void ofApp::touchMoved(int x, int y, int id){}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//

void ofApp::drawPlayMSX(){

    ofPushStyle();
        ofNoFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetCircleResolution(200);
        ofSetLineWidth(5);
        ofCircle(ofGetWidth()/2, ofGetHeight()/2, 350);
        fontBig.drawString("START THE MSX >", (ofGetWidth()/4), ofGetHeight()/2);
    ofPopStyle();
}

void ofApp::drawLoadImage(){

    loadingImage.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::loadContents(string contents_dir){

    // get the list of video files in the MSX dir
    ofDirectory dir(contents_dir);
    dir.allowExt("mp4");
    dir.listDir();
    dir.sort();
    // if already loaded closes the movie and dealocate
    if(video.isLoaded() == true){
        video.close();

    }
    ///now where sure that video is not loaded load it and set it to pause at frame 0

    //if server load first video if client load video at client index
    if(isServer){
        video.load(dir.getPath(0));
    }else{
        video.load(dir.getPath(screenIndex));
    }
    video.setLoopState(OF_LOOP_NORMAL);
    video.play();
    video.setPosition(0);
    video.setPaused(true);

}

void ofApp::loadPreviews(string previews_dir){

    string rawXML = ofFile("http://192.168.1.37:8000/wall.xml").readToBuffer();

    if(XML.loadFromBuffer(rawXML)){

        ofLog(OF_LOG_NOTICE, "XML LOADED");
    }else{

        ofLog(OF_LOG_NOTICE, "XML NOT LOADED");
    }


    ofLog(OF_LOG_NOTICE, "loading from");
    ofLog(OF_LOG_NOTICE, previews_dir);

    ofDirectory dir(previews_dir);
    dir.allowExt("jpeg");

    dir.listDir();
    dir.sort();

    ofLog(OF_LOG_NOTICE, "num previews");
    ofLog(OF_LOG_NOTICE, ofToString(dir.numFiles()));

    //loadImages
    for(int i = 0; i<dir.numFiles(); i++){
        ofImage * prev = new ofImage();
        ofLog(OF_LOG_NOTICE, dir.getPath(i));
        prev->load(dir.getPath(i));
        previews.push_back(*prev);
    }

}

void ofApp::drawPreviews(){
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
    //populate previewbuttons
        if(previewsLoc.size()>0){

            previewsLoc.clear();
        }
        int screendiv = ofGetWidth()/previews.size();

        for(int i = 0; i<previews.size(); i++){

            ofRectangle * rect = new ofRectangle();

            rect->x = screendiv * i;
            rect->y = ofGetHeight()/4;
            rect->width = screendiv;
            rect->height = ofGetHeight()/2;

            previewsLoc.push_back(*rect);
        }

    ofPushStyle();
        ofSetColor(255, 255, 255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        fontBig.drawString("SELECT THE MSX", (ofGetWidth()/4), 100);
    ofPopStyle();

    if(previews.size() > 0){

        ofPushStyle();
        for(int i =0; i<previews.size(); i++){
            previews[i].draw(previewsLoc[i].x, previewsLoc[i].y, previewsLoc[i].width, previewsLoc[i].height);
            ofSetLineWidth(3);
            ofNoFill();
            ofSetColor(255, 255, 255);
            ofRect(previewsLoc[i].x, previewsLoc[i].y, previewsLoc[i].width, previewsLoc[i].height);
        }
        ofPopStyle();
    }
}

void ofApp::getNumMsx(string msx_dir, int msx){

    ofDirectory dir(msx_dir);
    dir.listDir();
    dir.sort();
    ofLog(OF_LOG_NOTICE, "numMSX = ");
    ofLog(OF_LOG_NOTICE, ofToString(msx));
    ofLog(OF_LOG_NOTICE, "gettin MSX: ");
    ofLog(OF_LOG_NOTICE, dir.getPath(msx));

    loadContents(dir.getPath(msx));

    if(isServer){

        load_command = "F";
        load_data.valuesString.push_back(load_command);
        ofLog(OF_LOG_NOTICE, "SENDING MSX: ");
        ofLog(OF_LOG_NOTICE, ofToString(msx));
        load_data.valuesInt.clear();
        load_data.valuesInt.push_back(msx);
        server->sendData(load_data);

    }
}