
## Fetching deps

```bash
git submodule update --init
```

## Apply WIP CMake changes to Dawn and Tint

* dawn: https://dawn-review.googlesource.com/c/dawn/+/64482
* tint: https://dawn-review.googlesource.com/c/tint/+/64500

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

require('../dawn_node/build/dawnnode.node'); // Path to dawn_node build.
require('./src/common/tools/setup-ts-in-node.js');
require('./src/common/runtime/cmdline.ts');
```
