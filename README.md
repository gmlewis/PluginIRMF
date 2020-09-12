# PluginIRMF

PluginIRMF is a [SHADERed](https://shadered.org) plugin that simplifies
the process of editing your IRMF shaders.

See [irmf.io](http://irmf.io) for more details.

![Screenshot](./screenshot.png)

## How to build

Clone the project:
```bash
git clone https://github.com/gmlewis/PluginIRMF
```

### Linux

1. Install OpenSSL (libcrypto & libssl).

2. Build:
```bash
cmake .
make
```

### Windows

1. Install libcrypto & libssl through your favorite package manager (I recommend vcpkg)
2. Run cmake-gui and set CMAKE_TOOLCHAIN_FILE variable
3. Press Configure and then Generate if no errors occured
4. Open the .sln and build the project!

## How to use

This plugin requires at least SHADERed v1.4.

Copy the .dll/.so file to the `plugins/PluginIRMF` folder in your SHADERed's
installation directory.

After you start SHADERed, click on `File -> Import IRMF shader`. Enter IRMF URL
and choose a path where you want to save the SHADERed project. Press `Save`.

Currently, IRMF shader URLs must be hosted on GitHub and end in '.irmf'.

Valid URL examples:

* https://gmlewis.github.io/irmf-editor/?s=github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
* https://github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
* https://raw.githubusercontent.com/gmlewis/irmf/master/examples/001-sphere/sphere-1.irmf

----------------------------------------------------------------------

# License

Copyright 2020 Glenn M. Lewis. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
