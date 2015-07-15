/*
#include "ofMain.h"
#include "ofApp.h"

#ifdef TARGET_ANDROID
	#include "ofAppAndroidWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif

//#include "ofGLProgrammableRenderer.h"


int main(){
/*
#ifdef TARGET_ANDROID
	ofAppAndroidWindow *window = new ofAppAndroidWindow;
#else
	ofAppGlutWindow *window = new ofAppGlutWindow;
#endif

    //ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer()));
	ofSetupOpenGL(window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
    //ofSetupOpenGL(1024,768, OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

        /*
	    ofGLESWindowSettings settings;
        settings.width = 1280;
        settings.height = 720;
        settings.setGLESVersion(2);
        ofCreateWindow(settings);


	ofRunApp( new ofApp() );
	return 0;
}


#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif
*/
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1920, 1200, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}

#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif