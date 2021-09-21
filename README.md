
## Fetching deps

```bash
git submodule update --init
```

## Apply WIP CMake change to Dawn

```bash
cd third_party/dawn
git fetch https://dawn.googlesource.com/dawn refs/changes/43/64743/1 && git checkout FETCH_HEAD
```

## Building

```bash
mkdir build
cd build
cmake .. -GNinja
ninja
```

## Running CTS

Run this `.js` script from the root of a [WebGPU CTS](https://github.com/gpuweb/cts) checkout:

```js
#!/usr/bin/env node

require('../dawn-node/build/dawnnode.node'); // Path to dawn_node build.
require('./src/common/tools/setup-ts-in-node.js');
require('./src/common/runtime/cmdline.ts');
```
