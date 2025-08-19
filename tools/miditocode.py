import mido

NOTE_NAMES = ["C", "C_SHARP", "D", "D_SHARP", "E", "F", "F_SHARP", "G", "G_SHARP", "A", "A_SHARP", "B"]
MAX_NOTE = 108

# General MIDI instrument names (0–127)
GM_INSTRUMENTS = [
    "Acoustic Grand Piano","Bright Acoustic Piano","Electric Grand Piano","Honky-tonk Piano",
    "Electric Piano 1","Electric Piano 2","Harpsichord","Clavinet",
    "Celesta","Glockenspiel","Music Box","Vibraphone",
    "Marimba","Xylophone","Tubular Bells","Dulcimer",
    "Drawbar Organ","Percussive Organ","Rock Organ","Church Organ",
    "Reed Organ","Accordion","Harmonica","Tango Accordion",
    "Acoustic Guitar (nylon)","Acoustic Guitar (steel)","Electric Guitar (jazz)","Electric Guitar (clean)",
    "Electric Guitar (muted)","Overdriven Guitar","Distortion Guitar","Guitar harmonics",
    "Acoustic Bass","Electric Bass (finger)","Electric Bass (pick)","Fretless Bass",
    "Slap Bass 1","Slap Bass 2","Synth Bass 1","Synth Bass 2",
    "Violin","Viola","Cello","Contrabass",
    "Tremolo Strings","Pizzicato Strings","Orchestral Harp","Timpani",
    "String Ensemble 1","String Ensemble 2","SynthStrings 1","SynthStrings 2",
    "Choir Aahs","Voice Oohs","Synth Voice","Orchestra Hit",
    "Trumpet","Trombone","Tuba","Muted Trumpet",
    "French Horn","Brass Section","SynthBrass 1","SynthBrass 2",
    "Soprano Sax","Alto Sax","Tenor Sax","Baritone Sax",
    "Oboe","English Horn","Bassoon","Clarinet",
    "Piccolo","Flute","Recorder","Pan Flute",
    "Blown Bottle","Shakuhachi","Whistle","Ocarina",
    "Lead 1 (square)","Lead 2 (sawtooth)","Lead 3 (calliope)","Lead 4 (chiff)",
    "Lead 5 (charang)","Lead 6 (voice)","Lead 7 (fifths)","Lead 8 (bass+lead)",
    "Pad 1 (new age)","Pad 2 (warm)","Pad 3 (polysynth)","Pad 4 (choir)",
    "Pad 5 (bowed)","Pad 6 (metallic)","Pad 7 (halo)","Pad 8 (sweep)",
    "FX 1 (rain)","FX 2 (soundtrack)","FX 3 (crystal)","FX 4 (atmosphere)",
    "FX 5 (brightness)","FX 6 (goblins)","FX 7 (echoes)","FX 8 (sci-fi)",
    "Sitar","Banjo","Shamisen","Koto",
    "Kalimba","Bag pipe","Fiddle","Shanai",
    "Tinkle Bell","Agogo","Steel Drums","Woodblock",
    "Taiko Drum","Melodic Tom","Synth Drum","Reverse Cymbal",
    "Guitar Fret Noise","Breath Noise","Seashore","Bird Tweet",
    "Telephone Ring","Helicopter","Applause","Gunshot"
]

def note_to_string(note):
    octave = note // 12 - 1
    base = NOTE_NAMES[note % 12]
    if "_SHARP" in base:
        return f"{base.split('_')[0]}{octave}_SHARP"
    return f"{base}{octave}"

def get_channel_instruments(midi_path):
    mid = mido.MidiFile(midi_path)
    instruments = {}
    for track in mid.tracks:
        for msg in track:
            if msg.type == "program_change":
                instruments[msg.channel] = f"Program {msg.program}"  # your GM mapping
    return instruments

def midi_to_beepns(midi_path, channel=0, threaded=True, octave_shift=0):
    mid = mido.MidiFile(midi_path)
    ticks_per_beat = mid.ticks_per_beat
    tempo = 500000  # default 120bpm

    for track in mid.tracks:
        for msg in track:
            if msg.type == "set_tempo":
                tempo = msg.tempo
                break

    output = []
    func_name = "beepns" if threaded else "beep"

    events = []
    abs_time = 0
    for msg in mido.merge_tracks(mid.tracks):
        abs_time += mido.tick2second(msg.time, ticks_per_beat, tempo) * 1000
        if hasattr(msg, "channel") and msg.channel != channel:
            continue
        if msg.type == "note_on" and msg.velocity > 0:
            events.append((abs_time, "on", msg.note))
        elif msg.type == "note_off" or (msg.type == "note_on" and msg.velocity == 0):
            events.append((abs_time, "off", msg.note))

    events.sort(key=lambda x: x[0])

    current_time = 0
    active_notes = {}

    for event_time, typ, note in events:
        # apply octave shift
        shifted_note = note + octave_shift * 12
        if shifted_note < 0: shifted_note = 0
        if shifted_note > MAX_NOTE: shifted_note = MAX_NOTE

        gap = int(event_time - current_time)
        if gap > 0 and threaded and not active_notes:
            output.append(f"wait({gap});")
        current_time = event_time

        if typ == "on":
            active_notes[shifted_note] = current_time
        elif typ == "off" and shifted_note in active_notes:
            start_time = active_notes.pop(shifted_note)
            duration = int(current_time - start_time)
            note_name = note_to_string(shifted_note)
            output.append(f"{func_name}({note_name}, {duration});")
            if threaded:
                output.append(f"wait({duration});")

    return "\n".join(output)

if __name__ == "__main__":
    path = input("Enter MIDI file path: ")
    instruments = get_channel_instruments(path)
    print("Detected instruments per channel:")
    for ch, name in instruments.items():
        print(f"  Channel {ch}: {name}")

    channel = int(input("Pick channel to convert: "))
    threaded_input = input("Use threaded notes? (y/n): ").lower().startswith("y")
    octave_shift = int(input("Octave shift (max note C8): "))
    code = midi_to_beepns(path, channel=channel, threaded=threaded_input, octave_shift=octave_shift)
    print("===== GENERATED CODE =====")
    print(code)