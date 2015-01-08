#!/usr/bin/env python2
# coding: utf-8

from pylab import *
import scipy.io.wavfile as wav
import scipy.signal as signal

def integrate(xs):
    current = 0
    ys = []
    for xx in xs:
        current += xx
        ys.append(current)
    return array(ys)

def decompose(xs):
    last_sample = None
    running_length = 0
    for sample in xs:
        if last_sample is None:
            last_sample = sample
        running_length += 1
        if sample != last_sample:
            yield last_sample, running_length
            running_length = 0
            last_sample = sample
    yield last_sample, running_length

def peak(xs, thresh=.5, min_peak_width=10):
    thresholded_signal = map(lambda xx: 1 if xx>thresh else -1 if xx<-thresh else 0, xs)
    peak_signal = []
    for value, length in decompose(thresholded_signal):
        if value == 0:
            peak_signal.append(zeros(length))
            continue
        if length < min_peak_width:
            peak_signal.append(zeros(length))
        else:
            peak_signal.append([value])
            peak_signal.append(zeros(length-1))
    peak_signal = hstack(peak_signal)
    return array(peak_signal)

input_filename = "vierge.wav"

# load audio file
sample_rate, audio_data = wav.read(input_filename)
audio_data = audio_data.astype("double")/32768

# remove stereo
if len(audio_data.shape) == 2:
    audio_data /= audio_data.shape[1]
    audio_data = audio_data.sum(axis=1)

times = arange(audio_data.size, dtype='double')/sample_rate

# filter noise and peak detection
filter_data = signal.firwin(1024, [50, 2500], pass_zero=False, nyq=sample_rate/2)
audio_data_filter = convolve(audio_data, filter_data, 'same')
audio_data_peak = peak(audio_data_filter)
audio_data_integrate = integrate(-audio_data_peak)

figure()
title("clean signal")
plot(times, audio_data)
plot(times, audio_data_filter)

figure()
title("peak signal")
plot(times, audio_data_filter)
plot(times, audio_data_peak)
plot(times, audio_data_integrate)

pulse_widths = array([length for value, length in decompose(audio_data_integrate)])
width_thresh = 133
raw_decoded = pulse_widths > width_thresh
print raw_decoded
decoded = []
for value, length in decompose(raw_decoded):
    if value:
        decoded.append(ones(length))
    else:
        assert( length % 2 == 0 )
        decoded.append(zeros(length/2))
decoded = hstack(decoded)
print decoded

figure()
title("pulse width histogram")
hist(pulse_widths, 100, (0,300))
axvline(width_thresh, color="r")


show()
