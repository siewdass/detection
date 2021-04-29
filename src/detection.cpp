#include "detection.h"
#include <string.h>
#include <iostream>
#include <dlib/svm_threaded.h>
#include <dlib/image_processing.h>
//#include <dlib/data_io.h>
//#include <dlib/opencv.h>

#include <dlib/image_io.h>
//#include <opencv2/opencv.hpp>
#include <iostream>

Napi::Object MyObject::Init( Napi::Env env, Napi::Object exports ) {
    Napi::Function func = DefineClass(  env, "objectDetector", {
        InstanceMethod( "detector", &MyObject::detector ) 
    } );

    Napi::FunctionReference* constructor = new Napi::FunctionReference( );
    *constructor = Napi::Persistent( func );
    env.SetInstanceData( constructor );

    exports.Set( "objectDetector", func );
    return exports;
}

MyObject::MyObject( const Napi::CallbackInfo& info ) : Napi::ObjectWrap < MyObject > ( info ) {

    Napi::Env env = info.Env( );

    if ( info.Length( )  <= 0 || !info[ 0 ].IsString( ) ) {

        Napi::TypeError::New( env, "String expected" ).ThrowAsJavaScriptException( );
        return;

    }

    MyObject::svm = info[ 0 ].ToString( ).Utf8Value( );

}

Napi::Value MyObject::detector( const Napi::CallbackInfo& info ) {

    Napi::Env env = info.Env( );
    Napi::Array arr = Napi::Array::New( env );

    int length = info.Length( );

    if ( length <= 0 || !info[ 0 ].IsString( ) ) {

        Napi::TypeError::New( env, "String expected" ).ThrowAsJavaScriptException( );

    }

    MyObject::img = info[ 0 ].ToString( ).Utf8Value( );

    try {

        //cv::Mat img = cv::imread( "/home/siewdass/detection/test.png" );
        //dlib::cv_image < dlib::rgb_pixel > dlib_img( img );

        dlib::array2d < dlib::rgb_pixel > dlib_img;    
        dlib::load_image( dlib_img, MyObject::img );

        typedef dlib::scan_fhog_pyramid < dlib::pyramid_down < 6 > > scanner; 
        dlib::object_detector < scanner > detector;
        dlib::deserialize( MyObject::svm ) >> detector;

        std::vector < dlib::rectangle > detections = detector( dlib_img );

        for ( int i = 0; i < detections.size( ); i++ ) {
            int x = detections[ i ].left( );
            int y = detections[ i ].top( );
            int width = detections[ i ].right( );
            int height = detections[ i ].bottom( );
            
            Napi::Object obj = Napi::Object::New( env );
            obj.Set( Napi::String::New( env, "x" ), Napi::Number::New( env, x ) );
            obj.Set( Napi::String::New( env, "y" ), Napi::Number::New( env, y ) );
            obj.Set( Napi::String::New( env, "width" ), Napi::Number::New( env, width ) );
            obj.Set( Napi::String::New( env, "height" ), Napi::Number::New( env, height ) );
            arr.Set( i, obj );
        }

    } catch ( std::exception &e ) {

        Napi::TypeError::New( env, e.what( ) ).ThrowAsJavaScriptException( );

    }

    return arr;

}

