//  DRAWPOLYGONS_v02
//
//  Created by Anto+Creo on 06/01/2015.
//
//

#include "ofApp.h"


////-------------------------------------------------------------
//ofApp::~ofApp() {
//    
//    delete beat.myData; /*you should probably delete myData for any sample object
//                         that you've created in ofApp.h*/
//    
//}






//--------------------------------------------------------------
void ofApp::setup(){
    
    sender.setup(HOST, PORT);
    stage.set(0, 0, ofGetWidth(), ofGetHeight());

    
    ofSetFullscreen(true);  // set fullscreen
    
    open=true;              // set the polygon true at the beginning
    running=true;
    ableClick=true;
//    ofSetBackgroundAuto(false);
    ofBackground(15);      //background white
    handlerRadius = 5;     // radius of the handler of the polygon
    
    
    ///////////COLOR PICKER////////
    //setting up the color picker
    
    float w = ofGetWidth()/6;
    float h = ofGetHeight()/6;
    float cx = w/2;
    float cy = h/2;
    
  
    img.allocate(w,h,OF_IMAGE_COLOR);
    
    for (float y=0; y<h; y++) {
        for (float x=0; x<w; x++) {
            
            float angle = atan2(y-cy,x-cy)+PI;
            float dist = ofDist(x,y,cx,cy);
            float hue = angle/TWO_PI*255;
            float sat = ofMap(dist,0,w/4,0,255,true);
            float bri = ofMap(dist,w/4,w/2,255,0,true);
            
            img.setColor(x,y,ofColor::fromHsb(hue,sat,bri));
            
        }
    }
    
    img.reloadTexture();
    
    ofPoint mousePos(mouseX,mouseY);
    
    
    colorPickerRect.set((ofGetWidth()-ofGetWidth()/6),(ofGetHeight()-ofGetHeight()/6), ofGetWidth(), ofGetHeight());
    


    
//////////////////////////////
    
    curvedVal= "Curved";
    
    
    ////////////////// fbo color picker /////////////////
    
//        pickerFbo.allocate(w, h,GL_RGB);
//    
//        pickerFbo.begin();
//        ofClear(255,255,255);
//        pickerFbo.end();
    
    
    ofSetRectMode(OF_RECTMODE_CORNER);
//    img.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight()-ofGetHeight()/6);
    
    if (colorPickerRect.inside(mouseX, mouseY)) {
        
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255,255,255,200);
        ofRect(mouseX+10, mouseY, 30, 30);
        
        
        colorPicked.set(img.getColor(mouseX-(ofGetWidth()-ofGetWidth()/6), mouseY-(ofGetHeight()-ofGetHeight()/6)));
//        colorPicked.set(colorPicked.r,colorPicked.g,colorPicked.b,curveVertices.size()*10);
        ofSetColor(colorPicked);
        
        ofRect(mouseX+10, mouseY, 20, 20);
        ofFill();
        ofSetColor(255);
        ofPushMatrix();
        
        ofDrawBitmapString(ofToString( colorPicked), 50,500);
    }

    ////////////////// end fbo color picker /////////////////

    
    //temp coordinates
    for (int i = 0; i < curveVertices.size(); i++){
        
        xt = curveVertices[i].x;
        yt = curveVertices[i].y;
        
        
    }
    
    
    ///////////////// maximilian//////////////
    
    /* some standard setup stuff*/
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    
    /* This is stuff you always need.*/
    
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
    
    
    /* Now you can put anything you would normally put in maximilian's 'setup' method in here. */
    
    
    beat.load(ofToDataPath("beat2.wav"));
    beat.getLength();
    
    
    ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);/* Call this last ! */
 

    randomZ=0;

}

//--------------------------------------------------------------
void ofApp::update(){
    img.update();
    
    for (int i = 0; i < curveVertices.size(); i++){

    
    xt +=(ofGetWidth()/2-xt)*0.01;
    yt +=(ofGetHeight()/2-yt)*0.01;

//        xt = curveVertices[i].x;
//        yt = curveVertices[i].y;
    
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    

    if (selectedColor.size()==0) {
        
        colorPicker();
    }
   

    //---------POLYGONS---------------------

    if (selectedColor.size()>0) {
        polygons();
        
        for (int i = 0; i < curveVertices.size(); i++){
            
            
            
        }
        
        
        
        
        ///////////maximilian///////
        
        
        /* You can use any of the data from audio received and audiorequested to draw stuff here.
         Importantly, most people just use the input and output arrays defined above.
         Clever people don't do this. This bit of code shows that by default, each signal is going to flip
         between -1 and 1. You need to account for this somehow. Get the absolute value for example.
         */
        
//        ofSetColor(255, 255, 255,255);
//        ofRect(600, 300, sample*150, sample*150); /* audio sigs go between -1 and 1. See?*/
//        ofCircle(200, 300, wave*150);
        


    }
    
    
    //---------COLOR PICKER---------------------
    
//    pickerFbo.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight() - ofGetHeight()/6);
    
    if (colorPickerRect.inside(mouseX, mouseY)) {
        colorPicker();
        ableClick=false;
        
    }else{
    
        ableClick=true;
    
    }
  

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ///screen false/true
    
    if(key=='a'){
        ofColor transpaColor(colorPicked.r,colorPicked.g,colorPicked.b,ofRandom(20,150));
        selectedColor.clear();
        selectedColor.push_back(transpaColor);
        
        ofSetBackgroundAuto(false);
        
            }
    
    //easycam
    
    if(key=='z'){
        camActive=true;
        randomZ=ofRandom(-1000,1000);
        
    }else{
    
        camActive=false;
    }

    
    
    
    
//    ///close path with spacebar
//    if(key==' '){
//        if(ofFill){
//            ofNoFill();
//        }else{
//            ofFill;
//        }
//    }
    
    
    ///DELETE LAST POINT
    
    if(key=='b' && curveVertices.size()>0){
        open=true;
        curveVertices.pop_back();
        myPoints.pop_back();
        ableClick=true;
        saveXml(); //saving Xml file

    }
    
    //DELETE ALL
    
    if(key==OF_KEY_BACKSPACE){
        
        curveVertices.clear();
        myPoints.clear();
        distance=0;

        open=true;
        ableClick=true;
        saveXml(); //saving Xml file

    }
  
    ///MAKE IT CURVE
    
    if(key=='c' && curveVertices.size()>0){
        if (curved) {
            curved=false;
            curvedVal= "Curved";


        } else {
            curved=true;
            curvedVal= "Straight";


        }
            
        saveXml(); //saving Xml file
    }
    
    ///FULLSCREEN
    
    if(key=='f'){
        ofToggleFullscreen();  // set fullscreen
    }
        
     ///TEST RANDOM
    
    if(key=='k'){
        
        
        
        if (curved) {
//            ofBackground(15);
            ofColor randomColor(ofRandom(255), ofRandom(255),ofRandom(255),curveVertices.size()*10);
            selectedColor.clear();
            selectedColor.push_back(randomColor);

            
            for (int i = 0; i < curveVertices.size(); i++){
                
                curveVertices[i].x=ofRandom(ofGetWidth());
                curveVertices[i].y=ofRandom(ofGetHeight());
            
            }
            
        } else {
            ofColor randomColor(ofRandom(255), ofRandom(255),ofRandom(255),curveVertices.size()*10);
            selectedColor.clear();
            selectedColor.push_back(randomColor);

//            ofSetBackgroundAuto(false);
            for (int i = 0; i < curveVertices.size(); i++){
                ofPoint position(curveVertices[i].x,curveVertices[i].y);
                ofPoint centre(ofGetWidth()/2,ofGetHeight()/2);
                
                float dist = ofDist(position.x,position.y , centre.x,centre.y);
                
                
                
                
                if (stage.inside(curveVertices[i].x,curveVertices[i].y)) {
   
                    directionX=ofMap(mouseX, 0, ofGetWidth(), -100, 100);
                    directionY=ofMap(mouseY, 0, ofGetHeight(), -100, 100);

                    
                    curveVertices[i].x+=ofRandom(directionX);
                    curveVertices[i].y+=ofRandom(directionY);
                   
            }
        
        else {
            curveVertices[i].x=ofGetWidth() -  curveVertices[i].x;
            curveVertices[i].y=ofGetHeight() -  curveVertices[i].y;
            curveVertices[i].x+=ofRandom(directionX);
            curveVertices[i].y+=ofRandom(directionY);
            
            

          }
       }
    }
        
        saveXml(); //saving Xml file
        
        
    }
    

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if (key=='z') {
        myCam.end();
        camActive=false;
        randomZ=0;
    }
    
    if(key=='a'){
        
        ofSetBackgroundAuto(true);
        
    }
    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    //draggable vertices
    
    for (int i = 0; i < curveVertices.size(); i++){
        float diffx = x - curveVertices[i].x;
        float diffy = y - curveVertices[i].y;
        float dist = sqrt(diffx*diffx + diffy*diffy);
        if (dist < curveVertices[i].radius){
            curveVertices[i].bOver = true;
        } else {
            curveVertices[i].bOver = false;
            
            saveXml(); //saving Xml file

        }
        
    }

    if (colorPickerRect.inside(mouseX, mouseX)) {
    
        img.reloadTexture();
    
    }




}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    //draggable vertices

    
    for (int i = 0; i < curveVertices.size(); i++){
        if (curveVertices[i].bBeingDragged == true){
            curveVertices[i].x = x;
            curveVertices[i].y = y;
            saveXml(); //saving Xml file

        }
    }


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    

    
    if (colorPickerRect.inside(mouseX, mouseX) && selectedColor.size()>0) {
        
            selectedColor.clear();
            selectedColor.push_back(colorPicked);
        

    }
    
    
    
    ofColor newColor(int r, int g,int b, int a);
    selectedColor.push_back(colorPicked);
    
//    img.clear();
    
  
    
    // if the size of the vector color is > 1 then start drawing the shape
    

    
        if (selectedColor.size()>1 && ableClick==true) {
            
            printf("V %lu \n",myPoints.size() );
            printf("curvevertices %lu \n",curveVertices.size() );
            
            draggableVertex newDraggableVertex(x,y,randomZ);
            curveVertices.push_back(newDraggableVertex);
            
            ofPoint newPoint(x,y,0);
            myPoints.push_back(newPoint);
            
            
            
//            saveXml(); //saving Xml file


            }
    
    
    //draggable vertices
    
    for (int i = 0; i < curveVertices.size(); i++){
        float diffx = x - curveVertices[i].x;
        float diffy = y - curveVertices[i].y;
        float dist = sqrt(diffx*diffx + diffy*diffy);
        
        
//        curveVertices[i].x=xt;
//        curveVertices[i].y=yt;
        
        if (curveVertices[i].bOver==true) {
            ofSetColor(0);
            ofDrawBitmapString(ofToString(curveVertices[i].bOver), 30,100);
//            ableClick=false;

        }
        
        if (dist < curveVertices[i].radius){
            curveVertices[i].bBeingDragged = true;
            saveXml(); //saving Xml file

            
        } else {
            curveVertices[i].bBeingDragged = false;
        }
     
    }
    

    

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    //draggable vertices

    for (int i = 0; i < curveVertices.size(); i++){
        curveVertices[i].bBeingDragged = false;
        saveXml(); //saving Xml file

    }

    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



//---------COLOR PICKER-----------------------------------------

void ofApp::colorPicker(){
    
    if (colorPickerRect.inside(mouseX, mouseY)) {

    ofSetRectMode(OF_RECTMODE_CORNER);
    img.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight()-ofGetHeight()/6);
    
   
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255,255,255,200);
    ofRect(mouseX+10, mouseY, 30, 30);
    
    
    colorPicked.set(img.getColor(mouseX-(ofGetWidth()-ofGetWidth()/6), mouseY-(ofGetHeight()-ofGetHeight()/6)));
    ofSetColor(colorPicked);

    ofRect(mouseX+10, mouseY, 20, 20);
    ofFill();
    ofSetColor(255);
    ofPushMatrix();
    
//    ofDrawBitmapString(ofToString( colorPicked), 50,500);

    }
    

}


//---------POLYGONS-----------------------------------------

void ofApp::polygons(){
    
    if (camActive) {
        myCam.begin();

        ofPushMatrix();
        ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);


    }

  
///////////////////////////////////////////////////////////////////////////////////
    


    // if drawing the lines the polygon is open
    if(open){
        ableClick=true;        // if the polygone it's open activate the click function
        for(int k=0;k<curveVertices.size();k++){
            distance= ofDist(curveVertices[k].x, curveVertices[k].y, curveVertices[0].x, curveVertices[0].y);
            
            if(k>=0){
                ofSetColor(selectedColor[0]);
                ofSetLineWidth(3);
                ofNoFill();
                ofBeginShape();
                //shape
                for (int i = 0; i < curveVertices.size(); i++){
                    ofVertex(curveVertices[i].x, curveVertices[i].y);
                }
                ofEndShape(false);
                //handle circle
                for (int i = 0; i < curveVertices.size(); i++){
                    if (curveVertices[i].bOver == true) ofFill();
                    else ofNoFill();
                    ofCircle(curveVertices[i].x, curveVertices[i].y,4);
                }
            }
            
            

            //checking the distance mousePos and first point
            if (   mouseX - curveVertices[0].x<=handlerRadius*5
                && mouseY - curveVertices[0].y<=handlerRadius*5
                && curveVertices[0].x - mouseX<=handlerRadius*5
                && curveVertices[0].y - mouseY<=handlerRadius*5
                ) {
                ofSetColor(0);
                string mouseMessage;
                
                //CLOSE MESSAGE
//                mouseMessage = "close the pattern";
//                if (curveVertices.size()>1) {
//                    ofDrawBitmapString(mouseMessage, mouseX-mouseMessage.length()*10,mouseY+10);
//                    
//                }

                // handle blink
                for (int i=1; i<ofGetFrameNum()%40; i++) {
                    ofNoFill();
                    ofSetColor(selectedColor[0],200/i+10);
                    ofSetLineWidth(5/i);
                    ofCircle(curveVertices[0].x,curveVertices[0].y,(i*(i+1)/100));
                
                        }
                    }
      
                }
//
    }
        if (distance>0 && distance<=handlerRadius*2) {
    
//        ofSetColor(selectedColor[0]);
//        ofDrawBitmapString("Press C to make " + curvedVal + " shape", 30,ofGetHeight()/2);
        
            //handles
            for (int i = 0; i < curveVertices.size(); i++){
                if (curveVertices[i].bOver == true) ofFill();
                else ofNoFill();
                ofCircle(curveVertices[i].x, curveVertices[i].y,5);
            }

        ////////THIS IS THE OLD BEGIN SHAPE

        ofBeginShape();
        for(int i=0;i<curveVertices.size();i++){

            ofSetColor(selectedColor[0]);
            ofFill();
          
            //curved shape
            if (curved) {
                
                if (i == 0){
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
                } else if (i == curveVertices.size()-1){
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
                } else {
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                }
                


            }else{
                
            //straight shape
            ofVertex(curveVertices[i].x,curveVertices[i].y);
            curved=false;
            }

        }
            
        ofEndShape();
            open=false;
//            ableClick=false;        // if the polygone it's closed deactivate the click function

            
            if (camActive) {
                
//                myCam.setTarget(ofVec3f(ofGetWidth(),ofGetHeight()));

                ofPopMatrix();
                
                myCam.end();


            }
            
            
        ///END/////
        
        
    }


}





//---------XML---------------------

void ofApp::saveXml(){
    
    //////////////////XML//////////////
    ofxXmlSettings positions;
    
    ofxOscMessage colorOSC;
    ofxOscMessage positionOSC;
    
    ///color
    positions.addTag("color");
    positions.pushTag("color");

    positions.addValue("R", selectedColor[0].r);
    positions.addValue("G", selectedColor[0].g);
    positions.addValue("B", selectedColor[0].b);
    
    //osc
//    m.setAddress("/color");

    colorOSC.addIntArg(selectedColor[0].r);
    colorOSC.addIntArg(selectedColor[0].g);
    colorOSC.addIntArg(selectedColor[0].b);
    sender.sendMessage(colorOSC);



    positions.popTag();//pop color
    ///
    
    ///curved
    positions.addTag("curved");
    positions.pushTag("curved");
    positions.addValue("value", curved);
    positions.popTag();//pop curved
    ///
    
    ///point coord
    positions.addTag("positions");
    positions.pushTag("positions");
    //positions is a vector<draggableVerteces> that we want to save to a file
    for(int i = 0; i < curveVertices.size(); i++){
        //each position tag represents one point
        positions.addTag("position");
        positions.pushTag("position",i);
        //so set the three values in the file
        positions.addValue("X", curveVertices[i].x);
        positions.addValue("Y", curveVertices[i].y);
        positions.addValue("Z", curveVertices[i].z);

//        //osc
//        m.setAddress("/coordinate");

        positionOSC.addIntArg(curveVertices[i].x);
        positionOSC.addIntArg(curveVertices[i].y);
        sender.sendMessage(positionOSC);
        positions.popTag();//pop position
    }
    positions.popTag(); //pop position
    positions.saveFile("positions.xml");
    
    //////////////////XML//////////////
    
    ///OSC message
    
    
    
}

//--------------------------------------------------------------
void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        
        /* Stick your maximilian 'play()' code in here ! Declare your objects in ofApp.h.
         
         For information on how maximilian works, take a look at the example code at
         
         http://www.maximilian.strangeloop.co.uk
         
         under 'Tutorials'.
         
         */
        
        
        
//        sample=beat.play(0.25, 0, beat.length);
//        wave=sine1.sinebuf(abs(mouseX));/* mouse controls sinewave pitch. we get abs value to stop it dropping
                                         //										 delow zero and killing the soundcard*/
        
        
        
        wave=0;
        wave2=0;
        for(int i=0; i < curveVertices.size(); i++) {
            double f0 = abs(curveVertices[i].x);
            double f1 = abs(curveVertices[i].y);
            double f2 = abs(curveVertices[i].z);
            double rgb = abs(colorPicked.b*colorPicked.r*colorPicked.g);
            double sound2 = ofMap(rgb,0,(powf(255,3)),20,600);
//            double thisSine = wave + sineBank[i].sinewave(f0 + (i * f1))*sin(i)+ sineBank[i-1].square(f1);
            double thisSine = (sineBank[i-1].sinewave(sound2)*sin(i*f2))*(sineBank[i].sinewave(f1)*cos(i));
            
            if (camActive) {
                double thisSine2 = (sineBank[i-1].triangle(f2)*sin(i)*sin(colorPicked.b))*(sineBank[i].sinewave(colorPicked.r)*sin(colorPicked.g));
                wave2 = wave2 + thisSine2;
            } else {

            double thisSine2 = (sineBank[i-1].sawn(f2)*sin(i)*sin(colorPicked.b))*(sineBank[i].sinewave(colorPicked.r)*sin(colorPicked.g));
                wave2 = wave2 + thisSine2;
            
            }

//            double multiplier = 1.0 / (i+1.0);
//            double multiplier = 1.0 / (i);

//            thisSine = (thisSine)*multiplier;
            wave = wave + thisSine;
            
            
            //            f0+=sin(multiplier)*i*10;
        }
        if (curveVertices.size()>0){
            wave *=ofNormalize((curveVertices.size()),.1,.5);
            wave2 *=ofNormalize((curveVertices.size()),.1,.5);

//            ofNormalize(wave, 0, .1);

        }

        *output = wave;//simple as that!
        *output = wave2;

        

        
       

        
        mymix.stereo(sample + wave + wave2, outputs, 0.5);

        
        output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
        output[i*nChannels + 1] = outputs[1];
    }
    
}

//--------------------------------------------------------------
void ofApp::audioReceived 	(float * input, int bufferSize, int nChannels){
    
    
    /* You can just grab this input and stick it in a double, then use it above to create output*/
    
    for (int i = 0; i < bufferSize; i++){
        
        /* you can also grab the data out of the arrays*/
        
        
    }
    
}










