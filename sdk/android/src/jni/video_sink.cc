/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <modules/video_coding/codecs/multiplex/include/augmented_video_frame_buffer.h>
#include "sdk/android/src/jni/video_sink.h"

#include "sdk/android/generated_video_jni/VideoSink_jni.h"
#include "sdk/android/src/jni/video_frame.h"

namespace webrtc {
namespace jni {

ScopedJavaLocalRef<jbyteArray> ToJavaByteArray(JNIEnv* env,
                                           const uint8_t* bytes,
                                           size_t len) {
   jbyteArray byte_array = env->NewByteArray(len);


   env->SetByteArrayRegion(
        byte_array, 0, len, reinterpret_cast<const jbyte*>(bytes));

   return ScopedJavaLocalRef<jbyteArray>(env, byte_array);
}


VideoSinkWrapper::VideoSinkWrapper(JNIEnv* jni, const JavaRef<jobject>& j_sink)
    : j_sink_(jni, j_sink) {}

VideoSinkWrapper::~VideoSinkWrapper() {}

void VideoSinkWrapper::OnFrame(const VideoFrame& frame) {
  JNIEnv* jni = AttachCurrentThreadIfNeeded();
  ScopedJavaLocalRef<jobject> j_frame = NativeToJavaVideoFrame(jni, frame);

   jint augLen =0;


   ScopedJavaLocalRef<jbyteArray> augData ;


  if (frame.video_frame_buffer()->type() == VideoFrameBuffer::Type::kAugmented) { //arvind umrao

      AugmentedVideoFrameBuffer* augmentedBuffer = static_cast<AugmentedVideoFrameBuffer*>(frame.video_frame_buffer().get()); ;
      augLen = augmentedBuffer->GetAugmentingDataSize();
      augData = ToJavaByteArray( jni, augmentedBuffer->GetAugmentingData(),  augmentedBuffer->GetAugmentingDataSize() );
  }
  Java_VideoSink_onFrame(jni, j_sink_, j_frame, augLen, augData);
  //ReleaseJavaVideoFrame(jni, augData);
  ReleaseJavaVideoFrame(jni, j_frame);
}

}  // namespace jni
}  // namespace webrtc
