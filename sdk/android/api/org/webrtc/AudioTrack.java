/*
 *  Copyright 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package org.webrtc;

import java.util.IdentityHashMap;

/** Java wrapper for a C++ AudioTrackInterface */
public class AudioTrack extends MediaStreamTrack {
  private final IdentityHashMap<AudioSink, Long> sinks = new IdentityHashMap<AudioSink, Long>();

  public AudioTrack(long nativeTrack) {
    super(nativeTrack);
  }

  /** Sets the volume for the underlying MediaSource. Volume is a gain value in the range
   *  0 to 10.
   */
  public void setVolume(double volume) {
    nativeSetVolume(getNativeAudioTrack(), volume);
  }

  /**
   * Adds a AudioSink to the track.
   */
  public void addSink(AudioSink sink) {
    final long nativeSink = nativeWrapSink(sink);
    sinks.put(sink, nativeSink);
    nativeAddSink(getNativeAudioTrack(), nativeSink);
  }

  /**
   * Removes a AudioSink from the track.
   *
   * If the AudioSink was not attached to the track, this is a no-op.
   */
  public void removeSink(AudioSink sink) {
    final long nativeSink = sinks.remove(sink);
    if (nativeSink != 0) {
      nativeRemoveSink(getNativeAudioTrack(), nativeSink);
      nativeFreeSink(nativeSink);
    }
  }

  /** Returns a pointer to webrtc::AudioTrackInterface. */
  long getNativeAudioTrack() {
    return getNativeMediaStreamTrack();
  }

  private static native void nativeSetVolume(long track, double volume);
  private static native void nativeAddSink(long track, long sink);
  private static native void nativeRemoveSink(long track, long sink);
  private static native long nativeWrapSink(AudioSink sink);
  private static native void nativeFreeSink(long sink);
}
