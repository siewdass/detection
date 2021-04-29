#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <napi.h>
#include <string.h>
#include <vector>

class MyObject : public Napi::ObjectWrap < MyObject > {
 public:
  static Napi::Object Init( Napi::Env env, Napi::Object exports );
  MyObject( const Napi::CallbackInfo& info );

 private:
  Napi::Value detector( const Napi::CallbackInfo& info );

  double value_;
  std::string svm;
  std::string img;
  std::vector < std::vector < int > > detections;
};

#endif