# Bluesend

A simple audio plugin to send raw audio data to any connected bluetooth device (i.e Raspberry Pi).

Based on [WDL / IPlug (Oli Larkin Edition)][wdl-ol-github].

Part of my SD #22 District Project Scholarship submission.

## Installing bluetooth-serial-port

This project relies on [Agamnentzar/bluetooth-serial-port][bluetooth-serial-port-github] to connect to bluetooth devices. I've got it set up as a git submodule. So, after cloning:

```bash
$ git submodule init
$ git submodule update
$ cd bluetooth-serial-port
$ cmake .
$ cd ..
```

If you don't have CMake, you can get it [here][cmake-page]. Or, if you're on Windows you can install it via [chocolatey][chocolatey-page], i.e:

```powershell
> choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
```

## Links to audio SDKs (for future reference)

- [VST SDKs][vst-sdks-download] &mdash; Contains VST2 and VST3 SDKs. I haven't tested the included ASIO SDK. Feel free to try it out yourself, and tell me what happens!
- [ASIO SDK][rtaudio-with-asio-sdk-download] &mdash; The RtAudio download contains a working ASIO SDK (for standalone builds on Windows). Use that.

[bluetooth-serial-port-github]: https://github.com/Agamnentzar/bluetooth-serial-port
[cmake-page]: https://cmake.org
[chocolatey-page]: https://chocolatey.org
[wdl-ol-github]: https://github.com/olilarkin/wdl-ol
[vst-sdks-download]: http://www.steinberg.net/en/company/developers.html
[rtaudio-with-asio-sdk-download]: https://www.music.mcgill.ca/~gary/rtaudio/