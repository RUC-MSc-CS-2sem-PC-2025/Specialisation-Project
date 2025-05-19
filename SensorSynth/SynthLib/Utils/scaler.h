
#pragma once
#ifndef SCLR_H
#define SCLR_H

#ifdef __cplusplus

namespace sensorsynth
{
    class Scaler
    {
    public:
        static inline int SensorValueToMidi(float value)
        {
            return static_cast<int>(value * 127.0f + 0.5f);
        };

        /*
            Converts Sensor reading its corespongin frequency;
            Range = 20kHz
            Function: 20000 / value;
        */
        static inline float SensorValueToFrequency(float value)
        {
            return 20000 / value;
        };

        /*
            Converts MIDI note to its corespongin frequency;
            Tunning = 440Hz
            Function: 440.0f * pow(2.0f, (midi_note - 69.0f) / 12.0f);
         */
        static inline float MidiNoteToFrequency(int midi_note)
        {
            return 440.0f * pow(2.0f, (midi_note - 69.0f) / 12.0f);
        };

        /*
            Converts Frequency to semitone
            key_freq = root frequenct
            semitone = how many steps
        */
        static inline float CalculateSemitoneFromFreq(float key_freq, float semitone)
        {
            return key_freq * pow(2.0f, semitone / 12.0f);
        };

        /*
            Flips the sensor value
            value = range 0.0f - 1.0f
        */
        static inline float ReverseSensorValue(float value)
        {
            return 1.0f - value;
        };

        /*
            Converts a sensor value into Midi note and locks the range within a numebr of octaves, can be offset by midi starting point
            value = sensor value 0.0f - 1.0f
            num_octaves = the range of loop
            midi_starting_point =
        */
        static inline float OctaveRepeater(float value, u_int8_t num_octaves, u_int8_t midi_starting_point)
        {
            if (num_octaves == 0)
                return midi_starting_point;
            return (static_cast<int>(value * 127.0f + 0.5f) % (12 * num_octaves)) + midi_starting_point;
        };

    private:
    };
}

#endif
#endif
