#include "ofApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::setup()
{
     ofSetLogLevel(OF_LOG_NOTICE);
     ofLog(OF_LOG_NOTICE, "start");
    cout<<"setting up"<<endl;
    ofBackground(0);
	isServer = false;
    //ofxAccelerometer.setup();
    //ofSetOrientation(OF_ORIENTATION_90_LEFT);
	//fontSmall.loadFont("Fonts/DIN.otf", 8 );
	ofEnableAlphaBlending();
    ofEnableArbTex();
    wichVid = 0;

	ofSeedRandom();

	ofLog(OF_LOG_NOTICE, "setup OSC");

    receiver.setup(RCVPORT);
    ofLog(OF_LOG_NOTICE, "done OSC");
    //small
    loadPath = "/storage/emulated/0/wall_box/";
    //big
    //loadPath = "/mnt/internal_sd/wall_box/";

    ////SYNC STUFF
    int uniqueID = ofRandom( 999999999 ); // Yeah this is bogus I know. Good enough for our purposes.
    server = NULL;
	client = NULL;
    ofLog(OF_LOG_NOTICE, "setup server");


    if( ofFile( ofToDataPath("Settings/IsServer.txt")).exists() )
	{
		server = new ServerOscManager();
		//server->init( "Settings/ServerSettings.xml" );
        server->init("192.168.1.255", 7776, 7777);
		isServer = server->isInitialised();
        //isServer = true;
        ofAddListener( server->newDataEvent, this, &ofApp::newData );
        ofLog(OF_LOG_NOTICE, "starting server");
	}
	else
	{   ofLog(OF_LOG_NOTICE, "starting client");
        ofxXmlSettings XML;
        bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
        if( loadedFile )
        {
            screenIndex = 1; //XML.getValue("Settings:ScreenIndex", 0);
            displayWidth = XML.getValue("Settings:DisplayWidth", 1920);
            displayHeight = XML.getValue("Settings:DisplayHeight", 1080);
            viewWidth = XML.getValue("Settings:ViewWidth", ofGetWidth());
            viewHeight = XML.getValue("Settings:ViewHeight", ofGetHeight());
            //screenOffsetX = viewWidth*screenIndex;
        }

		client = new ClientOSCManager();
		client->init( screenIndex, 7776);
        ofLog(OF_LOG_NOTICE, "done init client");

		commonTimeOsc = client->getCommonTimeOscObj();
		commonTimeOsc->setEaseOffset( true );

		ofAddListener( client->newDataEvent, this, &ofApp::newData );
	}


    //small

    videoProjection.set(ofGetWidth(),  ofGetHeight());
    videoProjection.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    videoProjection.setResolution(8, 8);

    //BIG
    //videoProjection.set(ofGetHeight(),  ofGetWidth());
    //videoProjection.setPosition((int)ofGetHeight()/2, (int)ofGetWidth()/2, 0);
    //videoProjection.setResolution(8, 8);

    ofLog(OF_LOG_NOTICE, "done texture");
    selection.x = 0.0;
    selection.y = 0.0;
    selection.width = 1.0;
    selection.height = 1.0;
    ofLog(OF_LOG_NOTICE, "remap selection");
    remapSel();

    //loadMedia("unknown");

    //sender.setup(SENDHOST, INFOPORT);
}

void ofApp::remapSel(){

     videoProjection.mapTexCoords(selection.x, selection.y,selection.x + selection.width,selection.y + selection.height);

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::update()
{
    //videoProjection.set(ofGetWidth(), ofGetHeight());
    currTime = 0.0f;
    if( isServer ) {currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }



    while(receiver.hasWaitingMessages()){

        ofxOscMessage m;
        receiver.getNextMessage(&m);

        if(m.getAddress() == "/LOC"){

            selection.x = m.getArgAsFloat(0);
            selection.y = m.getArgAsFloat(1);
            selection.width = m.getArgAsFloat(2);
            selection.height = m.getArgAsFloat(3);

            
            remapSel();

        }else if(m.getAddress()=="/NUMVID"){

            //if(m.getArgAsInt32(0)>= numLoaded){
                wichVid = m.getArgAsInt32(0);
            //}

        }else if(m.getAddress()=="/LOAD"){
            ofLog(OF_LOG_NOTICE, m.getArgAsString(0));
            loadMedia(m.getArgAsString(0));

        }else if(m.getAddress() == "/CLEAR"){

            numLoaded = 0;
            images.clear();

        }
    }
    //ofLog(OF_LOG_NOTICE, "checking battery");
    //batteryInfo();

}


void ofApp::loadMedia(string path){

    ofDirectory dir2scan(path);
    dir2scan.allowExt("jpg");
    dir2scan.listDir();


    ofxImageSequence * img = new ofxImageSequence();
    //small
    img->loadSequence(path+"/", "jpg",  0, dir2scan.size()-1);
    //img->loadSequence(path, "jpg", 0, numFiles);
    img->preloadAllFrames();
    img->setFrameRate(13);
    images.push_back(*img);

    numLoaded++;

}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::draw()
{
            if(numLoaded > 0){

            images[wichVid].getFrameForTime(ofGetElapsedTimef())->draw(0, 0,images[wichVid].getWidth(), images[wichVid].getHeight());
            images[wichVid].getTextureReference().bind();
                //old
                //videoProjection.draw();
                /*
                ofPushMatrix();
                    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
                    ofRotate(90, 0, 0, -1);

                        ofPushMatrix();
                            ofTranslate( -ofGetHeight()/2, -ofGetWidth()/2,0);
                            videoProjection.draw();

                        ofPopMatrix();
                ofPopMatrix();
                */
            images[wichVid].getTextureReference().unbind();
            }
            //ofDrawBitmapString(loadPath+"/"+"bubbles", 100, 100);

}

void ofApp::newData( DataPacket& _packet  )
{

    if(!isServer){
        if(_packet.valuesInt.size() > 0){
            if(_packet.valuesInt[0] == 0){

            }
            if(_packet.valuesInt[0] == 1){

            }
            if(_packet.valuesInt[0] == 2){

            }
            if(_packet.valuesInt[0] == 3){

            }
            if(_packet.valuesInt[0] == 4){

            }
            if(_packet.valuesInt[0] == 5){

            }
        }
    }else{
        cout<<"new data"<<endl;
    }

}



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::keyPressed(int key)
{
    if(isServer){
        DataPacket data;
        int client;
        string command;
        float value;
        if(key == 'a'){
            client = 0;
            command = "play";
            value = 100.00;
        }
        if(key == 'z'){
            client = 1;
            command = "play";
            value = 50.00;

        }
        if(key == 'e'){
            client = 2;
            command = "play";
            value = 25.00;

        }
        if(key == 'r'){
            client = 3;
            command = "play";
            value = 15.00;

        }
        if(key == 't'){
            client = 4;
            command = "play";
            value = 5.00;

        }
        if(key == 'y'){
            client = 5;
            command = "play";
            value = 2.50;
        }
        data.valuesFloat.push_back(value);
        data.valuesInt.push_back(client);
        data.valuesString.push_back(command);

        server->sendData(data.valuesString, data.valuesInt, data.valuesFloat);
    }

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::keyReleased(int key)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseMoved(int x, int y )
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseDragged(int x, int y, int button)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mousePressed(int x, int y, int button)
{

    if(!isServer){
        DataPacket packet;
        packet.valuesString.push_back("mouseClicked");
        client->sendData(packet);
    }

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::batteryInfo(){

	string delimiter = "=";

	ofFile tempFile;
	tempFile.open("/sys/class/power_supply/battery/uevent", ofFile::ReadOnly, false);
	ofBuffer buff = tempFile.readToBuffer();
	while(!buff.isLastLine()){
		string text = buff.getNextLine();
		string token = text.substr(0, text.find(delimiter));
		if (token == "POWER_SUPPLY_CAPACITY"){
			batteryCapacity = atoi(text.substr(text.find(delimiter) +1 , text.size()).c_str());
		    ofLog(OF_LOG_NOTICE, ofToString(batteryCapacity));
		} else if (token == "POWER_SUPPLY_STATUS"){
			if (text.substr(text.find(delimiter) +1 , text.size()).c_str()=="Not charging") batteryStatus = 0;
			else batteryStatus = 1;


		} else if (token == "POWER_SUPPLY_TIME_TO_EMPTY_NOW"){
			batteryTimeToFull = atoi(text.substr(text.find(delimiter) +1 , text.size()).c_str());
		} else if (token == "POWER_SUPPLY_TIME_TO_FULL_NOW"){
			batteryTimeToEmpty = atoi(text.substr(text.find(delimiter) +1 , text.size()).c_str());
		}

	}

	ofxOscMessage m;
	m.setAddress( "/battery");
	m.addIntArg(batteryCapacity);
	m.addIntArg(batteryStatus);
	m.addIntArg(batteryTimeToEmpty);
	m.addIntArg(batteryTimeToFull);

	sender.sendMessage( m );

}